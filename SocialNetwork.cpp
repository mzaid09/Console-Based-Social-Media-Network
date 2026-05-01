/*
 * File     : SocialNetwork.cpp
 * Module   : B — Social Network (Graph with Adjacency Lists)
 * Purpose  : Implements all graph operations for the friend system.
 *            Adjacency lists are stored as Edge* linked lists inside User nodes.
 * Member   : Member 1
 */

#include "SocialNetwork.h"
#include "UserManager.h"   // for hashSearch()

/* ══════════════════════════════════════════════════════════════
 * addEdge (internal helper)
 * Adds a one-directional edge from 'fromUser' to 'toName'.
 * Does NOT check duplicates — caller must check first.
 ══════════════════════════════════════════════════════════════ */
static void addEdge(User* fromUser, string toName) {
    Edge* newEdge       = new Edge();
    newEdge->friendUserName = toName;
    newEdge->next       = fromUser->friendList;  // insert at head
    fromUser->friendList = newEdge;
}

/* ══════════════════════════════════════════════════════════════
 * alreadyFriends (internal helper)
 * Returns true if 'targetName' already exists in user's edge list.
 ══════════════════════════════════════════════════════════════ */
static bool alreadyFriends(User* user, string targetName) {
    Edge* curr = user->friendList;
    while (curr != nullptr) {
        if (curr->friendUserName == targetName) return true;
        curr = curr->next;
    }
    return false;
}

/* ══════════════════════════════════════════════════════════════
 * addFriend
 * Verifies both users exist, checks no duplicate friendship,
 * then adds edges in BOTH directions (undirected graph).
 ══════════════════════════════════════════════════════════════ */
void addFriend(string user1, string user2) {
    if (currentUser == nullptr) {
        cout << "Error: Please login first." << endl;
        return;
    }

    User* u1 = hashSearch(user1);
    User* u2 = hashSearch(user2);

    if (u1 == nullptr) { cout << "Error: User '" << user1 << "' not found." << endl; return; }
    if (u2 == nullptr) { cout << "Error: User '" << user2 << "' not found." << endl; return; }
    if (user1 == user2) { cout << "Error: Cannot add yourself as a friend." << endl; return; }

    if (alreadyFriends(u1, user2)) {
        cout << user1 << " and " << user2 << " are already friends." << endl;
        return;
    }

    addEdge(u1, user2);  // A → B
    addEdge(u2, user1);  // B → A (undirected)

    cout << "\n✓ " << user1 << " and " << user2 << " are now friends!" << endl;
}

/* ══════════════════════════════════════════════════════════════
 * removeEdge (internal helper)
 * Removes a one-directional edge for 'targetName' from user's list.
 ══════════════════════════════════════════════════════════════ */
static void removeEdge(User* fromUser, string targetName) {
    Edge* curr = fromUser->friendList;
    Edge* prev = nullptr;

    while (curr != nullptr) {
        if (curr->friendUserName == targetName) {
            if (prev == nullptr)
                fromUser->friendList = curr->next;
            else
                prev->next = curr->next;
            delete curr;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

/* ══════════════════════════════════════════════════════════════
 * removeFriend
 * Removes edges in both directions between user1 and user2.
 ══════════════════════════════════════════════════════════════ */
void removeFriend(string user1, string user2) {
    if (currentUser == nullptr) {
        cout << "Error: Please login first." << endl;
        return;
    }

    User* u1 = hashSearch(user1);
    User* u2 = hashSearch(user2);

    if (u1 == nullptr || u2 == nullptr) {
        cout << "Error: One or both users not found." << endl;
        return;
    }

    removeEdge(u1, user2);
    removeEdge(u2, user1);

    cout << "\n✓ Friendship between " << user1 << " and " << user2 << " removed." << endl;
}

/* ══════════════════════════════════════════════════════════════
 * displayFriends
 * Traverses the user's Edge* friendList and prints each friend name.
 ══════════════════════════════════════════════════════════════ */
void displayFriends(string userName) {
    User* u = hashSearch(userName);
    if (u == nullptr) {
        cout << "User '" << userName << "' not found." << endl;
        return;
    }

    cout << "\nFriends of " << userName << ":" << endl;
    Edge* curr = u->friendList;
    int   count = 0;

    if (curr == nullptr) {
        cout << "  No friends yet." << endl;
        return;
    }

    while (curr != nullptr) {
        cout << "  → " << curr->friendUserName << endl;
        count++;
        curr = curr->next;
    }
    cout << "Total: " << count << " friend(s)" << endl;
}

/* ══════════════════════════════════════════════════════════════
 * BFS
 * Breadth-First Search from startUser.
 * Uses a simple array-based manual queue (no STL queue).
 * Visits all direct friends first, then their friends, etc.
 ══════════════════════════════════════════════════════════════ */
void BFS(string startUser) {
    User* start = hashSearch(startUser);
    if (start == nullptr) {
        cout << "User '" << startUser << "' not found." << endl;
        return;
    }

    // Manual queue using an array of strings
    string bfsQueue[200];
    string visitedNames[200];
    int    visitedCount  = 0;
    int    front = 0, rear = 0;

    // Lambda to check if already visited
    auto isVisited = [&](string name) -> bool {
        for (int i = 0; i < visitedCount; i++)
            if (visitedNames[i] == name) return true;
        return false;
    };

    // Enqueue start
    bfsQueue[rear++] = startUser;
    visitedNames[visitedCount++] = startUser;

    cout << "\nBFS Traversal from '" << startUser << "':" << endl;

    while (front < rear) {
        string current = bfsQueue[front++];
        cout << "  " << current << endl;

        User* currUser = hashSearch(current);
        if (currUser == nullptr) continue;

        Edge* edge = currUser->friendList;
        while (edge != nullptr) {
            if (!isVisited(edge->friendUserName)) {
                bfsQueue[rear++] = edge->friendUserName;
                visitedNames[visitedCount++] = edge->friendUserName;
            }
            edge = edge->next;
        }
    }
}

/* ══════════════════════════════════════════════════════════════
 * DFSHelper (internal recursive helper)
 ══════════════════════════════════════════════════════════════ */
static void DFSHelper(string userName, string visited[], int& visitedCount) {
    // mark as visited
    visited[visitedCount++] = userName;
    cout << "  " << userName << endl;

    User* u = hashSearch(userName);
    if (u == nullptr) return;

    Edge* edge = u->friendList;
    while (edge != nullptr) {
        bool seen = false;
        for (int i = 0; i < visitedCount; i++)
            if (visited[i] == edge->friendUserName) { seen = true; break; }

        if (!seen)
            DFSHelper(edge->friendUserName, visited, visitedCount);

        edge = edge->next;
    }
}

/* ══════════════════════════════════════════════════════════════
 * DFS
 * Depth-First Search from startUser.
 * Uses recursion — goes as deep as possible before backtracking.
 ══════════════════════════════════════════════════════════════ */
void DFS(string startUser) {
    User* start = hashSearch(startUser);
    if (start == nullptr) {
        cout << "User '" << startUser << "' not found." << endl;
        return;
    }

    string visited[200];
    int    visitedCount = 0;

    cout << "\nDFS Traversal from '" << startUser << "':" << endl;
    DFSHelper(startUser, visited, visitedCount);
}

/* ══════════════════════════════════════════════════════════════
 * removeAllEdgesOf
 * Called by Integration (Module H) when a user is deleted.
 * Visits every friend of the deleted user and removes the
 * reverse edge pointing back to the deleted user.
 ══════════════════════════════════════════════════════════════ */
void removeAllEdgesOf(string userName) {
    User* u = hashSearch(userName);
    if (u == nullptr) return;

    // For each friend of this user, remove the reverse edge
    Edge* curr = u->friendList;
    while (curr != nullptr) {
        User* friendUser = hashSearch(curr->friendUserName);
        if (friendUser != nullptr)
            removeEdge(friendUser, userName);
        curr = curr->next;
    }

    // Now delete this user's own edge list
    curr = u->friendList;
    while (curr != nullptr) {
        Edge* temp = curr;
        curr = curr->next;
        delete temp;
    }
    u->friendList = nullptr;
}

/* ══════════════════════════════════════════════════════════════
 * socialNetworkMenu
 * Displays the Social Network submenu and handles user input.
 ══════════════════════════════════════════════════════════════ */
void socialNetworkMenu() {
    int choice;
    do {
        cout << "\n======= SOCIAL NETWORK =======" << endl;
        if (currentUser)
            cout << "Logged in as: " << currentUser->userName << endl;
        else
            cout << "Not logged in" << endl;
        cout << "------------------------------" << endl;
        cout << "1. Add Friend"      << endl;
        cout << "2. Remove Friend"   << endl;
        cout << "3. View Friends"    << endl;
        cout << "4. BFS Traversal"   << endl;
        cout << "5. DFS Traversal"   << endl;
        cout << "6. Back"            << endl;
        cout << "------------------------------" << endl;
        cout << "Choice: "; cin >> choice;

        string u1, u2, name;
        switch (choice) {
            case 1:
                if (!currentUser) { cout << "Please login first." << endl; break; }
                cout << "Your username   : " << currentUser->userName << endl;
                cout << "Friend username : "; cin >> u2;
                addFriend(currentUser->userName, u2);
                break;
            case 2:
                if (!currentUser) { cout << "Please login first." << endl; break; }
                cout << "Remove friend username: "; cin >> u2;
                removeFriend(currentUser->userName, u2);
                break;
            case 3:
                cout << "Enter username: "; cin >> name;
                displayFriends(name);
                break;
            case 4:
                cout << "Start BFS from user: "; cin >> name;
                BFS(name);
                break;
            case 5:
                cout << "Start DFS from user: "; cin >> name;
                DFS(name);
                break;
            case 6: cout << "Returning to main menu..." << endl; break;
            default: cout << "Invalid choice." << endl;
        }
    } while (choice != 6);
}
