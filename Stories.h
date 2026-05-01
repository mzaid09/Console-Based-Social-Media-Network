#pragma once
/*
 * File     : Stories.h
 * Module   : D — Stories (Circular Linked List)
 * Purpose  : Declares story functions. One global circular linked
 *            list holds all stories from all users.
 *            tail->next always points back to head.
 * Member   : Member 2
 */

#include "globals.h"

/*
 * addStory
 * Creates a new Story node for the logged-in user.
 * Inserts into the circular linked list.
 * Updates tail->next = head to maintain circular property.
 */
void addStory();

/*
 * viewStories
 * Starts from the head of the circular list.
 * Uses a counter (storyCount) to stop after printing all stories.
 * Without the counter, the loop would run forever.
 */
void viewStories();

/*
 * removeStoriesOf
 * Called by Module H during account deletion.
 * Removes all story nodes belonging to the given username.
 */
void removeStoriesOf(string userName);

/* ─── Submenu ─── */

/*
 * storiesMenu
 * Displays the Stories submenu and handles input in a loop.
 */
void storiesMenu();
