#pragma once

#include "Person.h"
#include <vector>
#include <unordered_map>

class Graph {
public:
    void readData(const std::string &filename);
    void addPerson(int id, const Person &person);
    const Person* getPerson(int id) const;
    void suggestFriends(int person_id, int mode) const;
    void suggestFriendsByCommonFriends(const Person *person) const;
    void suggestFriendsByOccupation(const Person *person) const;
    void suggestFriendsByAge(const Person *person) const;
    int getSize() const;
    void degreeCentrality() const;
    double clusteringCoefficient(int id) const;
    std::vector<std::pair<int, Person>> getGraph() const;
    std::vector<std::vector<int>> girvanNewman(int iterations);
    void removeFriendship(int id1, int id2);
private:
    std::unordered_map<int, Person> graph;
    int size;
    static double edgeWeight(const Graph &graph, int u, int v);
};
