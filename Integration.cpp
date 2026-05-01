/*
 * File     : Integration.cpp
 * Module   : H — Integration (Delete Account Cascade)
 * Purpose  : Implements the cleanupDeletedUser function.
 *            This is the MOST IMPORTANT integration point.
 *            Wrong order causes dangling pointers and crashes.
 * Member   : Member 1
 */

#include "Integration.h"
#include "SocialNetwork.h"   // removeAllEdgesOf()
#include "PostManager.h"     // deleteAllPostsOf()
#include "Analytics.h"       // deleteUserAVL()
#include "Messaging.h"       // deleteAllMessagesOf()
#include "Stories.h"         // removeStoriesOf()
#include "Notifications.h"   // clearNotificationsOf()

/* ══════════════════════════════════════════════════════════════
 * cleanupDeletedUser
 * Receives the User* pointer BEFORE it is deleted from the hash table.
 * Calls each module's cleanup function in the correct order.
 * Order matters — do NOT change it.
 ══════════════════════════════════════════════════════════════ */
void cleanupDeletedUser(User* user) {
    if (user == nullptr) return;

    cout << "\n[Integration] Starting full account cleanup for: "
         << user->userName << endl;

    // ── Step 1: Graph (Module B) ──────────────────────────────
    // Remove this user from every friend's adjacency list.
    // Also frees this user's own edge list.
    cout << "[Step 1] Removing graph edges..." << endl;
    removeAllEdgesOf(user->userName);

    // ── Step 2: Posts (Module C) ─────────────────────────────
    // Delete every post in user's doubly linked list.
    // Each post is also removed from the AVL post tree inside this call.
    cout << "[Step 2] Deleting posts..." << endl;
    deleteAllPostsOf(user);

    // ── Step 3: AVL User Tree (Module F) ─────────────────────
    // Remove user from the user activity AVL tree.
    cout << "[Step 3] Removing from user AVL tree..." << endl;
    deleteUserAVL(user->userName);

    // ── Step 4: Messages (Module G) ──────────────────────────
    // Delete all conversation nodes and their message stacks.
    cout << "[Step 4] Deleting message stacks..." << endl;
    deleteAllMessagesOf(user);

    // ── Step 5: Stories (Module D) ───────────────────────────
    // Remove this user's story nodes from the circular list.
    cout << "[Step 5] Removing stories..." << endl;
    removeStoriesOf(user->userName);

    // ── Step 6: Notifications (Module E) ─────────────────────
    // Clear the user's notification queue.
    cout << "[Step 6] Clearing notification queue..." << endl;
    clearNotificationsOf(user);

    cout << "[Integration] Cleanup complete. No dangling pointers." << endl;
}
