#include "user.h"
#include "global.h"

UserDirectory::UserDirectory() {
    for (int i = 0;i < 10;i++) table[i] = 0;
}

int UserDirectory::hashFunction(string key) {
    int sum = 0;
    for (int i = 0; key[i] != '\0'; i++)
        sum += key[i];
    return sum % 10;
}

void UserDirectory::registerUser(string name, string pass) {
    int index = hashFunction(name);

    User* newUser = new User;
    newUser->userName = name;
    newUser->password = pass;
    newUser->postHead = 0;
    newUser->friendList = 0;
    newUser->next = table[index];

    table[index] = newUser;
}

User* UserDirectory::loginUser(string name, string pass) {
    User* u = findUser(name);
    if (u && u->password == pass) {
        currentUser = u;
        return u;
    }
    return 0;
}

User* UserDirectory::findUser(string name) {
    int index = hashFunction(name);
    User* temp = table[index];

    while (temp) {
        if (temp->userName == name)
            return temp;
        temp = temp->next;
    }
    return 0;
}

void UserDirectory::displayUsers() {
    for (int i = 0;i < 10;i++) {
        User* temp = table[i];
        while (temp) {
            cout << temp->userName << endl;
            temp = temp->next;
        }
    }
}