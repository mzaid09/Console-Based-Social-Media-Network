/*
 * File     : UserManager.cpp
 * Module   : A — User Directory (Hash Table with Chaining)
 * Purpose  : Implements all hash table operations for user management.
 *            This file defines the hash table array and all user functions.
 * Member   : Member 1
 */

#include "UserManager.h"
#include "Analytics.h"      // to call AVL insert on register
#include "Integration.h"    // to call full cleanup on delete

/* ─── Hash Table Definition ─── */
User* hashTable[TABLE_SIZE] = { nullptr };  // all buckets start empty

/* ─── Global post ID counter (used by PostManager.cpp too) ─── */
int postIDCounter = 1;

/* ══════════════════════════════════════════════════════════════
 * hashFunction
 * Input  : userName string
 * Output : integer index between 0 and TABLE_SIZE-1
 * Logic  : sum ASCII values of all characters, then mod TABLE_SIZE
 ══════════════════════════════════════════════════════════════ */
int hashFunction(string userName) {
    int sum = 0;
    for (int i = 0; i < (int)userName.length(); i++) {
        sum += (int)userName[i];
    }
    return sum % TABLE_SIZE;
}

/* ══════════════════════════════════════════════════════════════
 * hashSearch
 * Searches hash table for a user by userName.
 * Returns pointer to User node if found, nullptr if not found.
 * Called by EVERY other module before performing any operation.
 ══════════════════════════════════════════════════════════════ */
User* hashSearch(string userName) {
    int index = hashFunction(userName);
    User* current = hashTable[index];

    while (current != nullptr) {
        if (current->userName == userName) {
            return current;  // found
        }
        current = current->next;
    }
    return nullptr;  // not found
}

/* ══════════════════════════════════════════════════════════════
 * registerUser
 * Prompts user for account details, creates a User node,
 * inserts into hash table via chaining, then inserts into
 * the AVL user activity tree with initial value 0.
 ══════════════════════════════════════════════════════════════ */
void registerUser() {
    string uname, pass, email, bio;

    cout << "\n--- Register New User ---" << endl;
    cout << "Enter username  : "; cin >> uname;

    // check duplicate
    if (hashSearch(uname) != nullptr) {
        cout << "Error: Username '" << uname << "' already exists." << endl;
        return;
    }

    cout << "Enter password  : "; cin >> pass;
    cout << "Enter email     : "; cin >> email;
    cin.ignore();
    cout << "Enter bio       : "; getline(cin, bio);

    // create new user node
    User* newUser       = new User();
    newUser->userName   = uname;
    newUser->password   = pass;
    newUser->email      = email;
    newUser->bio        = bio;
    newUser->lastActive = "2024-12-01";  // TODO: use ctime for real timestamp

    // insert into hash table at correct bucket (insert at head of chain)
    int index             = hashFunction(uname);
    newUser->next         = hashTable[index];
    hashTable[index]      = newUser;

    // MODULE F: insert into AVL user tree with activity = 0
    insertUserAVL(uname, 0);

    cout << "\n✓ User '" << uname << "' registered successfully!" << endl;
}

/* ══════════════════════════════════════════════════════════════
 * loginUser
 * Searches hash table for the username, compares password,
 * and sets the global currentUser pointer on success.
 ══════════════════════════════════════════════════════════════ */
void loginUser() {
    if (currentUser != nullptr) {
        cout << "Already logged in as: " << currentUser->userName << endl;
        cout << "Please logout first." << endl;
        return;
    }

    string uname, pass;
    cout << "\n--- Login ---" << endl;
    cout << "Enter username: "; cin >> uname;
    cout << "Enter password: "; cin >> pass;

    User* found = hashSearch(uname);

    if (found == nullptr) {
        cout << "Error: User '" << uname << "' not found." << endl;
        return;
    }
    if (found->password != pass) {
        cout << "Error: Incorrect password." << endl;
        return;
    }

    currentUser = found;  // set the global session pointer
    cout << "\n✓ Login successful! Welcome, " << currentUser->userName << "." << endl;
}

/* ══════════════════════════════════════════════════════════════
 * logoutUser
 * Clears the global currentUser pointer (ends the session).
 ══════════════════════════════════════════════════════════════ */
void logoutUser() {
    if (currentUser == nullptr) {
        cout << "No user is currently logged in." << endl;
        return;
    }
    cout << "Goodbye, " << currentUser->userName << "!" << endl;
    currentUser = nullptr;
}

/* ══════════════════════════════════════════════════════════════
 * deleteUser
 * Removes the specified user from the hash table chain,
 * then triggers full Module H cleanup across all modules.
 ══════════════════════════════════════════════════════════════ */
void deleteUser() {
    if (currentUser == nullptr) {
        cout << "Error: Please login first." << endl;
        return;
    }

    string uname;
    cout << "\n--- Delete Account ---" << endl;
    cout << "Enter username to delete: "; cin >> uname;

    int    index = hashFunction(uname);
    User*  curr  = hashTable[index];
    User*  prev  = nullptr;

    // search the chain for this user
    while (curr != nullptr && curr->userName != uname) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == nullptr) {
        cout << "Error: User '" << uname << "' not found." << endl;
        return;
    }

    // unlink from hash chain
    if (prev == nullptr)
        hashTable[index] = curr->next;   // was the head
    else
        prev->next = curr->next;

    // MODULE H: trigger full cleanup across all modules
    cleanupDeletedUser(curr);

    // if the deleted user was logged in, log them out
    if (currentUser != nullptr && currentUser->userName == uname) {
        currentUser = nullptr;
        cout << "You deleted your own account. Logged out." << endl;
    }

    delete curr;
    cout << "\n✓ Account '" << uname << "' deleted. All data cleaned up." << endl;
}

/* ══════════════════════════════════════════════════════════════
 * displayAllUsers
 * Iterates through all 100 buckets and prints every user found.
 * Shows bucket index and chain position to illustrate hashing.
 ══════════════════════════════════════════════════════════════ */
void displayAllUsers() {
    cout << "\n--- All Registered Users ---" << endl;
    int totalUsers = 0;

    for (int i = 0; i < TABLE_SIZE; i++) {
        User* curr = hashTable[i];
        if (curr != nullptr) {
            cout << "Bucket[" << i << "]: ";
            while (curr != nullptr) {
                cout << curr->userName;
                if (curr->next != nullptr) cout << " -> ";
                curr = curr->next;
                totalUsers++;
            }
            cout << endl;
        }
    }

    if (totalUsers == 0)
        cout << "No users registered yet." << endl;
    else
        cout << "\nTotal users: " << totalUsers << endl;
}

/* ══════════════════════════════════════════════════════════════
 * userManagementMenu
 * Displays the User Management submenu in a loop.
 * Handles all user input and calls appropriate functions.
 ══════════════════════════════════════════════════════════════ */
void userManagementMenu() {
    int choice;
    do {
        cout << "\n========= USER MANAGEMENT =========" << endl;
        if (currentUser)
            cout << "Logged in as: " << currentUser->userName << endl;
        else
            cout << "Not logged in" << endl;
        cout << "-----------------------------------" << endl;
        cout << "1. Register User" << endl;
        cout << "2. Login"         << endl;
        cout << "3. Logout"        << endl;
        cout << "4. Delete User"   << endl;
        cout << "5. Search User"   << endl;
        cout << "6. Display All"   << endl;
        cout << "7. Back"          << endl;
        cout << "-----------------------------------" << endl;
        cout << "Choice: "; cin >> choice;

        switch (choice) {
            case 1: registerUser();    break;
            case 2: loginUser();       break;
            case 3: logoutUser();      break;
            case 4: deleteUser();      break;
            case 5: {
                string name;
                cout << "Enter username to search: "; cin >> name;
                User* u = hashSearch(name);
                if (u) {
                    cout << "\nName      : " << u->userName   << endl;
                    cout << "Email     : " << u->email       << endl;
                    cout << "Bio       : " << u->bio         << endl;
                    cout << "Last seen : " << u->lastActive  << endl;
                } else {
                    cout << "User '" << name << "' not found." << endl;
                }
                break;
            }
            case 6: displayAllUsers(); break;
            case 7: cout << "Returning to main menu..." << endl; break;
            default: cout << "Invalid choice." << endl;
        }
    } while (choice != 7);
}
