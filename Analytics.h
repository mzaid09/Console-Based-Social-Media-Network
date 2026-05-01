#pragma once
/*
 * File     : Analytics.h
 * Module   : F — Analytics & Ranking (AVL Tree x2)
 * Purpose  : Declares all AVL tree operations.
 *            Two separate AVL trees:
 *              userAVLRoot — ranks users by activity count
 *              postAVLRoot — ranks posts by likes count
 *            AVL self-balances after every insert/delete.
 * Member   : Member 3
 */

#include "globals.h"

/* ═══════════════════════════════════════════════════════
   AVL TREE ROOTS — defined in Analytics.cpp
═══════════════════════════════════════════════════════ */
extern AVLNode* userAVLRoot;
extern AVLNode* postAVLRoot;

/* ─── Core AVL Utilities ─── */

/*
 * avlHeight
 * Returns height of a node. Returns 0 if node is nullptr.
 */
int avlHeight(AVLNode* node);

/*
 * avlGetBalance
 * Returns balance factor: height(left) - height(right).
 * Must stay between -1 and +1. Outside this range = rotate.
 */
int avlGetBalance(AVLNode* node);

/*
 * avlRotateRight
 * Performs a right rotation on the given node.
 * Used when left subtree is heavier (balance factor > 1).
 */
AVLNode* avlRotateRight(AVLNode* y);

/*
 * avlRotateLeft
 * Performs a left rotation on the given node.
 * Used when right subtree is heavier (balance factor < -1).
 */
AVLNode* avlRotateLeft(AVLNode* x);

/*
 * avlInsert
 * Inserts a key-value pair into the given AVL tree.
 * Automatically rebalances after insertion.
 * Returns the new root of the (sub)tree.
 */
AVLNode* avlInsert(AVLNode* root, string key, int value);

/*
 * avlDelete
 * Removes a node by key from the AVL tree.
 * Rebalances after deletion. Returns new root.
 */
AVLNode* avlDelete(AVLNode* root, string key);

/*
 * avlSearch
 * Searches the AVL tree for a node by key.
 * Returns pointer to the node if found, nullptr otherwise.
 */
AVLNode* avlSearch(AVLNode* root, string key);

/*
 * avlUpdateValue
 * Finds the node by key and updates its value field.
 */
void avlUpdateValue(AVLNode* root, string key, int newValue);

/*
 * avlInorder
 * Prints all nodes in sorted order (ascending by key).
 * Left → Root → Right traversal.
 */
void avlInorder(AVLNode* root);

/*
 * avlGetTopK
 * Collects nodes into an array via inorder traversal,
 * then prints the top K entries (highest values).
 */
void avlGetTopK(AVLNode* root, int k);

/*
 * avlRangeQuery
 * Prints all nodes whose value is between minVal and maxVal inclusive.
 */
void avlRangeQuery(AVLNode* root, int minVal, int maxVal);

/* ─── Module-level wrappers (called by other modules) ─── */

/*
 * insertUserAVL    — called by registerUser()
 * deleteUserAVL    — called by cleanupDeletedUser()
 * incrementUserActivity — called by createPost()
 */
void insertUserAVL(string userName, int activity);
void deleteUserAVL(string userName);
void incrementUserActivity(string userName);

/*
 * insertPostAVL    — called by createPost()
 * deletePostAVL    — called by deletePost() and deleteAllPostsOf()
 * updatePostAVL    — called by likePost()
 */
void insertPostAVL(string postID, int likes);
void deletePostAVL(string postID);
void updatePostAVL(string postID, int newLikes);

/* ─── Submenu ─── */

/*
 * analyticsMenu
 * Displays the Analytics & Ranking submenu and handles input.
 */
void analyticsMenu();
