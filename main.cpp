/*
 * File     : main.cpp
 * Purpose  : Entry point for the console-based Social Media Platform.
 *            Defines the global currentUser pointer.
 *            Runs the main menu loop.
 *            Includes all module headers.
 *
 * HOW TO RUN IN VISUAL STUDIO:
 *   1. Create an Empty C++ Project
 *   2. Add ALL .cpp files to Source Files (NOT .h files — those are auto-included)
 *   3. Press Ctrl+F5 to build and run
 *
 * FILE LIST TO ADD IN VISUAL STUDIO:
 *   Source Files:
 *     main.cpp
 *     UserManager.cpp
 *     SocialNetwork.cpp
 *     PostManager.cpp
 *     Stories.cpp
 *     Notifications.cpp
 *     Analytics.cpp
 *     Messaging.cpp
 *     Integration.cpp
 *   Header Files (for reference — do NOT compile separately):
 *     globals.h
 *     UserManager.h
 *     SocialNetwork.h
 *     PostManager.h
 *     Stories.h
 *     Notifications.h
 *     Analytics.h
 *     Messaging.h
 *     Integration.h
 */

#include "globals.h"
#include "UserManager.h"
#include "SocialNetwork.h"
#include "PostManager.h"
#include "Stories.h"
#include "Notifications.h"
#include "Analytics.h"
#include "Messaging.h"
#include "Integration.h"

/* ═══════════════════════════════════════════════════════
   GLOBAL VARIABLE DEFINITIONS
   (declared extern in globals.h — defined ONCE here)
═══════════════════════════════════════════════════════ */
User* currentUser = nullptr;   // tracks who is currently logged in

/* ══════════════════════════════════════════════════════════════
 * showMainMenu
 * Displays the main menu header and all options.
 * Always shows login status at the top.
 ══════════════════════════════════════════════════════════════ */
void showMainMenu() {
    cout << "\n=============================================" << endl;
    if (currentUser != nullptr)
        cout << "  Logged in as: " << currentUser->userName << endl;
    else
        cout << "  Not logged in" << endl;
    cout << "=============================================" << endl;
    cout << "       CONSOLE SOCIAL MEDIA PLATFORM        " << endl;
    cout << "=============================================" << endl;
    cout << "  1. User Management     (Hash Table)       " << endl;
    cout << "  2. Social Network      (Graph)            " << endl;
    cout << "  3. Posts & Feed         (Linked Lists)    " << endl;
    cout << "  4. Stories             (Circular List)    " << endl;
    cout << "  5. Notifications       (Queue)            " << endl;
    cout << "  6. Analytics & Ranking (AVL Tree)         " << endl;
    cout << "  7. Messaging System    (Stack)            " << endl;
    cout << "  8. Exit                                   " << endl;
    cout << "=============================================" << endl;
    cout << "  Enter choice: ";
}

/* ══════════════════════════════════════════════════════════════
 * main
 * Program entry point. Runs the main menu loop until user exits.
 ══════════════════════════════════════════════════════════════ */
int main() {
    int choice;

    do {
        showMainMenu();
        cin >> choice;

        switch (choice) {
            case 1: userManagementMenu();  break;
            case 2: socialNetworkMenu();   break;
            case 3: postsMenu();           break;
            case 4: storiesMenu();         break;
            case 5: notificationsMenu();   break;
            case 6: analyticsMenu();       break;
            case 7: messagingMenu();       break;
            case 8:
                cout << "\nExiting... Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please enter 1-8." << endl;
        }

    } while (choice != 8);

    return 0;
}
