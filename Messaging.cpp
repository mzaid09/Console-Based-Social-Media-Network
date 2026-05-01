/*
 * File     : Messaging.cpp
 * Module   : G — Messaging System (Stack per Conversation)
 * Purpose  : Implements stack-based messaging. Each conversation
 *            is a stack where push = send, pop = delete, peek = view latest.
 * Member   : Member 3
 */

#include "Messaging.h"
#include "UserManager.h"    // for hashSearch()
#include "Notifications.h"  // for enqueueNotification()

/* ══════════════════════════════════════════════════════════════
 * findOrCreateConversation
 * Searches user's conversation list for a conversation with 'otherUser'.
 * Creates a new Conversation node if not found.
 ══════════════════════════════════════════════════════════════ */
Conversation* findOrCreateConversation(User* user, string otherUser) {
    // search existing conversations
    Conversation* curr = user->convList;
    while (curr != nullptr) {
        if (curr->otherUser == otherUser)
            return curr;
        curr = curr->next;
    }

    // not found — create new conversation and insert at head
    Conversation* newConv  = new Conversation();
    newConv->otherUser     = otherUser;
    newConv->top           = nullptr;
    newConv->next          = user->convList;
    user->convList         = newConv;

    return newConv;
}

/* ══════════════════════════════════════════════════════════════
 * sendMessage
 * Validates sender and receiver, creates a Message node,
 * pushes it to the top of the conversation stack (LIFO).
 * Sends a notification to the receiver via Module E.
 ══════════════════════════════════════════════════════════════ */
void sendMessage() {
    if (currentUser == nullptr) {
        cout << "Error: Please login first." << endl;
        return;
    }

    string toUser, text;
    cout << "\n--- Send Message ---" << endl;
    cout << "To user : "; cin >> toUser;

    // verify receiver exists in hash table (Module A)
    User* receiver = hashSearch(toUser);
    if (receiver == nullptr) {
        cout << "Error: User '" << toUser << "' not found." << endl;
        return;
    }
    if (toUser == currentUser->userName) {
        cout << "Error: Cannot message yourself." << endl;
        return;
    }

    cin.ignore();
    cout << "Message : "; getline(cin, text);

    // create new message node
    Message* newMsg       = new Message();
    newMsg->fromUser      = currentUser->userName;
    newMsg->toUser        = toUser;
    newMsg->text          = text;
    newMsg->timestamp     = "12:00";    // TODO: use ctime for real timestamp

    // find/create conversation on SENDER's side and push
    Conversation* senderConv = findOrCreateConversation(currentUser, toUser);
    newMsg->next             = senderConv->top;   // push to top of stack
    senderConv->top          = newMsg;

    // also push a copy to RECEIVER's side
    Message* msgCopy      = new Message();
    msgCopy->fromUser     = newMsg->fromUser;
    msgCopy->toUser       = newMsg->toUser;
    msgCopy->text         = newMsg->text;
    msgCopy->timestamp    = newMsg->timestamp;

    Conversation* recvConv = findOrCreateConversation(receiver, currentUser->userName);
    msgCopy->next          = recvConv->top;
    recvConv->top          = msgCopy;

    // MODULE E: enqueue notification for receiver
    string notifMsg = "New message from " + currentUser->userName;
    enqueueNotification(receiver, notifMsg);

    cout << "\n✓ Message sent to " << toUser << "!" << endl;
}

/* ══════════════════════════════════════════════════════════════
 * viewLatestMessage
 * Peeks at the top of the conversation stack.
 * Does NOT remove the message — peek operation only.
 ══════════════════════════════════════════════════════════════ */
void viewLatestMessage() {
    if (currentUser == nullptr) {
        cout << "Error: Please login first." << endl;
        return;
    }

    string otherUser;
    cout << "Conversation with: "; cin >> otherUser;

    Conversation* conv = currentUser->convList;
    while (conv != nullptr) {
        if (conv->otherUser == otherUser) break;
        conv = conv->next;
    }

    if (conv == nullptr || conv->top == nullptr) {
        cout << "No messages with '" << otherUser << "'." << endl;
        return;
    }

    // peek — just read top without removing
    cout << "\nLatest message:" << endl;
    cout << "  " << conv->top->fromUser << " [" << conv->top->timestamp
         << "]: \"" << conv->top->text << "\"" << endl;
}

/* ══════════════════════════════════════════════════════════════
 * popMessage
 * Removes the top message from a conversation stack.
 ══════════════════════════════════════════════════════════════ */
void popMessage() {
    if (currentUser == nullptr) {
        cout << "Error: Please login first." << endl;
        return;
    }

    string otherUser;
    cout << "Remove top message from conversation with: "; cin >> otherUser;

    Conversation* conv = currentUser->convList;
    while (conv != nullptr) {
        if (conv->otherUser == otherUser) break;
        conv = conv->next;
    }

    if (conv == nullptr || conv->top == nullptr) {
        cout << "No messages with '" << otherUser << "'." << endl;
        return;
    }

    // pop — remove top node
    Message* temp = conv->top;
    conv->top     = temp->next;

    cout << "\n✓ Removed: \"" << temp->text << "\"" << endl;
    delete temp;
}

/* ══════════════════════════════════════════════════════════════
 * displayConversation
 * Traverses the message stack from top to bottom (LIFO order)
 * and prints each message. Newest first.
 ══════════════════════════════════════════════════════════════ */
void displayConversation() {
    if (currentUser == nullptr) {
        cout << "Error: Please login first." << endl;
        return;
    }

    string otherUser;
    cout << "View conversation with: "; cin >> otherUser;

    Conversation* conv = currentUser->convList;
    while (conv != nullptr) {
        if (conv->otherUser == otherUser) break;
        conv = conv->next;
    }

    if (conv == nullptr || conv->top == nullptr) {
        cout << "No conversation with '" << otherUser << "'." << endl;
        return;
    }

    cout << "\n--- Conversation: " << currentUser->userName
         << " <-> " << otherUser << " ---" << endl;

    Message* curr = conv->top;
    while (curr != nullptr) {
        cout << "  " << curr->fromUser << " [" << curr->timestamp
             << "]: \"" << curr->text << "\"" << endl;
        curr = curr->next;
    }
}

/* ══════════════════════════════════════════════════════════════
 * deleteAllMessagesOf
 * Called by Module H during account deletion.
 * Deletes all conversation nodes and every message in each stack.
 ══════════════════════════════════════════════════════════════ */
void deleteAllMessagesOf(User* user) {
    Conversation* conv = user->convList;
    while (conv != nullptr) {
        // delete all messages in this conversation's stack
        Message* msg = conv->top;
        while (msg != nullptr) {
            Message* tempMsg = msg;
            msg = msg->next;
            delete tempMsg;
        }
        Conversation* tempConv = conv;
        conv = conv->next;
        delete tempConv;
    }
    user->convList = nullptr;
}

/* ══════════════════════════════════════════════════════════════
 * messagingMenu — Displays the Messaging submenu
 ══════════════════════════════════════════════════════════════ */
void messagingMenu() {
    int choice;
    do {
        cout << "\n======= MESSAGING =======" << endl;
        if (currentUser)
            cout << "Logged in as: " << currentUser->userName << endl;
        else
            cout << "Not logged in" << endl;
        cout << "-------------------------" << endl;
        cout << "1. Send Message"         << endl;
        cout << "2. View Latest Message"  << endl;
        cout << "3. Pop Message"          << endl;
        cout << "4. View Conversation"    << endl;
        cout << "5. Back"                 << endl;
        cout << "-------------------------" << endl;
        cout << "Choice: "; cin >> choice;

        switch (choice) {
            case 1: sendMessage();        break;
            case 2: viewLatestMessage();  break;
            case 3: popMessage();         break;
            case 4: displayConversation(); break;
            case 5: cout << "Returning to main menu..." << endl; break;
            default: cout << "Invalid choice." << endl;
        }
    } while (choice != 5);
}
