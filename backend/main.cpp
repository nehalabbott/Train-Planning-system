#include <bits/stdc++.h>
using namespace std;
#include <fstream>
#include <sstream>

struct Edge {
    string destination;
    int distance;
    int time;
    string line;
};

struct RouteResult
{
    vector<string> path;

    int distance;

    int time;

    int fare;
};

unordered_map <string, unordered_map<string,string>> lineMap;

unordered_map<string, vector<Edge>> graph;

void addConnection(
    string src,
    string dest,
    int distance,
    int time,
    string line
) {
    graph[src].push_back({dest, distance, time, line});
    graph[dest].push_back({src, distance, time, line});

    lineMap[src][dest] = line;
    lineMap[dest][src] = line;
}

void loadConnections()
{
    ifstream file("data/connections.csv");

    string line;

while(getline(file, line))
{
    cout << line << endl;

    stringstream ss(line);

    string src, dest, distance, time, metroLine;

    getline(ss, src, ',');
    getline(ss, dest, ',');
    getline(ss, distance, ',');
    getline(ss, time, ',');
    getline(ss, metroLine);

    cout << "Distance = " << distance << endl;
    cout << "Time = " << time << endl;

    addConnection(
        src,
        dest,
        stoi(distance),
        stoi(time),
        metroLine
    );
}

    file.close();
}

vector<string> getStations()
{
    vector<string> stations;

    for(auto &station : graph)
    {
        stations.push_back(station.first);
    }

    sort(stations.begin(), stations.end());

    return stations;
}

void displayStations()
{
    vector<string> stations = getStations();

    cout << "\nStations Available:\n\n";

    for(int i = 0; i < stations.size(); i++)
    {
        cout << i + 1
             << ". "
             << stations[i]
             << "\n";
    }
}

string selectStation()
{
    vector<string> stations = getStations();

    int choice;

    cout << "\nEnter Station Number: ";
    cin >> choice;

    if(choice < 1 || choice > stations.size())
    {
        cout << "Invalid Choice!\n";
        return "";
    }

    return stations[choice - 1];
}

int calculateFare(int distance)
{
    if(distance <= 5) return 10;
    if(distance <= 15) return 20;
    if(distance <= 30) return 30;
    return 40;
}

RouteResult findRoute(
    string source,
    string destination,
    bool useTime
)
{
    unordered_map<string,int> cost;
    unordered_map<string,string> parent;

    for(auto &station : graph)
    {
        cost[station.first] = INT_MAX;
    }

    priority_queue<
        pair<int,string>,
        vector<pair<int,string>>,
        greater<pair<int,string>>
    > pq;

    cost[source] = 0;
    pq.push({0, source});

    while(!pq.empty())
    {
        auto [currCost, currStation] = pq.top();
        pq.pop();

        if(currCost > cost[currStation])
            continue;

        for(auto &edge : graph[currStation])
        {
            int weight =
                useTime ?
                edge.time :
                edge.distance;

            if(currCost + weight <
               cost[edge.destination])
            {
                cost[edge.destination] =
                    currCost + weight;

                parent[edge.destination] =
                    currStation;

                pq.push({
                    cost[edge.destination],
                    edge.destination
                });
            }
        }
    }

if(cost[destination] == INT_MAX)
{
    RouteResult emptyResult;

    emptyResult.distance = -1;
    emptyResult.time = -1;
    emptyResult.fare = -1;

    return emptyResult;
}

    vector<string> path;

    string curr = destination;

    while(curr != source)
    {
        path.push_back(curr);
        curr = parent[curr];
    }

    path.push_back(source);

    reverse(path.begin(), path.end());

    int totalDistance = 0;
    int totalTime = 0;

    for(int i=0;i<path.size()-1;i++)
    {
        for(auto &edge : graph[path[i]])
        {
            if(edge.destination ==
               path[i+1])
            {
                totalDistance +=
                    edge.distance;

                totalTime +=
                    edge.time;

                break;
            }
        }
    }


    RouteResult result;

    result.path = path;
    result.distance = totalDistance;
    result.time = totalTime;
    result.fare = calculateFare(totalDistance);

    return result;
}

void printRoute(
    RouteResult route,
    string source,
    string destination,
    string routeType
)
{
    cout << routeType << "\n";

    cout << "Path:\n\n";

    for(int i=0;i<route.path.size();i++)
    {
        cout << route.path[i];

        if(i != route.path.size()-1)
            cout << "\n↓\n";
    }

    cout << "\n\n";

    cout << "Distance      : "
         << route.distance
         << " km\n";

    cout << "Time          : "
         << route.time
         << " min\n";

    cout << "Fare          : ₹"
         << route.fare
         << "\n";

}


void showNetworkStats()
{
    cout << "NETWORK STATISTICS\n";


    cout << "\nTotal Stations : "
         << graph.size()
         << "\n";

    int edges = 0;

    string mostConnected;
    int maxDegree = 0;

    for(auto &station : graph)
    {
        edges += station.second.size();

        if(station.second.size() > maxDegree)
        {
            maxDegree = station.second.size();
            mostConnected = station.first;
        }
    }

    cout << "Total Connections : "
         << edges / 2
         << "\n";

    cout << "Most Connected Station : "
         << mostConnected
         << "\n";

    cout << "Degree : "
         << maxDegree
         << "\n";
}

void compareRoutes(
    string source,
    string destination
)
{
    RouteResult shortest =
        findRoute(
            source,
            destination,
            false
        );

    RouteResult fastest =
        findRoute(
            source,
            destination,
            true
        );

    printRoute(
        shortest,
        source,
        destination,
        "SHORTEST DISTANCE ROUTE"
    );

    cout << "\n\n";

    printRoute(
        fastest,
        source,
        destination,
        "FASTEST ROUTE"
    );

}
void printJSON(RouteResult route)
{
    cout << "{";
    cout << "\"path\":[";
    for(int i=0;i<route.path.size();i++)
    {
        cout << "\"" << route.path[i] << "\"";
        if(i!=route.path.size()-1)
            cout << ",";
    }
    cout << "],";
    cout << "\"distance\":" << route.distance << ",";
    cout << "\"time\":" << route.time << ",";
    
    // CHANGED: Removed the trailing comma here
    cout << "\"fare\":" << route.fare; 
    
    cout << "}";
}

int main(int argc, char* argv[])
{
    // Load data first
    loadConnections();

    // IF ARGUMENTS ARE PASSED: Act as an API worker
    if (argc > 1) 
    {
        if (argc < 4) {
            cout << "{\"error\": \"Missing arguments. Expected: ./metro <source> <dest> <optimization>\"}";
            return 1;
        }

        string source = argv[1];
        string destination = argv[2];
        string optimization = argv[3]; // "fastest" or "shortest"

        // Map optimization flag to your findRoute boolean
        // Assuming true = fastest (time), false = shortest (distance)
        bool optimizeTime = (optimization == "fastest");

        // Run your existing routing logic
        RouteResult route = findRoute(source, destination, optimizeTime);

        // Print the clean, valid JSON output
        printJSON(route);
        return 0; 
    }

    // FALLBACK: Interactive CLI Menu if no arguments are provided
    while(true){
        cout << "     DELHI METRO ROUTE PLANNER\n";

        cout << "1. Show All Stations\n";
        cout << "2. Shortest Distance Route\n";
        cout << "3. Fastest Route\n";
        cout << "4. Compare All Routes\n";
        cout << "5. Network Statistics\n";
        cout << "6. Exit\n";

        cout << "\nEnter Choice: ";

        int choice;
        cin >> choice;

        if(choice == 1)
        {
            displayStations();
        }

        else if(choice == 2)
        {
            displayStations();

            cout << "\nSelect Source Station\n";
            string source = selectStation();

            cout << "\nSelect Destination Station\n";
            string destination = selectStation();

            if(source.empty() || destination.empty())
                continue;

            RouteResult route =
            findRoute(
                source,
                destination,
                false
            );

            printRoute(
                route,
                source,
                destination,
                "SHORTEST DISTANCE ROUTE"
            );
        }

        else if(choice == 3)
        {
            displayStations();

            cout << "\nSelect Source Station\n";
            string source = selectStation();

            cout << "\nSelect Destination Station\n";
            string destination = selectStation();

            if(source.empty() || destination.empty())
                continue;
            RouteResult route =
                findRoute(
                    source,
                    destination,
                    true
                );

            printRoute(
                route,
                source,
                destination,
                "FASTEST ROUTE"
            );
        }
        else if(choice == 4)
        {
            displayStations();

            cout << "\nSelect Source Station\n";
            string source = selectStation();

            cout << "\nSelect Destination Station\n";
            string destination = selectStation();

            if(source.empty() || destination.empty())
                continue;

            compareRoutes(
                source,
                destination
            );
        }
        else if(choice==5){
            showNetworkStats();
        }
        else if(choice == 6)
        {
            cout << "\nThank you for using Metro Planner!\n";
            break;
        }
        else
        {
            cout << "\nInvalid Choice!\n";
        }
    }
    return 0;
}