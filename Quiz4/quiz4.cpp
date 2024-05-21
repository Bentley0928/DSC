#include <bits/stdc++.h>
using namespace std;
typedef pair<int, int> pii;
const long long INF = INT64_MAX;

vector<long long> dijkstra(int start, int n, const vector<vector<pii>>& adj) {
    vector<long long> dist(n, INF);
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;

        for (auto& edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<pii>> adj(n);
    int from, to, weight;

    for (int i = 0; i < m; ++i) {
        cin >> from >> to >> weight;
        adj[from].push_back({to, weight});
    }

    int t, A, B=0, dest;
    cin >> t >> A;
    if (t == 2) cin >> B;
    cin >> dest;

    vector<long long> distFromA = dijkstra(A, n, adj);
    vector<long long> distFromB = dijkstra(B, n, adj);

    long long result = INF;

    if (t == 1) {
        result = distFromA[dest];
    } else if (t == 2) {
        long long directAtoDest = distFromA[dest];
        long long directBtoDest = distFromB[dest];
        long long indirectAtoBtoDest = distFromA[B] == INF || distFromB[dest] == INF ? INF : distFromA[B] + distFromB[dest];
        long long indirectBtoAtoDest = distFromB[A] == INF || distFromA[dest] == INF ? INF : distFromB[A] + distFromA[dest];

        result = min({(directAtoDest+directBtoDest), indirectAtoBtoDest, indirectBtoAtoDest});
    }

    if (result == INF) {
        cout << -1 << endl;
    } else {
        cout << result << endl;
    }

    return 0;
}
