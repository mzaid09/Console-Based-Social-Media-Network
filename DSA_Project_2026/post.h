#ifndef POST_H
#define POST_H

#include <iostream>
using namespace std;

struct Post {
    string content;
    int likes;

    Post* next;
    Post* prev;
};

class PostManager {
public:
    void createPost(struct User* u, string text);
    void displayPosts(struct User* u);
};

#endif