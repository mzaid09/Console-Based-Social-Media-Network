#include "graph.h"
#include "user.h"

void Graph::addFriend(User* u1, User* u2) {
    if (!u1 || !u2) return;

    Edge* e1 = new Edge;
    e1->friendName = u2->userName;
    e1->next = u1->friendList;
    u1->friendList = e1;

    Edge* e2 = new Edge;
    e2->friendName = u1->userName;
    e2->next = u2->friendList;
    u2->friendList = e2;
}

void Graph::displayFriends(User* u) {
    Edge* temp = u->friendList;
    while (temp) {
        cout << temp->friendName << endl;
        temp = temp->next;
    }
}