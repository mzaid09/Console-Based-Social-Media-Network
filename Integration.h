#pragma once
/*
 * File     : Integration.h
 * Module   : H — Integration (Delete Account Cascade)
 * Purpose  : Declares the cleanupDeletedUser function which
 *            coordinates deletion across all 6 other modules.
 *            This is the glue that makes the system fully integrated.
 * Member   : Member 1
 */

#include "globals.h"

/*
 * cleanupDeletedUser
 * Called by deleteUser() in UserManager.cpp.
 * Performs full cleanup across all modules in correct order:
 *   Step 1: Graph    — remove all edges of this user
 *   Step 2: Posts    — delete all posts and remove from AVL
 *   Step 3: AVL User — remove from user activity tree
 *   Step 4: Messages — delete all conversations and stacks
 *   Step 5: Stories  — remove user's stories from circular list
 *   Step 6: Notifs   — clear notification queue
 */
void cleanupDeletedUser(User* user);
