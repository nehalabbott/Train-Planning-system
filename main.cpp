#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // Auto-converts C++ vectors/strings to Python lists/strings
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <climits>

namespace py = pybind11;
using namespace std;

struct Edge {
    string destination;
    int distance;
    int time;
    string line;
};

struct RouteResult {
    vector<string> path;
    int distance;
    int time;
    int fare;
};

class MetroSystem {
private:
    unordered_map<string, vector<Edge>> graph;

    int calculateFare(int distance) {
        if(distance <= 5) return 10;
        if(distance <= 15) return 20;
        if(distance <= 30) return 30;
        return 40;
    }

public:
    // Reads your CSV directly into C++ memory
    void loadConnections(const string& filepath) {
        ifstream file(filepath);
        string line;
        
        if (!file.is_open()) {
            throw runtime_error("Could not open CSV file");
        }

        while(getline(file, line)) {
            stringstream ss(line);
            string src, dest, distance, time, metroLine;

            getline(ss, src, ',');
            getline(ss, dest, ',');
            getline(ss, distance, ',');
            getline(ss, time, ',');
            getline(ss, metroLine);

            int dist_val = stoi(distance);
            int time_val = stoi(time);

            graph[src].push_back({dest, dist_val, time_val, metroLine});
            graph[dest].push_back({src, dist_val, time_val, metroLine});
        }
        file.close();
    }

    // Fetches all unique stations for the frontend dropdown
    vector<string> getStations() {
        vector<string> stations;
        for(const auto& station : graph) {
            stations.push_back(station.first);
        }
        sort(stations.begin(), stations.end());
        return stations;
    }

    // Your exact Dijkstra logic, untouched
    RouteResult findRoute(string source, string destination, string optimization) {
        bool useTime = (optimization == "fastest");
        unordered_map<string, int> cost;
        unordered_map<string, string> parent;

        for(auto &station : graph) {
            cost[station.first] = INT_MAX;
        }

        priority_queue<pair<int,string>, vector<pair<int,string>>, greater<pair<int,string>>> pq;

        cost[source] = 0;
        pq.push({0, source});

        while(!pq.empty()) {
            auto [currCost, currStation] = pq.top();
            pq.pop();

            if(currCost > cost[currStation]) continue;

            for(auto &edge : graph[currStation]) {
                int weight = useTime ? edge.time : edge.distance;

                if(currCost + weight < cost[edge.destination]) {
                    cost[edge.destination] = currCost + weight;
                    parent[edge.destination] = currStation;
                    pq.push({cost[edge.destination], edge.destination});
                }
            }
        }

        RouteResult result;
        if(cost[destination] == INT_MAX) {
            result.distance = -1;
            result.time = -1;
            result.fare = -1;
            return result;
        }

        string curr = destination;
        while(curr != source) {
            result.path.push_back(curr);
            curr = parent[curr];
        }
        result.path.push_back(source);
        reverse(result.path.begin(), result.path.end());

        int totalDistance = 0;
        int totalTime = 0;
        for(size_t i = 0; i < result.path.size() - 1; i++) {
            for(auto &edge : graph[result.path[i]]) {
                if(edge.destination == result.path[i+1]) {
                    totalDistance += edge.distance;
                    totalTime += edge.time;
                    break;
                }
            }
        }

        result.distance = totalDistance;
        result.time = totalTime;
        result.fare = calculateFare(totalDistance);
        return result;
    }
};

// --- PYBIND11 MODULE EXPORT ---
PYBIND11_MODULE(metro_routing, m) {
    // Export the RouteResult struct so Python can read its properties
    py::class_<RouteResult>(m, "RouteResult")
        .def_readonly("path", &RouteResult::path)
        .def_readonly("distance", &RouteResult::distance)
        .def_readonly("time", &RouteResult::time)
        .def_readonly("fare", &RouteResult::fare);

    // Export the main system class
    py::class_<MetroSystem>(m, "MetroSystem")
        .def(py::init<>())
        .def("load_connections", &MetroSystem::loadConnections)
        .def("get_stations", &MetroSystem::getStations)
        .def("find_route", &MetroSystem::findRoute);
}