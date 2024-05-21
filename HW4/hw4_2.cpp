#include <bits/stdc++.h>
using namespace std;

struct Edge {
    long long to, distance, traffic_capacity;
};

struct Order {
    long long id, src, ts, total_distance, dst;
    vector<long long> path_to_pickup;
    vector<long long> path_to_client;
    bool picked_up = false;
};

class Graph {
private:
    long long V;
    vector<vector<Edge>> adjList;
    map<long long, long long> availableDrivers;
    unordered_map<long long, Order> orders;
    set<long long> waiting;

public:
    Graph(long long V) : V(V) {
        adjList.resize(V + 1);
    }

    void addEdge(long long s, long long d, long long dis, long long t) {
        adjList[s].push_back({d, dis, t});
        adjList[d].push_back({s, dis, t});
    }

    void placeDriver(long long v, long long count) {
        availableDrivers[v] += count;
    }

    void updateTrafficCapacity(long long u, long long v, long long ts, bool increase) {
        auto updateCapacity = [&](long long from, long long to, long long value) {
            for (auto& edge : adjList[from]) {
                if (edge.to == to) {
                    edge.traffic_capacity += (increase ? value : -value);
                    if (edge.traffic_capacity < 0) edge.traffic_capacity = 0;  // Ensure non-negative capacity
                    break;
                }
            }
        };

        updateCapacity(u, v, ts);
        updateCapacity(v, u, ts);  // Since the graph is undirected
    }

    string processOrder(long long ID, long long src, long long ts) {
        vector<long long> dist(V + 1, LLONG_MAX);
        vector<long long> prev(V + 1, -1);
        vector<bool> visited(V + 1, false);
        dist[src] = 0;
        priority_queue<pair<long long, long long>, vector<pair<long long, long long>>, greater<>> pq;
        pq.push({0, src});

        while (!pq.empty()) {
            long long u = pq.top().second;
            pq.pop();
            if (visited[u]) continue;
            visited[u] = true;

            for (const auto& edge : adjList[u]) {
                if (edge.traffic_capacity >= ts && dist[edge.to] > dist[u] + edge.distance) {
                    dist[edge.to] = dist[u] + edge.distance;
                    prev[edge.to] = u;
                    pq.push({dist[edge.to], edge.to});
                }
            }
        }

        long long nearestDriverLocation = -1;
        long long minDistance = LLONG_MAX;
        for (const auto& loc : availableDrivers) {
            if (loc.second > 0 && dist[loc.first] < minDistance) {
                nearestDriverLocation = loc.first;
                minDistance = dist[loc.first];
            }
        }

        if (nearestDriverLocation == -1 || minDistance == LLONG_MAX) {
            return "Just walk. T-T";
        } else {
            vector<long long> path;
            if (dist[nearestDriverLocation] <= minDistance) {
                for (long long u = nearestDriverLocation; u != -1; u = prev[u]) {
                    path.push_back(u);
                }
                reverse(path.begin(), path.end());
            } else {
                path = {src, nearestDriverLocation};
            }

            availableDrivers[nearestDriverLocation]--;
            orders[ID] = {ID, src, ts, minDistance, -1, path, {}, false};

            for (size_t i = 0; i < path.size() - 1; i++) {
                updateTrafficCapacity(path[i], path[i + 1], ts, false);
            }

            return "Order " + to_string(ID) + " from: " + to_string(nearestDriverLocation);
        }
    }

    string processDrop(long long ID, long long dst, bool checkk) {
        if (orders.find(ID) == orders.end()) {
            return "";
        }

        Order& order = orders[ID];
        order.dst = dst;
        
        if (checkk == 0) {
            for (size_t i = 0; i < order.path_to_pickup.size() - 1; i++) {
                updateTrafficCapacity(order.path_to_pickup[i], order.path_to_pickup[i + 1], order.ts, true);
            }
        }

        vector<long long> dist(V + 1, LLONG_MAX);
        vector<long long> prev(V + 1, -1);
        vector<bool> visited(V + 1, false);
        dist[order.src] = 0;
        priority_queue<pair<long long, long long>, vector<pair<long long, long long>>, greater<>> pq;
        pq.push({0, order.src});

        while (!pq.empty()) {
            long long u = pq.top().second;
            pq.pop();

            if (visited[u]) continue;
            visited[u] = true;

            for (const auto& edge : adjList[u]) {
                if (edge.traffic_capacity >= order.ts && dist[edge.to] > dist[u] + edge.distance) {
                    dist[edge.to] = dist[u] + edge.distance;
                    prev[edge.to] = u;
                    pq.push({dist[edge.to], edge.to});
                }
            }
        }

        long long directDistance = LLONG_MAX;
        vector<long long> directPath;
        for (const auto& edge : adjList[order.src]) {
            if (edge.to == dst && edge.traffic_capacity >= order.ts) {
                directDistance = edge.distance;
                directPath = {order.src, dst};
                break;
            }
        }

        if (dist[dst] == LLONG_MAX && directDistance == LLONG_MAX) {
            waiting.insert(ID);
            return "No Way Home";
        } else {
            vector<long long> path;
            if (directDistance <= dist[dst]) {
                path = directPath;
            } else {
                for (long long u = dst; u != -1; u = prev[u]) {
                    path.push_back(u);
                }
                reverse(path.begin(), path.end());
            }

            order.total_distance += min(directDistance, dist[dst]);
            order.path_to_client = path;
            for (size_t i = 0; i < path.size() - 1; i++) {
                updateTrafficCapacity(path[i], path[i + 1], order.ts, false);
            }

            order.picked_up = true;
            return "Order " + to_string(ID) + " distance: " + to_string(order.total_distance);
        }
    }

    void processComplete(long long ID) {
        if (orders.find(ID) == orders.end() || !orders[ID].picked_up) {
            return;
        }

        Order& order = orders[ID];
        for (size_t i = 0; i < order.path_to_client.size() - 1; i++) {
            updateTrafficCapacity(order.path_to_client[i], order.path_to_client[i + 1], order.ts, true);
        }
        availableDrivers[order.dst]++;
        orders.erase(ID);
        handleWaitingOrders();
    }

    void handleWaitingOrders() {
        vector<long long> toRemove;
        for (long long id : waiting) {
            if (processOrderWithExistingData(id)) {
                toRemove.push_back(id);
            }
        }
        for (long long id : toRemove) {
            waiting.erase(id);
        }
    }

    bool processOrderWithExistingData(long long ID) {
        if (orders.find(ID) == orders.end()) return false;

        Order& order = orders[ID];
        if (!canProcessOrder(order.src, order.ts)) {
            return false;
        }
        string result = processDrop(ID, order.dst, 1);
        if (result != "No Way Home") {
            cout << result << endl;
            return true;
        }
        return false;
    }

    bool canProcessOrder(long long src, long long ts) {
        vector<long long> dist(V + 1, LLONG_MAX);
        dist[src] = 0;
        priority_queue<pair<long long, long long>, vector<pair<long long, long long>>, greater<>> pq;
        pq.push({0, src});

        while (!pq.empty()) {
            long long u = pq.top().second;
            pq.pop();
            for (const auto& edge : adjList[u]) {
                if (edge.traffic_capacity >= ts && dist[edge.to] > dist[u] + edge.distance) {
                    dist[edge.to] = dist[u] + edge.distance;
                    pq.push({dist[edge.to], edge.to});
                }
            }
        }

        for (const auto& loc : availableDrivers) {
            if (loc.second > 0 && dist[loc.first] < LLONG_MAX) {
                return true;
            }
        }
        return false;
    }
};

int main() {
    long long V, E, D;
    cin >> V >> E >> D;
    Graph g(V);

    for (long long i = 0; i < D; i++) {
        long long v, c;
        string s;
        cin >> s >> v >> c;
        g.placeDriver(v, c);
    }

    for (long long i = 0; i < E; i++) {
        string ss;
        long long s, d, dis, t;
        cin >> ss >> s >> d >> dis >> t;
        g.addEdge(s, d, dis, t);
    }

    long long C;
    cin >> C;
    for (long long i = 0; i < C; i++) {
        string command;
        cin >> command;
        if (command == "Order") {
            long long ID, src, ts;
            cin >> ID >> src >> ts;
            cout << g.processOrder(ID, src, ts) << endl;
        } else if (command == "Drop") {
            long long ID, dst;
            cin >> ID >> dst;
            cout << g.processDrop(ID, dst, 0) << endl;
        } else if (command == "Complete") {
            long long ID;
            cin >> ID;
            g.processComplete(ID);
        }
    }

    return 0;
}
