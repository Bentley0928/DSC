#include <bits/stdc++.h>
using namespace std;

// Edge structure
struct Edge {
    long long to, distance, traffic_capacity;
};

// Order structure
struct Order {
    long long id, src, ts, total_distance, dst;
    vector<long long> path_to_pickup;
    vector<long long> path_to_client;
    bool picked_up;
};

// Graph class
class Graph {
private:
    long long V;
    vector<vector<Edge>> adjList;
    map<long long, long long> availableDrivers; // Available drivers at each location
    unordered_map<long long, Order> orders; // Store orders
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

    void printGraph() {
        cout << "Graph representation:\n";
        for (long long u = 1; u <= V; ++u) {
            cout << "Vertex " << u << ":\n";
            for (const auto& edge : adjList[u]) {
                cout << "  -> " << edge.to << " (distance: " << edge.distance << ", traffic_capacity: " << edge.traffic_capacity << ")\n";
            }
        }
    }

    void updateTrafficCapacity(long long u, long long v, long long ts, bool increase) {
        auto update = [=](long long u, long long v) {
            for (Edge& edge : adjList[u]) {
                if (edge.to == v) {
                    if (increase) {
                        edge.traffic_capacity += ts;
                    } else {
                        edge.traffic_capacity -= ts;
                    }
                    break;
                }
            }
        };
        update(u, v);
        update(v, u);
    }

    string processOrder(long long ID, long long src, long long ts) {
        // Initialize distance, predecessors, and priority queue for Dijkstra's algorithm
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

            // Update adjacent vertices
            vector<Edge> neighbors = adjList[u];
            sort(neighbors.begin(), neighbors.end(), [](const Edge& a, const Edge& b) {
                return a.to < b.to;
            });

            for (const auto& edge : neighbors) {
                if (edge.traffic_capacity >= ts && dist[edge.to] > dist[u] + edge.distance) {
                    dist[edge.to] = dist[u] + edge.distance;
                    prev[edge.to] = u;
                    pq.push({dist[edge.to], edge.to});
                }
            }
        }

        // Find the nearest available driver using Dijkstra's path
        long long nearestDriverLocation = -1;
        long long minDistance = LLONG_MAX;
        for (const auto& loc : availableDrivers) {
            if (loc.second > 0 && dist[loc.first] < minDistance) {
                nearestDriverLocation = loc.first;
                minDistance = dist[loc.first];
            }
        }

        // Check direct paths if they have drivers and the path is shorter or equal
        for (const auto& edge : adjList[src]) {
            if (edge.traffic_capacity >= ts && edge.distance <= minDistance && availableDrivers[edge.to] > 0) {
                nearestDriverLocation = edge.to;
                minDistance = edge.distance;
            }
        }

        if (nearestDriverLocation == -1 || minDistance == LLONG_MAX) {
            return "Just walk. T-T";
        } else {
            vector<long long> path;
            if (dist[nearestDriverLocation] <= minDistance) {
                // Use Dijkstra path
                for (long long u = nearestDriverLocation; u != -1; u = prev[u]) {
                    path.push_back(u);
                }
                reverse(path.begin(), path.end());
            } else {
                // Use direct path
                path = {src, nearestDriverLocation};
            }

            // Decrease the available driver count at the found location
            availableDrivers[nearestDriverLocation]--;

            // Store order
            orders[ID] = {ID, src, ts, static_cast<long long>(minDistance), -1, path, {}, false};

            // Decrease the traffic capacity along the path
            long long siz = path.size();
            for (size_t i = 0; i < siz - 1; i++) {
                long long u = path[i];
                long long v = path[i + 1];
                updateTrafficCapacity(u, v, ts, false);
            }

            return "Order " + to_string(ID) + " from: " + to_string(nearestDriverLocation);
        }
    }

    void printOrderPath(long long ID) {
        if (orders.find(ID) == orders.end()) {
            cout << "Order ID not found\n";
            return;
        }

        Order& order = orders[ID];
        cout << "Path for Order " << ID << ": ";
        for (long long i = 0; i < order.path_to_pickup.size(); ++i) {
            if (i > 0) cout << " -> ";
            cout << order.path_to_pickup[i];
        }
        cout << endl;
    }

string processDrop(long long ID, long long dst, bool checkk) {
    if (orders.find(ID) == orders.end()) {
        return "";
    }

    Order& order = orders[ID];
    order.dst = dst;
    
    // 更新到達取餐點的路徑流量容量
    if(checkk == 0) {
        for (size_t i = 0; i < order.path_to_pickup.size() - 1; i++) {
            long long u = order.path_to_pickup[i];
            long long v = order.path_to_pickup[i + 1];
            updateTrafficCapacity(u, v, order.ts, true);
        }
    }

    // 初始化距離、前驅節點和優先佇列，用於 Dijkstra 演算法
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

        // 更新相鄰頂點
        vector<Edge> neighbors = adjList[u];
        sort(neighbors.begin(), neighbors.end(), [](const Edge& a, const Edge& b) {
            return a.to < b.to;
        });

        for (const auto& edge : neighbors) {
            if (edge.traffic_capacity >= order.ts && dist[edge.to] > dist[u] + edge.distance) {
                dist[edge.to] = dist[u] + edge.distance;
                prev[edge.to] = u;
                pq.push({dist[edge.to], edge.to});
            }
        }
    }

    // 檢查直接路徑是否可達
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
        waiting.insert(ID); // 訂單無法送達，加入等待集合
        return "No Way Home";
    } else {
        // 找到可達路徑
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

        // 更新路徑流量容量
        for (size_t i = 0; i < path.size() - 1; i++) {
            long long u = path[i];
            long long v = path[i + 1];
            updateTrafficCapacity(u, v, order.ts, false);
        }

        order.picked_up = true;
        return "Order " + to_string(ID) + " distance: " + to_string(order.total_distance);
    }
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
        string result = processDrop(ID, order.dst, 0);
        if (result != "No Way Home") {
            cout << result << endl;
            return true;
        }
        return false;
    }

    // 檢查特定訂單是否可以處理
    bool canProcessOrder(long long src, long long ts) {
        priority_queue<pair<long long, long long>, vector<pair<long long, long long>>, greater<>> pq;
        vector<long long> dist(V + 1, LLONG_MAX);
        dist[src] = 0;
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

    void processComplete(long long ID) {
        if (orders.find(ID) == orders.end() || !orders[ID].picked_up) {
            return;
        }

        Order& order = orders[ID];
        for (size_t i = 0; i < order.path_to_client.size() - 1; i++) {
            long long u = order.path_to_client[i];
            long long v = order.path_to_client[i + 1];
            updateTrafficCapacity(u, v, order.ts, true);
        }
        availableDrivers[order.dst]++;
        orders.erase(ID);

        handleWaitingOrders();
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
            //g.printOrderPath(ID);
        } else if (command == "Drop") {
            long long ID, dst;
            cin >> ID >> dst;
            cout << g.processDrop(ID, dst, 0) << endl;
        } else if (command == "Complete") {
            long long ID;
            cin >> ID;
            g.processComplete(ID);
        }/* else if (command == "PT") {
            g.printGraph();
        }*/
    }
    //g.printGraph();
    return 0;
}
