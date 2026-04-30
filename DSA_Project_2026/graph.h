#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
using namespace std;

struct Edge {
    string friendName;
    Edge* next;
};

class Graph {
public:
    void addFriend(struct User* u1, struct User* u2);
    void displayFriends(struct User* u);
};

#endif