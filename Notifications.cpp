/*
 * File     : Notifications.cpp
 * Module   : E — Notifications (Queue — FIFO)
 * Purpose  : Implements the FIFO notification queue.
 *            Each user has their own queue stored inside their User node
 *            via notifFront and notifRear pointers.
 * Member   : Member 2
 */

#include "Notifications.h"

/* ══════════════════════════════════════════════════════════════
 * enqueueNotification
 * Creates a Notification node and adds it to the REAR of
 * the target user's queue. Called by other modules automatically.
 ══════════════════════════════════════════════════════════════ */
void enqueueNotification(User* targetUser, string message) {
    if (targetUser == nullptr) return;

    Notification* newNotif   = new Notification();
    newNotif->message        = message;
    newNotif->targetUser     = targetUser->userName;
    newNotif->timestamp      = "12:00";   // TODO: use ctime for real timestamp
    newNotif->next           = nullptr;

    // add to rear of queue
    if (targetUser->notifRear == nullptr) {
        // queue was empty — front and rear both point to new node
        targetUser->notifFront = newNotif;
        targetUser->notifRear  = newNotif;
    } else {
        targetUser->notifRear->next = newNotif;
        targetUser->notifRear       = newNotif;
    }
}

/* ══════════════════════════════════════════════════════════════
 * dequeueNotification
 * Removes and displays the FRONT notification (oldest one).
 * Updates the front pointer to the next node.
 ══════════════════════════════════════════════════════════════ */
void dequeueNotification() {
    if (currentUser == nullptr) {
        cout << "Error: Please login first." << endl;
        return;
    }

    if (currentUser->notifFront == nullptr) {
        cout << "No notifications to process." << endl;
        return;
    }

    Notification* temp     = currentUser->notifFront;
    currentUser->notifFront = temp->next;

    // if queue is now empty, reset rear too
    if (currentUser->notifFront == nullptr)
        currentUser->notifRear = nullptr;

    cout << "\n✓ Processed: \"" << temp->message
         << "\" [" << temp->timestamp << "]" << endl;

    delete temp;
}

/* ══════════════════════════════════════════════════════════════
 * peekNotification
 * Shows the front notification without removing it.
 ══════════════════════════════════════════════════════════════ */
void peekNotification() {
    if (currentUser == nullptr) {
        cout << "Error: Please login first." << endl;
        return;
    }

    if (currentUser->notifFront == nullptr) {
        cout << "No notifications." << endl;
        return;
    }

    cout << "\nNext notification: \""
         << currentUser->notifFront->message
         << "\" [" << currentUser->notifFront->timestamp << "]" << endl;
}

/* ══════════════════════════════════════════════════════════════
 * displayNotifications
 * Traverses the queue from front to rear and prints all notifications.
 ══════════════════════════════════════════════════════════════ */
void displayNotifications() {
    if (currentUser == nullptr) {
        cout << "Error: Please login first." << endl;
        return;
    }

    cout << "\n--- Notifications for " << currentUser->userName << " ---" << endl;
    Notification* curr = currentUser->notifFront;
    int index = 1;

    if (curr == nullptr) {
        cout << "No notifications." << endl;
        return;
    }

    while (curr != nullptr) {
        cout << "[" << index << "] " << curr->message
             << " — " << curr->timestamp << endl;
        curr = curr->next;
        index++;
    }
}

/* ══════════════════════════════════════════════════════════════
 * clearNotificationsOf
 * Called by Module H during account deletion.
 * Deletes all notification nodes in the user's queue.
 ══════════════════════════════════════════════════════════════ */
void clearNotificationsOf(User* user) {
    Notification* curr = user->notifFront;
    while (curr != nullptr) {
        Notification* temp = curr;
        curr = curr->next;
        delete temp;
    }
    user->notifFront = nullptr;
    user->notifRear  = nullptr;
}

/* ══════════════════════════════════════════════════════════════
 * notificationsMenu
 * Displays the Notifications submenu and handles input.
 ══════════════════════════════════════════════════════════════ */
void notificationsMenu() {
    int choice;
    do {
        cout << "\n======= NOTIFICATIONS =======" << endl;
        if (currentUser)
            cout << "Logged in as: " << currentUser->userName << endl;
        else
            cout << "Not logged in" << endl;
        cout << "-----------------------------" << endl;
        cout << "1. View Notifications"        << endl;
        cout << "2. Process Notification"      << endl;
        cout << "3. Peek Next Notification"    << endl;
        cout << "4. Back"                      << endl;
        cout << "-----------------------------" << endl;
        cout << "Choice: "; cin >> choice;

        switch (choice) {
            case 1: displayNotifications(); break;
            case 2: dequeueNotification();  break;
            case 3: peekNotification();     break;
            case 4: cout << "Returning to main menu..." << endl; break;
            default: cout << "Invalid choice." << endl;
        }
    } while (choice != 4);
}
