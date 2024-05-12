#include <iostream>
#include <string>
#include "Graph.h"

int main() {
    Graph graph;
    std::string filename = "social_network.csv";
    graph.readData(filename);

    int choice;
    do {
        std::cout << "Menu:\n"
                     "1. Display the social network\n"
                     "2. Suggest friends\n"
                     "3. Calculate degree centrality\n"
                     "4. Calculate clustering coefficient\n"
                     "5. Detect communities using Girvan-Newman algorithm\n"
                     "6. Exit\n"
                     "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(); // Ignore newline character

        switch (choice) {
            case 1: {
                std::vector<std::pair<int, Person>> network = graph.getGraph();
                for (const auto& pair : network) {
                    std::cout << "ID: " << pair.first << ", Name: " << pair.second.getName() << ", Age: "<< pair.second.getAge() 
                    << ", Gender: "<<pair.second.getGender()<< ", Occupation: "<<pair.second.getOccupation()<< ", Friends: ";
                    const std::vector<int>& friends = pair.second.getFriends();
                    for (int friend_id : friends) {
                        std::cout << friend_id << " ";
                    }
                    std::cout << "\n";
                }
                break;
            }
            case 2: {
                int person_id, mode;
                std::cout << "Enter person ID: ";
                std::cin >> person_id;
                std::cout << "Enter mode (1.Common Friends, 2.Similar Occupation, 3.Similar Age): ";
                std::cin >> mode;
                graph.suggestFriends(person_id, mode);
                break;
            }
            case 3:
                graph.degreeCentrality();
                break;
            case 4: {
                int id;
                std::cout << "Enter person ID: ";
                std::cin >> id;
                double cc = graph.clusteringCoefficient(id);
                std::cout << "Clustering coefficient for person ID " << id << ": " << cc << "\n";
                break;
            }
            case 5: {
                int iterations;
                std::cout << "Enter number of iterations for Girvan-Newman algorithm: ";
                std::cin >> iterations;
                graph.girvanNewman(iterations);
                break;
            }
            case 6:
                std::cout << "Exiting...\nGood Bye!\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 6);

    return 0;
}
