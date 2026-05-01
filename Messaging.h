#pragma once
/*
 * File     : Messaging.h
 * Module   : G — Messaging System (Stack per Conversation)
 * Purpose  : Declares all messaging functions.
 *            Each user has a linked list of Conversation nodes.
 *            Each Conversation has a stack of Message nodes (LIFO).
 * Member   : Member 3
 */

#include "globals.h"

/*
 * findOrCreateConversation
 * Searches the current user's conversation list for a conversation
 * with 'otherUser'. If not found, creates and links a new one.
 * Returns pointer to the Conversation node.
 */
Conversation* findOrCreateConversation(User* user, string otherUser);

/*
 * sendMessage
 * Validates both sender and receiver exist in hash table.
 * Pushes a new Message onto the top of the conversation stack.
 * Calls Module E to enqueue a notification for the receiver.
 */
void sendMessage();

/*
 * viewLatestMessage
 * Peeks at the top of the conversation stack.
 * Does NOT remove the message — just displays it.
 */
void viewLatestMessage();

/*
 * popMessage
 * Removes the top message from a conversation stack.
 * Frees the memory of the removed node.
 */
void popMessage();

/*
 * displayConversation
 * Traverses the conversation stack from top to bottom
 * and prints all messages in LIFO order (newest first).
 */
void displayConversation();

/*
 * deleteAllMessagesOf
 * Called by Module H during account deletion.
 * Deletes every conversation and every message stack for a user.
 */
void deleteAllMessagesOf(User* user);

/* ─── Submenu ─── */

/*
 * messagingMenu
 * Displays the Messaging submenu and handles input in a loop.
 */
void messagingMenu();
