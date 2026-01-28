#include "City.h"
#include <algorithm>
#include <iostream>

City::City() : nextNodeId(0) {}

int City::addLocation(const std::string& name, int zoneID) {
    Node newNode;
    newNode.id = nextNodeId;
    newNode.name = name;
    newNode.zoneID = zoneID;

    nodes.push_back(newNode);
    adjacencyList.resize(nextNodeId + 1);

    return nextNodeId++;
}

void City::addRoad(const std::string& locationA, const std::string& locationB, int distance) {
    int idA = findNodeByName(locationA);
    int idB = findNodeByName(locationB);

    if (idA == -1 || idB == -1) {
        std::cerr << "Error: One or both locations not found\n";
        return;
    }

    // Add bidirectional edges
    Edge edgeA;
    edgeA.destination = idB;
    edgeA.weight = distance;
    adjacencyList[idA].push_back(edgeA);

    Edge edgeB;
    edgeB.destination = idA;
    edgeB.weight = distance;
    adjacencyList[idB].push_back(edgeB);
}

int City::findNodeByName(const std::string& name) const {
    for (size_t i = 0; i < nodes.size(); ++i) {
        if (nodes[i].name == name) {
            return nodes[i].id;
        }
    }
    return -1;
}

const Node& City::getNode(int nodeId) const {
    return nodes[nodeId];
}

int City::getZoneByName(const std::string& name) const {
    int nodeId = findNodeByName(name);
    if (nodeId != -1) {
        return nodes[nodeId].zoneID;
    }
    return -1;
}

int City::getShortestDistance(const std::string& from, const std::string& to) const {
    int startId = findNodeByName(from);
    int endId = findNodeByName(to);

    if (startId == -1 || endId == -1) {
        return -1;
    }

    // Dijkstra's Algorithm without std::map
    std::vector<int> dist(nodes.size(), INT_MAX);
    std::vector<bool> visited(nodes.size(), false);

    dist[startId] = 0;

    // Process all nodes
    for (size_t count = 0; count < nodes.size(); ++count) {
        // Find unvisited node with minimum distance
        int minDist = INT_MAX;
        int minNode = -1;

        for (size_t i = 0; i < nodes.size(); ++i) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                minNode = i;
            }
        }

        if (minNode == -1 || minDist == INT_MAX) {
            break;
        }

        visited[minNode] = true;

        // Update distances of neighbors
        for (const Edge& edge : adjacencyList[minNode]) {
            int neighbor = edge.destination;
            int weight = edge.weight;

            if (!visited[neighbor] && dist[minNode] + weight < dist[neighbor]) {
                dist[neighbor] = dist[minNode] + weight;
            }
        }
    }

    return dist[endId] == INT_MAX ? -1 : dist[endId];
}

const std::vector<Node>& City::getAllNodes() const {
    return nodes;
}
