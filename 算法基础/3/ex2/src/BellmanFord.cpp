#include <iostream>
#include <map>
#include <vector>

#define FOR(i, l, r) for (i = l; i <= r; i++)

#define INF 50000000

using std::map;
using std::vector;

class Node {
public:
    int d;
    int prev;
    int id;
};

class Edge {
public:
    int from;
    int to;
    int weight;

    Edge(int from, int to, int weight) {
        this->from = from;
        this->to = to;
        this->weight = weight;
    }
};

class Graph {
public:
    vector<Node> nodes;
    vector<map<int, int>> adj;

    Graph(int n) {
        int i;
        nodes = vector<Node>(n);
        adj = vector<map<int, int>>(n);
        FOR (i, 0, n - 1)
            nodes[i].id = i;
    }
};

void initializeSingleSource(Graph& G, int s) {
    for (auto&& v : G.nodes) {
        v.d = 2 * INF;
        v.prev = -1;
    }
    G.nodes[s].d = 0;
}

void relax(Graph& G, int u, int v, int w) {
    if (G.nodes[v].d > G.nodes[u].d + w) {
        G.nodes[v].d = G.nodes[u].d + w;
        G.nodes[v].prev = u;
    }
}

void bellmanFord(Graph& G, int s) {
    int i, j, n;
    n = G.nodes.size();
    initializeSingleSource(G, s);

    FOR (j, 1, n - 1)
        FOR (i, 0, n - 1)
            for (auto iter = G.adj[i].begin(); iter != G.adj[i].end(); iter++)
                relax(G, i, iter->first, iter->second);

    FOR (i, 0, n - 1)
        for (auto iter = G.adj[i].begin(); iter != G.adj[i].end(); iter++) {
            Node* u = &G.nodes[i];
            Node* v = &G.nodes[iter->first];
            int w = iter->second;
            if (v->d > u->d + w)
                exit(2);
        }
}
