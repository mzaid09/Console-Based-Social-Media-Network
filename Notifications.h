#pragma once
/*
 * File     : Notifications.h
 * Module   : E — Notifications (Queue — FIFO)
 * Purpose  : Declares all notification queue functions.
 *            Each User node has notifFront and notifRear pointers.
 *            Notifications are added at rear, processed from front.
 * Member   : Member 2
 */

#include "globals.h"

/*
 * enqueueNotification
 * Creates a new Notification node with the given message.
 * Adds it to the REAR of the target user's notification queue.
 * Called automatically by likePost() and sendMessage().
 */
void enqueueNotification(User* targetUser, string message);

/*
 * dequeueNotification
 * Removes and prints the FRONT notification from the logged-in user's queue.
 * This is "processing" the notification (marking it as read).
 */
void dequeueNotification();

/*
 * peekNotification
 * Displays the front notification WITHOUT removing it.
 */
void peekNotification();

/*
 * displayNotifications
 * Traverses the entire notification queue and prints all messages.
 * Does not remove any notifications.
 */
void displayNotifications();

/*
 * clearNotificationsOf
 * Called by Module H during account deletion.
 * Deletes all notification nodes in the user's queue.
 */
void clearNotificationsOf(User* user);

/* ─── Submenu ─── */

/*
 * notificationsMenu
 * Displays the Notifications submenu and handles input in a loop.
 */
void notificationsMenu();
