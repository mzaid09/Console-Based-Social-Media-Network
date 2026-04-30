#include <iostream>
#include "user.h"
#include "graph.h"
#include "post.h"
#include "global.h"

using namespace std;

int main() {
    UserDirectory users;
    Graph g;
    PostManager posts;

    users.registerUser("ali", "123");
    users.registerUser("ahmed", "456");

    User* u1 = users.loginUser("ali", "123");
    User* u2 = users.findUser("ahmed");

    g.addFriend(u1, u2);

    posts.createPost(u1, "Hello World!");
    posts.displayPosts(u1);

    cout << "Friends of Ali:\n";
    g.displayFriends(u1);

    return 0;
}