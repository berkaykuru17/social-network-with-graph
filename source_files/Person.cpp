#include "Person.h"
#include <iostream>
Person::Person(int id,const std::string &name,int age,const std::string &gender,const std::string &occupation,const std::vector<int> &friends)
    : id(id), name(name), age(age), gender(gender), occupation(occupation), friends(friends) {}

Person::Person() : id(-1), name(""), age(0), gender(""), occupation(""), friends({}) {}


int Person::getId() const {
    return id;
}

std::string Person::getName() const {
    return name;
}

int Person::getAge() const {
    return age;
}

std::string Person::getGender() const {
    return gender;
}

std::string Person::getOccupation() const {
    return occupation;
}

std::vector<int> Person::getFriends() const {
     return friends;
}

void Person::addFriend(int friendId) {
    friends.push_back(friendId);
}
void Person::setFriends(const std::vector<int> &newFriends) {
    friends = newFriends;
}
