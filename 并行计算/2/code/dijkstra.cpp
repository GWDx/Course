#include <bits/stdc++.h>

using namespace std;

int lenV, lenE;

vector<int> dijkstra(vector<vector<pair<int, int>>>& adj, int s) {
    vector<int> dist(lenV, INT_MAX / 2);
    priority_queue<pair<int, int>> pq;
    pq.push({0, s});
    dist[s] = 0;
    while (!pq.empty()) {
        int d = pq.top().first;
        int v = pq.top().second;
        pq.pop();
        if (dist[v] < d)
            continue;
        for (auto& p : adj[v]) {
            int u = p.first;
            int w = p.second;
            if (dist[u] > dist[v] + w) {
                dist[u] = dist[v] + w;
                pq.push({dist[u], u});
            }
        }
    }
    return dist;
}

int main() {
    ifstream in("data.txt");
    in >> lenV >> lenE;
    vector<vector<pair<int, int>>> adj(lenV);

    for (int i = 0; i < lenE; i++) {
        int u, v, w;
        in >> u >> v >> w;
        adj[u].push_back({v, w});
    }
    auto dist = dijkstra(adj, 0);
    int printLength = min(lenV, 10);
    for (int i = 0; i < printLength; i++) {
        if (dist[i] == INT_MAX / 2)
            cout << "inf\n";
        else
            cout << dist[i] << "\n";
    }
    return 0;
}
