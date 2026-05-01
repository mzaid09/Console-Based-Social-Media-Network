/*
 * File     : PostManager.cpp
 * Module   : C — Posts & Feed (Doubly Linked List)
 * Purpose  : Implements all post operations including creating, deleting,
 *            displaying, feed navigation, and liking posts.
 * Member   : Member 2
 */

#include "PostManager.h"
#include "UserManager.h"       // for hashSearch()
#include "Analytics.h"         // for insertPostAVL(), updatePostAVL(), deletePostAVL(), incrementUserActivity()
#include "Notifications.h"     // for enqueueNotification()

// Post ID counter — shared with UserManager.cpp
extern int postIDCounter;

/* ══════════════════════════════════════════════════════════════
 * generatePostID
 * Returns a string like "P001", "P002", etc.
 * Uses a static counter that increments each call.
 ══════════════════════════════════════════════════════════════ */
string generatePostID() {
    string id = "P";
    if (postIDCounter < 10)       id += "00" + to_string(postIDCounter);
    else if (postIDCounter < 100) id += "0"  + to_string(postIDCounter);
    else                          id +=         to_string(postIDCounter);
    postIDCounter++;
    return id;
}

/* ══════════════════════════════════════════════════════════════
 * createPost
 * Inserts a new Post at the HEAD of the logged-in user's
 * doubly linked list. Also updates both AVL trees.
 ══════════════════════════════════════════════════════════════ */
void createPost() {
    if (currentUser == nullptr) {
        cout << "Error: Please login first." << endl;
        return;
    }

    string content;
    cin.ignore();
    cout << "\n--- Create Post ---" << endl;
    cout << "Enter post content: "; getline(cin, content);

    // create new post node
    Post* newPost     = new Post();
    newPost->postID   = generatePostID();
    newPost->content  = content;
    newPost->owner    = currentUser->userName;
    newPost->likes    = 0;
    newPost->prev     = nullptr;

    // insert at HEAD of user's doubly linked list
    newPost->next = currentUser->postHead;
    if (currentUser->postHead != nullptr)
        currentUser->postHead->prev = newPost;
    currentUser->postHead = newPost;

    // MODULE F: insert into AVL post tree with likes = 0
    insertPostAVL(newPost->postID, 0);

    // MODULE F: increment user's activity score
    incrementUserActivity(currentUser->userName);

    cout << "\n✓ Post created! ID: " << newPost->postID << endl;
}

/* ══════════════════════════════════════════════════════════════
 * deletePost
 * Searches the logged-in user's post list for the given ID.
 * Removes it by relinking prev/next pointers, then frees memory.
 * Also removes from AVL post tree.
 ══════════════════════════════════════════════════════════════ */
void deletePost() {
    if (currentUser == nullptr) {
        cout << "Error: Please login first." << endl;
        return;
    }

    string pid;
    cout << "\n--- Delete Post ---" << endl;
    cout << "Enter Post ID to delete: "; cin >> pid;

    Post* curr = currentUser->postHead;

    while (curr != nullptr) {
        if (curr->postID == pid) {
            // relink doubly linked list
            if (curr->prev != nullptr)
                curr->prev->next = curr->next;
            else
                currentUser->postHead = curr->next;   // was head

            if (curr->next != nullptr)
                curr->next->prev = curr->prev;

            // MODULE F: remove from AVL post tree
            deletePostAVL(pid);

            delete curr;
            cout << "\n✓ Post '" << pid << "' deleted." << endl;
            return;
        }
        curr = curr->next;
    }

    cout << "Post '" << pid << "' not found in your posts." << endl;
}

/* ══════════════════════════════════════════════════════════════
 * displayMyPosts
 * Traverses the current user's post list from head and prints each.
 ══════════════════════════════════════════════════════════════ */
void displayMyPosts() {
    if (currentUser == nullptr) {
        cout << "Error: Please login first." << endl;
        return;
    }

    cout << "\n--- Posts by " << currentUser->userName << " ---" << endl;
    Post* curr = currentUser->postHead;

    if (curr == nullptr) {
        cout << "No posts yet." << endl;
        return;
    }

    while (curr != nullptr) {
        cout << "[" << curr->postID << "] " << curr->owner
             << " | Likes: " << curr->likes << endl;
        cout << "  \"" << curr->content << "\"" << endl;
        cout << "  ─────────────────────────" << endl;
        curr = curr->next;
    }
}

/* ══════════════════════════════════════════════════════════════
 * navigateFeed
 * Collects all posts from all users into a flat array,
 * then lets user navigate using N (next) and P (prev).
 * Demonstrates doubly linked list navigation.
 ══════════════════════════════════════════════════════════════ */
void navigateFeed() {
    if (currentUser == nullptr) {
        cout << "Error: Please login first." << endl;
        return;
    }

    // Collect all posts from all users via hash table (Module A extern)
    extern User* hashTable[];
    Post* feedArray[500];
    int   feedSize = 0;

    for (int i = 0; i < TABLE_SIZE; i++) {
        User* u = hashTable[i];
        while (u != nullptr) {
            Post* p = u->postHead;
            while (p != nullptr && feedSize < 500) {
                feedArray[feedSize++] = p;
                p = p->next;
            }
            u = u->next;
        }
    }

    if (feedSize == 0) {
        cout << "No posts in feed." << endl;
        return;
    }

    int    idx = 0;
    char   nav = 'S';

    while (nav != 'B' && nav != 'b') {
        Post* p = feedArray[idx];
        cout << "\n--- Feed Post " << (idx + 1) << " of " << feedSize << " ---" << endl;
        cout << "ID: " << p->postID << " | Owner: " << p->owner
             << " | Likes: " << p->likes << endl;
        cout << "\"" << p->content << "\"" << endl;
        cout << "[N] Next  [P] Prev  [B] Back : ";
        cin >> nav;

        if ((nav == 'N' || nav == 'n') && idx < feedSize - 1) idx++;
        else if ((nav == 'P' || nav == 'p') && idx > 0)       idx--;
        else if (nav != 'B' && nav != 'b')
            cout << "Already at the " << (idx == 0 ? "first" : "last") << " post." << endl;
    }
}

/* ══════════════════════════════════════════════════════════════
 * likePost
 * Finds the post by ID in the owner's post list.
 * Increments likes, updates AVL tree, sends notification.
 ══════════════════════════════════════════════════════════════ */
void likePost() {
    if (currentUser == nullptr) {
        cout << "Error: Please login first." << endl;
        return;
    }

    string pid, ownerName;
    cout << "\n--- Like a Post ---" << endl;
    cout << "Post ID     : "; cin >> pid;
    cout << "Post owner  : "; cin >> ownerName;

    User* owner = hashSearch(ownerName);
    if (owner == nullptr) {
        cout << "User '" << ownerName << "' not found." << endl;
        return;
    }

    Post* curr = owner->postHead;
    while (curr != nullptr) {
        if (curr->postID == pid) {
            curr->likes++;

            // MODULE F: update AVL post tree with new likes count
            updatePostAVL(pid, curr->likes);

            // MODULE E: send notification to the post owner
            string msg = currentUser->userName + " liked your post " + pid;
            enqueueNotification(owner, msg);

            cout << "\n✓ You liked " << ownerName << "'s post! Likes: " << curr->likes << endl;
            return;
        }
        curr = curr->next;
    }
    cout << "Post '" << pid << "' not found for user '" << ownerName << "'." << endl;
}

/* ══════════════════════════════════════════════════════════════
 * deleteAllPostsOf
 * Called by Module H during account deletion.
 * Traverses and deletes every post node in the user's list.
 * Also removes each post from the AVL post tree.
 ══════════════════════════════════════════════════════════════ */
void deleteAllPostsOf(User* user) {
    Post* curr = user->postHead;
    while (curr != nullptr) {
        Post* temp = curr;
        curr = curr->next;
        deletePostAVL(temp->postID);   // Module F cleanup
        delete temp;
    }
    user->postHead = nullptr;
}

/* ══════════════════════════════════════════════════════════════
 * postsMenu
 * Displays the Posts & Feed submenu and handles input.
 ══════════════════════════════════════════════════════════════ */
void postsMenu() {
    int choice;
    do {
        cout << "\n======= POSTS & FEED =======" << endl;
        if (currentUser)
            cout << "Logged in as: " << currentUser->userName << endl;
        else
            cout << "Not logged in" << endl;
        cout << "----------------------------" << endl;
        cout << "1. Create Post"       << endl;
        cout << "2. Delete Post"       << endl;
        cout << "3. View My Posts"     << endl;
        cout << "4. Navigate Feed"     << endl;
        cout << "5. Like a Post"       << endl;
        cout << "6. Back"              << endl;
        cout << "----------------------------" << endl;
        cout << "Choice: "; cin >> choice;

        switch (choice) {
            case 1: createPost();    break;
            case 2: deletePost();    break;
            case 3: displayMyPosts(); break;
            case 4: navigateFeed();  break;
            case 5: likePost();      break;
            case 6: cout << "Returning to main menu..." << endl; break;
            default: cout << "Invalid choice." << endl;
        }
    } while (choice != 6);
}
