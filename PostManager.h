#pragma once
/*
 * File     : PostManager.h
 * Module   : C — Posts & Feed (Doubly Linked List)
 * Purpose  : Declares all post management functions.
 *            Each User node has a Post* postHead pointing to
 *            their personal doubly linked list of posts.
 * Member   : Member 2
 */

#include "globals.h"

/*
 * createPost
 * Creates a new Post node for the logged-in user.
 * Inserts at the HEAD of the user's doubly linked list.
 * Calls Module F to insert into AVL post tree (likes=0).
 * Calls Module F to increment user activity count.
 */
void createPost();

/*
 * deletePost
 * Prompts for a post ID.
 * Finds and removes the post from the user's doubly linked list.
 * Calls Module F to remove from AVL post tree.
 */
void deletePost();

/*
 * displayMyPosts
 * Traverses the logged-in user's post list and prints all posts.
 */
void displayMyPosts();

/*
 * navigateFeed
 * Lets user scroll through a global feed using N (next) and P (prev).
 * The feed is built by collecting posts from all users.
 * Uses the prev/next pointers of the doubly linked list.
 */
void navigateFeed();

/*
 * likePost
 * Prompts for a post ID and owner username.
 * Increments post.likes by 1.
 * Calls Module F updatePostAVL() to update the AVL tree.
 * Calls Module E enqueueNotification() to notify the post owner.
 */
void likePost();

/*
 * deleteAllPostsOf
 * Helper called by Integration (Module H) during account deletion.
 * Deletes every post in the user's list and removes from AVL.
 */
void deleteAllPostsOf(User* user);

/*
 * generatePostID
 * Returns the next unique post ID as a string, e.g. "P001".
 */
string generatePostID();

/* ─── Submenu ─── */

/*
 * postsMenu
 * Displays the Posts & Feed submenu and handles input in a loop.
 */
void postsMenu();
