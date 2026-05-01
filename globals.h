#pragma once
/*
 * File     : globals.h
 * Purpose  : Central header file — defines ALL node structs and declares the
 *            global currentUser pointer. Every .cpp file includes this file.
 *            Do NOT include any other project header before this one.
 */

#include <iostream>
#include <string>
using namespace std;

/* ═══════════════════════════════════════════════════════
   FORWARD DECLARATIONS
   (needed because structs reference each other)
═══════════════════════════════════════════════════════ */
struct User;
struct Post;
struct Edge;
struct Story;
struct Notification;
struct Message;
struct Conversation;
struct AVLNode;

struct User {
    string userName;         // unique key used for hashing
    string password;
    string email;
    string bio;
    string lastActive;

    Post*         postHead;       // head of this user's doubly linked post list
    Edge*         friendList;     // head of this user's adjacency edge list
    Notification* notifFront;     // front of this user's notification queue
    Notification* notifRear;      // rear  of this user's notification queue
    Conversation* convList;       // head of this user's conversation linked list

    User*         next;           // next node in hash table chain (chaining)

    // Constructor — sets every pointer to nullptr safely
    User() {
        postHead   = nullptr;
        friendList = nullptr;
        notifFront = nullptr;
        notifRear  = nullptr;
        convList   = nullptr;
        next       = nullptr;
    }
};

/* ═══════════════════════════════════════════════════════
   MODULE B — Edge Node (Graph adjacency list)
═══════════════════════════════════════════════════════ */
struct Edge {
    string friendUserName;   // name of the friend this edge points to
    Edge*  next;             // next edge in this user's adjacency list

    Edge() { next = nullptr; }
};

/* ═══════════════════════════════════════════════════════
   MODULE C — Post Node (Doubly Linked List)
═══════════════════════════════════════════════════════ */
struct Post {
    string postID;           // unique ID, e.g. "P001"
    string content;
    string owner;            // userName of the post creator
    int    likes;

    Post*  next;             // forward  pointer (newer → older)
    Post*  prev;             // backward pointer (older → newer)

    Post() { likes = 0; next = nullptr; prev = nullptr; }
};

/* ═══════════════════════════════════════════════════════
   MODULE D — Story Node (Circular Linked List)
═══════════════════════════════════════════════════════ */
struct Story {
    string userName;
    string storyContent;
    Story* next;             // last node's next points back to head (circular)

    Story() { next = nullptr; }
};

/* ═══════════════════════════════════════════════════════
   MODULE E — Notification Node (Queue)
═══════════════════════════════════════════════════════ */
struct Notification {
    string message;
    string targetUser;
    string timestamp;
    Notification* next;

    Notification() { next = nullptr; }
};

/* ═══════════════════════════════════════════════════════
   MODULE F — AVL Tree Node
═══════════════════════════════════════════════════════ */
struct AVLNode {
    string   key;            // userName OR postID
    int      value;          // activity count OR likes count
    AVLNode* left;
    AVLNode* right;
    int      height;

    AVLNode() { value = 0; left = nullptr; right = nullptr; height = 1; }
};

/* ═══════════════════════════════════════════════════════
   MODULE G — Message Node (Stack)
═══════════════════════════════════════════════════════ */
struct Message {
    string   fromUser;
    string   toUser;
    string   text;
    string   timestamp;
    Message* next;           // stack pointer (top → bottom)

    Message() { next = nullptr; }
};

/* ═══════════════════════════════════════════════════════
   MODULE G — Conversation Node (Linked list of stacks)
═══════════════════════════════════════════════════════ */
struct Conversation {
    string        otherUser;     // the other person in this conversation
    Message*      top;           // top of the message stack (most recent)
    Conversation* next;          // next conversation in the list

    Conversation() { top = nullptr; next = nullptr; }
};

/* ═══════════════════════════════════════════════════════
   GLOBAL CONSTANTS
═══════════════════════════════════════════════════════ */
const int TABLE_SIZE = 100;       // hash table bucket count

/* ═══════════════════════════════════════════════════════
   GLOBAL CURRENT USER
   Defined once in main.cpp — declared extern here so
   every .cpp file can read and write it.
═══════════════════════════════════════════════════════ */
extern User* currentUser;

/* ═══════════════════════════════════════════════════════
   GLOBAL STORY LIST (Module D — one shared circular list)
   Defined in stories.cpp
═══════════════════════════════════════════════════════ */
extern Story* storyHead;
extern Story* storyTail;
extern int    storyCount;
