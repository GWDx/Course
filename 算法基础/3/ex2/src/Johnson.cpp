#include <assert.h>
#include "BellmanFord.cpp"

void swap(Node*& a, Node*& b) {
    Node* temp = a;
    a = b;
    b = temp;
}

class Heap;
class Heap {
public:
    map<int, int> idToIndex;
    vector<Node*> nodes;
    int size;
    Heap& heap = *this;

    void minHeapify(int i) {
        int l, r, minIndex;
        l = 2 * i + 1;
        r = 2 * i + 2;
        if (l < size and nodes[l]->d < nodes[i]->d)
            minIndex = l;
        else
            minIndex = i;
        if (r < size and nodes[r]->d < nodes[minIndex]->d)
            minIndex = r;
        if (minIndex != i) {
            idToIndex[nodes[i]->id] = minIndex;
            idToIndex[nodes[minIndex]->id] = i;

            swap(nodes[i], nodes[minIndex]);
            minHeapify(minIndex);
        }
    }

    Heap(vector<Node>& targetNodes) {
        int i;
        size = targetNodes.size();
        nodes = vector<Node*>(size);
        FOR (i, 0, size - 1)
            idToIndex[i] = i;
        FOR (i, 0, size - 1)
            nodes[i] = &targetNodes[i];
        for (i = size / 2; i >= 0; i--)
            minHeapify(i);
    }

    Node* extractMin() {
        assert(size > 0);
        idToIndex[nodes[size - 1]->id] = 0;
        idToIndex[nodes[0]->id] = INF;
        Node* minNode = nodes[0];
        nodes[0] = nodes[size - 1];

        nodes.pop_back();
        size--;
        minHeapify(0);
        return minNode;
    }

    void decreaseKey(int i, int key) {
        assert(key <= nodes[i]->d);
        nodes[i]->d = key;
        while (i > 0 and nodes[(i - 1) / 2]->d > nodes[i]->d) {
            Node*& A = nodes[i];
            Node*& B = nodes[(i - 1) / 2];
            idToIndex[A->id] = (i - 1) / 2;
            idToIndex[B->id] = i;
            swap(A, B);
            i = (i - 1) / 2;
        }
    }
};

void relax2(Graph& G, Heap& heap, int u, int v, int w) {
    if (G.nodes[v].d > G.nodes[u].d + w) {
        heap.decreaseKey(heap.idToIndex[v], G.nodes[u].d + w);
        G.nodes[v].prev = u;
    }
}

void dijkstra(Graph& G, int s) {
    initializeSingleSource(G, s);
    Heap heap(G.nodes);
    while (heap.size > 0) {
        Node* u = heap.extractMin();
        int uId = u->id;

        for (auto iter = G.adj[uId].begin(); iter != G.adj[uId].end(); iter++)
            relax2(G, heap, uId, iter->first, iter->second);
    }
}

void johnson(Graph& G, vector<vector<int>>& d) {
    int s, i, j, n;
    vector<int> h;

    s = G.nodes.size();
    n = s + 1;
    Node newNode;
    Graph G2 = G;
    newNode.id = s;
    G2.nodes.push_back(newNode);
    G2.adj.push_back(map<int, int>());
    FOR (i, 0, s - 1)
        G2.adj[s][i] = 0;
    Graph G3 = G2;

    bellmanFord(G2, s);
    FOR (i, 0, s)
        h.push_back(G2.nodes[i].d);
    FOR (i, 0, s)
        for (auto iter = G2.adj[i].begin(); iter != G2.adj[i].end(); iter++) {
            int v = iter->first;
            int w = iter->second;
            G3.adj[i][v] = w + h[i] - h[v];
        }
    d = vector<vector<int>>(n, vector<int>(n));
    FOR (i, 0, s - 1) {
        dijkstra(G3, i);
        FOR (j, 0, s - 1)
            d[i][j] = G3.nodes[j].d + h[j] - h[i];
    }
}
