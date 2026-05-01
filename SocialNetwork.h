#pragma once
/*
 * File     : SocialNetwork.h
 * Module   : B — Social Network (Graph with Adjacency Lists)
 * Purpose  : Declares all friendship/graph functions.
 *            Each user node stores an Edge* friendList.
 *            Friendships are undirected — adding A→B also adds B→A.
 * Member   : Member 1
 */

#include "globals.h"

/*
 * addFriend
 * Verifies both users exist in hash table.
 * Adds an edge in BOTH directions (undirected graph).
 * Checks for duplicate friendship before adding.
 */
void addFriend(string user1, string user2);

/*
 * removeFriend
 * Removes the edge between user1 and user2 in both directions.
 * Does nothing if friendship does not exist.
 */
void removeFriend(string user1, string user2);

/*
 * displayFriends
 * Traverses the given user's Edge* friendList and prints each friend.
 */
void displayFriends(string userName);

/*
 * BFS
 * Breadth-First Search starting from startUser.
 * Uses a manually implemented queue (array-based for simplicity).
 * Prints users level by level.
 */
void BFS(string startUser);

/*
 * DFS
 * Depth-First Search starting from startUser.
 * Uses recursion (implicit call stack).
 * Prints users in depth-first order.
 * visited[] array tracks already-visited users.
 */
void DFS(string startUser);

/*
 * removeAllEdgesOf
 * Helper called by Module H (Integration) during account deletion.
 * Removes this user from every friend's adjacency list.
 */
void removeAllEdgesOf(string userName);

/* ─── Submenu ─── */

/*
 * socialNetworkMenu
 * Displays the Social Network submenu and handles input in a loop.
 */
void socialNetworkMenu();
