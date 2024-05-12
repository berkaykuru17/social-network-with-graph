#include "Graph.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <set>
#include <fstream>
#include "utils.h"


void Graph::readData(const std::string &filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    std::string line;
    std::getline(file, line); // skip header line

    while (std::getline(file, line)) {
        std::vector<std::string> data = parseCsvLine(line);
        if (data.size() != 6) {
            throw std::runtime_error("Error processing line: Invalid CSV line: " + line);
        }

        int id = std::stoi(data[0]);
        std::string name = data[1];
        int age = std::stoi(data[2]);
        std::string gender = data[3];
        std::string occupation = data[4];
        const std::vector<int> friends = parseIntList(data[5]);

        addPerson(id, Person(id, name, age, gender, occupation, friends));
    }
}


void Graph::addPerson(int id, const Person &person) {
    graph[id] = person;
    size++;
}

const Person *Graph::getPerson(int id) const {
    auto it = graph.find(id);
    if (it != graph.end()) {
        return &(it->second);
    }
    return nullptr;
}

void Graph::suggestFriends(int person_id, int mode) const {
    const Person *person = getPerson(person_id);
    if (!person) {
        std::cerr << "Person with ID " << person_id << " not found.\n";
        return;
    }

    switch (mode) {
        case 1: 
            suggestFriendsByCommonFriends(person);
            break;
        case 2: 
            suggestFriendsByOccupation(person);
            break;
        case 3: 
            suggestFriendsByAge(person);
            break;
        default:
            std::cerr << "Invalid mode. Please enter a valid mode.\n";
            break;
    }
}


void Graph::suggestFriendsByCommonFriends(const Person *person) const {
    if (!person) {
        std::cerr << "Invalid person.\n";
        return;
    }

    const std::vector<int> &person_friends = person->getFriends();
    std::set<int> suggested_friends_set;
    bool no_common_friends = true;

    for (int friend_id : person_friends) {
        const Person *friend_person = getPerson(friend_id);
        if (!friend_person) {
            continue;
        }

        const std::vector<int> &friend_friends = friend_person->getFriends();
        for (int suggested_friend_id : friend_friends) {
            // check if the suggested friend is not the person itself // is not already a friend //  and is not already suggested
            if (suggested_friend_id != person->getId() &&
    std::find(person_friends.begin(), person_friends.end(), suggested_friend_id) == person_friends.end() &&
    suggested_friends_set.find(suggested_friend_id) == suggested_friends_set.end()) {
    suggested_friends_set.insert(suggested_friend_id);
    no_common_friends = false;
}
        }
    }

    if (no_common_friends) {
        std::cout << "There is no matching friends based on common friends." << std::endl;
        return;
    }

    // convert the set to a vector for manipulation
    std::vector<int> suggested_friends(suggested_friends_set.begin(), suggested_friends_set.end());

    std::cout << "Suggestions for person " << person->getId() << " based on common friends:\n";
    for (int suggested_friend_id : suggested_friends) {
        const Person *suggested_friend = getPerson(suggested_friend_id);
        if (suggested_friend) {
            std::cout << "ID: " << suggested_friend_id << ", Name: " << suggested_friend->getName() << "\n";
        }
    }
}



void Graph::suggestFriendsByOccupation(const Person *person) const {
    if (!person) {
        std::cerr << "Invalid person.\n";
        return;
    }

    const std::string &person_occupation = person->getOccupation();
    std::set<int> suggested_friends_set;
    bool no_occupation_friends=true;

    for (const auto &pair : graph) { // Skip the same person
        const Person &other_person = pair.second;
        if (other_person.getId() == person->getId()) {
            continue; 
        }

        if (other_person.getOccupation() == person_occupation) {
           suggested_friends_set.insert(other_person.getId()); //sadece ayni occupation
            // If the occupation matches, add this person's friends to the suggestions

            /*const std::vector<int> &other_person_friends = other_person.getFriends();
            suggested_friends_set.insert(other_person_friends.begin(), other_person_friends.end());*/  // benzer occupation
            no_occupation_friends = false;
        }
    }

    if (no_occupation_friends) {
        std::cout << "There is no matching friends based on similar occupation." << std::endl;
        return;
    }

    // convert the set to a vector for manipulation
    std::vector<int> suggested_friends(suggested_friends_set.begin(), suggested_friends_set.end());

    std::cout << "Suggestions for person ID " << person->getId() << " based on similar occupation:\n";
    for (int suggested_friend_id : suggested_friends) {
        const Person *suggested_friend = getPerson(suggested_friend_id);
        if (suggested_friend) {
            std::cout << "ID: " << suggested_friend_id << ", Name: " << suggested_friend->getName() << "\n";
        }
    }
}


void Graph::suggestFriendsByAge(const Person *person) const {
    if (!person) {
        std::cerr << "Invalid person.\n";
        return;
    }

    int person_age = person->getAge();
    std::set<int> suggested_friends_set;

    const std::vector<int> &person_friends = person->getFriends();
    for (int friend_id : person_friends) {
        const Person *friend_person = getPerson(friend_id);
        if (!friend_person) {
            continue;
        }

        for (const auto &pair : graph) {
            const Person &other_person = pair.second;
            if (other_person.getId() == person->getId() || other_person.getId() == friend_id) {
                continue; // Skip the same person and the current friend
            }
            
            int age_difference = std::abs(other_person.getAge() - person_age); //abs value ||
            if (age_difference <= 2) {
                suggested_friends_set.insert(other_person.getId());
            }
        }
    }

    std::vector<int> suggested_friends(suggested_friends_set.begin(), suggested_friends_set.end());

    std::cout << "Suggestions for person ID " << person->getId() << " based on similar age:\n";
    for (int suggested_friend_id : suggested_friends) {
        const Person *suggested_friend = getPerson(suggested_friend_id);
        if (suggested_friend) {
            std::cout << "ID: " << suggested_friend_id << ", Name: " << suggested_friend->getName() << "\n";
        }
    }
}

int Graph::getSize() const {
    return size;
}

void Graph::degreeCentrality() const {
    std::vector<std::pair<int, int>> centrality_results;

    // Store the results in the vector
    for (const auto &pair : graph) {
        centrality_results.push_back({pair.first, pair.second.getFriends().size()});
    }

    // id siralama
    std::sort(centrality_results.begin(), centrality_results.end(), [](const auto &a, const auto &b) {
        return a.first < b.first;
    }
    );
    std::cout << "Degree centrality results:\n";
    for (const auto &pair : centrality_results) {
        std::cout << "Degree centrality for person ID " << pair.first << ": " << pair.second << "\n";
    }
}

double Graph::clusteringCoefficient(int id) const {
    const Person *person = getPerson(id);
    if (!person) {
        return 0;
    }

    const std::vector<int> &friends = person->getFriends();
    int total_triangles = 0;
    std::set<int> unique_friends(friends.begin(), friends.end());

    for (int friend_id : unique_friends) { //unique friends icinde friend_id ara
        const Person *friend_person = getPerson(friend_id);
        if (!friend_person) {
            continue;
        }

        std::vector<int> mutual_friends;
        std::set_intersection(friends.begin(), friends.end(), friend_person->getFriends().begin(), friend_person->getFriends().end(),
                              std::back_inserter(mutual_friends));
        total_triangles += mutual_friends.size();
    }

    int possible_triangles = unique_friends.size() * (unique_friends.size() - 1) / 2;
    if (possible_triangles == 0) {
        return 0;
    }

    return static_cast<double>(total_triangles) / possible_triangles;
}

std::vector<std::pair<int, Person>> Graph::getGraph() const {
    std::vector<std::pair<int, Person>> result;
    for (const auto &pair : graph) {
        result.push_back(pair);
    }
    std::sort(result.begin(), result.end(), [](const auto &a, const auto &b) { //id sıralama
        return a.first < b.first;
    });
    return result;
}


static double edgeWeight(const Graph &graph, int u, int v) {
    const Person *person_u = graph.getPerson(u);
    const Person *person_v = graph.getPerson(v);

    if (!person_u || !person_v) {
        std::cerr << "One or both persons not found.\n";
        return 0;
    }

    const std::vector<int> &friends_u = person_u->getFriends();
    const std::vector<int> &friends_v = person_v->getFriends();

    // calculate the number of common friends for u and v
    int common_friends_count = 0;
    for (int friend_id : friends_u) {
        if (std::find(friends_v.begin(), friends_v.end(), friend_id) != friends_v.end()) {
            common_friends_count++;
        }
    }

    // jaccard coefficient
    double jaccard_coefficient = static_cast<double>(common_friends_count) / (friends_u.size() + friends_v.size() - common_friends_count);
    return jaccard_coefficient;
}


void Graph::removeFriendship(int id1, int id2) {
    auto it1 = graph.find(id1);
    auto it2 = graph.find(id2);

    if (it1 == graph.end() || it2 == graph.end()) {
        std::cerr << "One or both persons not found.\n";
        return;
    }

    auto friends1 = it1->second.getFriends();
    auto friends2 = it2->second.getFriends();

    // remove id2 -> id1 to friend list
    friends1.erase(std::remove(friends1.begin(), friends1.end(), id2), friends1.end());
    // remove id1 -> id2 to friend list
    friends2.erase(std::remove(friends2.begin(), friends2.end(), id1), friends2.end());
    // update the friend list  id1 id2
    it1->second.setFriends(friends1);
    it2->second.setFriends(friends2);

    std::cout << "Removed friendship between " << id1 << " and " << id2 << "\n";
}



std::vector<std::vector<int>> Graph::girvanNewman(int iterations) {
    // edge map
    std::unordered_map<int, std::unordered_map<int, double>> edges;

    // Graf edgeWeight
    for (const auto &pair : graph) {
        int id = pair.first;
        const Person &person = pair.second;
        const std::vector<int> &friends = person.getFriends();
        for (int friend_id : friends) {
            edges[id][friend_id] = edgeWeight(*this, id, friend_id);
        }
    }

    // iterations
    for (int i = 0; i < iterations; ++i) {
        // biggest edgewight
        std::pair<int, int> max_edge = { -1, -1 };
        double max_weight = -1.0;

        for (const auto &[id1, friends] : edges) {
            for (const auto &[id2, weight] : friends) {
                if (weight > max_weight) {
                    max_edge = { id1, id2 };
                    max_weight = weight;
                }
            }
        }

        if (max_edge.first == -1 || max_edge.second == -1) {
            // there is no edge
            break;
        }

        removeFriendship(max_edge.first, max_edge.second);

        // new edges
        edges.clear();
        for (const auto &pair : graph) {
            int id = pair.first;
            const Person &person = pair.second;
            const std::vector<int> &friends = person.getFriends();
            for (int friend_id : friends) {
                edges[id][friend_id] = edgeWeight(*this, id, friend_id);
            }
        }
    }

    std::unordered_map<int, std::vector<int>> communities;
    for (const auto &pair : graph) {
        const Person &person = pair.second;
        int person_id = person.getId();
        const std::vector<int> &friends = person.getFriends();
        for (int friend_id : friends) {
            communities[friend_id].push_back(person_id);
        }
    }
     // return 
    std::vector<std::vector<int>> result;
    for (const auto &pair : communities) {
        result.push_back(pair.second);
    }

    return result;
}

double Graph::edgeWeight(const Graph &graph, int u, int v) {
    const Person *person_u = graph.getPerson(u);
    const Person *person_v = graph.getPerson(v);

    if (!person_u || !person_v) {
        std::cerr << "One or both persons not found.\n";
        return 0;
    }

    const std::vector<int> &friends_u = person_u->getFriends();
    const std::vector<int> &friends_v = person_v->getFriends();

    // common friends count 
    int common_friends_count = 0;
    for (int friend_id : friends_u) {
        if (std::find(friends_v.begin(), friends_v.end(), friend_id) != friends_v.end()) {
            common_friends_count++;
        }
    }

    // edgewight katsayi olarak hesapla
    double jaccard_coefficient = static_cast<double>(common_friends_count) / (friends_u.size() + friends_v.size() - common_friends_count);
    return jaccard_coefficient;
}

