#pragma once
/*
 * File     : UserManager.h
 * Module   : A — User Directory (Hash Table with Chaining)
 * Purpose  : Declares all functions for managing user accounts.
 *            The hash table is the central registry. Every other
 *            module finds users through hashSearch().
 * Member   : Member 1
 */

#include "globals.h"

/* ═══════════════════════════════════════════════════════
   HASH TABLE — array of 100 User* bucket heads
   Defined in UserManager.cpp
═══════════════════════════════════════════════════════ */
extern User* hashTable[TABLE_SIZE];

/* ─── Core Hash Table Operations ─── */

/*
 * hashFunction
 * Converts a username string into a valid array index (0–99).
 * Sums the ASCII values of all characters then takes mod TABLE_SIZE.
 */
int  hashFunction(string userName);

/*
 * registerUser
 * Prompts for username, password, email, bio.
 * Creates a new User node and inserts it into the hash table.
 * Also inserts into the AVL user tree (calls Module F).
 * Rejects duplicate usernames.
 */
void registerUser();

/*
 * loginUser
 * Prompts for username and password.
 * Searches the hash table; if found and password matches,
 * sets the global currentUser pointer.
 */
void loginUser();

/*
 * logoutUser
 * Sets currentUser = nullptr.
 * Displays "Logged out successfully."
 */
void logoutUser();

/*
 * hashSearch
 * Searches the hash table for a user by username.
 * Returns a pointer to the User node if found, nullptr otherwise.
 * Used by ALL other modules to validate users.
 */
User* hashSearch(string userName);

/*
 * deleteUser
 * Removes the user from the hash table.
 * Then calls the Module H integration function to clean up
 * all data across graph, posts, AVL trees, messages, stories.
 */
void deleteUser();

/*
 * displayAllUsers
 * Scans all 100 buckets and prints every user's basic info.
 * Shows bucket number and chain position for each user.
 */
void displayAllUsers();

/* ─── Submenu ─── */

/*
 * userManagementMenu
 * Displays the User Management submenu and handles user input
 * in a loop until the user selects Back.
 */
void userManagementMenu();
