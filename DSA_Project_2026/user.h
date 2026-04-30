#ifndef USER_H
#define USER_H

#include <iostream>
using namespace std;

struct Post;   // forward
struct Edge;

struct User {
    string userName;
    string password;
    string email;

    Post* postHead;
    Edge* friendList;

    User* next;
};

class UserDirectory {
private:
    User* table[10];

public:
    UserDirectory();
    int hashFunction(string key);

    void registerUser(string name, string pass);
    User* loginUser(string name, string pass);
    User* findUser(string name);
    void displayUsers();
};

#endif