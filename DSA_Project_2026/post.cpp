#include "post.h"
#include "user.h"

void PostManager::createPost(User* u, string text) {
    if (!u) return;

    Post* p = new Post;
    p->content = text;
    p->likes = 0;
    p->next = u->postHead;
    p->prev = 0;

    if (u->postHead)
        u->postHead->prev = p;

    u->postHead = p;
}

void PostManager::displayPosts(User* u) {
    Post* temp = u->postHead;

    while (temp) {
        cout << temp->content << " Likes: " << temp->likes << endl;
        temp = temp->next;
    }
}