#ifndef CITY_H
#define CITY_H

#include <string>
#include <vector>
#include <climits>

struct Node {
    int id;
    std::string name;
    int zoneID;
};

struct Edge {
    int destination;
    int weight;
};

class City {
private:
    std::vector<Node> nodes;
    std::vector<std::vector<Edge>> adjacencyList;
    int nextNodeId;

public:
    City();

    // Add a location (node) to the city
    int addLocation(const std::string& name, int zoneID);

    // Add a road (bidirectional edge) between two locations
    void addRoad(const std::string& locationA, const std::string& locationB, int distance);

    // Find node by name, returns -1 if not found
    int findNodeByName(const std::string& name) const;

    // Get node by ID
    const Node& getNode(int nodeId) const;

    // Get zone ID for a location by name
    int getZoneByName(const std::string& name) const;

    // Dijkstra's algorithm to find shortest distance between two locations
    int getShortestDistance(const std::string& from, const std::string& to) const;

    // Get all node IDs
    const std::vector<Node>& getAllNodes() const;
};

#endif // CITY_H
