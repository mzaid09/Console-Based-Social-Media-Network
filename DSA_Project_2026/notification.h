#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <iostream>
using namespace std;

struct Notification {
    string message;
    Notification* next;
};

class NotificationQueue {
private:
    Notification* front;
    Notification* rear;

public:
    NotificationQueue();
    void enqueue(string msg);
    void display();
};

#endif