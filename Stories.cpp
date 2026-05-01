/*
 * File     : Stories.cpp
 * Module   : D — Stories (Circular Linked List)
 * Purpose  : Implements the circular linked list for the stories feature.
 *            One global list holds stories from all users.
 * Member   : Member 2
 */

#include "Stories.h"

// Define the global story list variables (declared extern in globals.h)
Story* storyHead  = nullptr;
Story* storyTail  = nullptr;
int    storyCount = 0;

/* ══════════════════════════════════════════════════════════════
 * addStory
 * Creates a new Story node and inserts it into the circular list.
 * After insertion, tail->next always points back to head.
 ══════════════════════════════════════════════════════════════ */
void addStory() {
    if (currentUser == nullptr) {
        cout << "Error: Please login first." << endl;
        return;
    }

    string content;
    cin.ignore();
    cout << "\n--- Add Story ---" << endl;
    cout << "Story content: "; getline(cin, content);

    Story* newStory          = new Story();
    newStory->userName       = currentUser->userName;
    newStory->storyContent   = content;
    newStory->next           = nullptr;

    if (storyHead == nullptr) {
        // first story — points to itself (circular)
        storyHead         = newStory;
        storyTail         = newStory;
        newStory->next    = storyHead;   // circular: points to self
    } else {
        // insert after tail
        storyTail->next   = newStory;
        storyTail         = newStory;
        storyTail->next   = storyHead;   // restore circular link to head
    }

    storyCount++;
    cout << "\n✓ Story added!" << endl;
}

/* ══════════════════════════════════════════════════════════════
 * viewStories
 * Traverses the circular list exactly storyCount times.
 * The counter prevents an infinite loop.
 ══════════════════════════════════════════════════════════════ */
void viewStories() {
    if (storyHead == nullptr || storyCount == 0) {
        cout << "\nNo stories to display." << endl;
        return;
    }

    cout << "\n--- Viewing Stories ---" << endl;
    Story* curr  = storyHead;
    int    count = storyCount;   // use counter to stop the circular loop

    int index = 1;
    while (count > 0) {
        cout << "Story " << index << " [" << curr->userName << "]: \""
             << curr->storyContent << "\"" << endl;
        curr = curr->next;    // moves forward; wraps to head automatically
        count--;
        index++;
    }

    cout << "\n--- End of stories (" << storyCount << " total) ---" << endl;
}

/* ══════════════════════════════════════════════════════════════
 * removeStoriesOf
 * Called by Module H during account deletion.
 * Searches the circular list for stories owned by userName.
 * Removes each found node while maintaining the circular structure.
 ══════════════════════════════════════════════════════════════ */
void removeStoriesOf(string userName) {
    if (storyHead == nullptr) return;

    // Keep removing until no story by this user remains
    bool removed = true;
    while (removed && storyCount > 0) {
        removed = false;
        Story* curr = storyHead;
        Story* prev = storyTail;   // in a circular list, tail is "before head"

        for (int i = 0; i < storyCount; i++) {
            if (curr->userName == userName) {
                // remove curr from circular list
                if (storyCount == 1) {
                    // only one node
                    storyHead = nullptr;
                    storyTail = nullptr;
                } else {
                    prev->next = curr->next;
                    if (curr == storyHead) storyHead = curr->next;
                    if (curr == storyTail) storyTail = prev;
                }
                delete curr;
                storyCount--;
                removed = true;
                break;
            }
            prev = curr;
            curr = curr->next;
        }
    }
}

/* ══════════════════════════════════════════════════════════════
 * storiesMenu
 * Displays the Stories submenu and handles input.
 ══════════════════════════════════════════════════════════════ */
void storiesMenu() {
    int choice;
    do {
        cout << "\n======= STORIES =======" << endl;
        if (currentUser)
            cout << "Logged in as: " << currentUser->userName << endl;
        else
            cout << "Not logged in" << endl;
        cout << "-----------------------" << endl;
        cout << "1. Add Story"   << endl;
        cout << "2. View Stories" << endl;
        cout << "3. Back"        << endl;
        cout << "-----------------------" << endl;
        cout << "Choice: "; cin >> choice;

        switch (choice) {
            case 1: addStory();    break;
            case 2: viewStories(); break;
            case 3: cout << "Returning to main menu..." << endl; break;
            default: cout << "Invalid choice." << endl;
        }
    } while (choice != 3);
}
