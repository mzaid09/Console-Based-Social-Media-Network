#include "notification.h"

NotificationQueue::NotificationQueue() {
    front = rear = 0;
}

void NotificationQueue::enqueue(string msg) {
    Notification* n = new Notification;
    n->message = msg;
    n->next = 0;

    if (!rear)
        front = rear = n;
    else {
        rear->next = n;
        rear = n;
    }
}

void NotificationQueue::display() {
    Notification* temp = front;
    while (temp) {
        cout << temp->message << endl;
        temp = temp->next;
    }
}