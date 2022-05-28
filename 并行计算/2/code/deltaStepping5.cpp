#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <climits>
#include <omp.h>

using namespace std;

int lenV, lenB;
int delta = 5;
int kernelNumber;
vector<vector<pair<int, int>>> adjLight;
vector<vector<pair<int, int>>> adjHeavy;
// vector<unordered_set<int>> B;
// vector<unordered_set<pair<int, bool>>> B;
vector<unordered_map<int, bool>> B;
vector<int> tent;

void parallelRelax(int w, int x) {
    if (x < tent[w]) {
        int index = tent[w] / delta;
        if (index < lenB) {
            // #pragma omp critical
            B[index][w] = false;
        }
        index = x / delta;

#pragma omp critical
        B[index][w] = true;
        tent[w] = x;
    }
}

void relax(int w, int x) {
    if (x < tent[w]) {
        int index = tent[w] / delta;
        if (index < lenB) {
            B[index].erase(w);
        }
        index = x / delta;

        B[index][w] = true;
        tent[w] = x;
    }
}

void relaxRequest(vector<pair<int, int>> req) {
    int reqSize = (int)req.size();
    if (reqSize < 50)
        for (int i = 0; i < reqSize; i++)
            relax(req[i].first, req[i].second);
    else
#pragma omp parallel for
        for (int i = 0; i < reqSize; i++)
            parallelRelax(req[i].first, req[i].second);
}

vector<pair<int, int>> findLightRequests(unordered_map<int, bool>& Bi) {
    vector<pair<int, int>> ans;
    for (auto& it : Bi) {
        int v = it.first;
        for (auto& p : adjLight[v])
            ans.push_back({p.first, tent[v] + p.second});
    }
    return ans;
}

vector<pair<int, int>> findHeavyRequests(unordered_map<int, bool>& Bi) {
    vector<pair<int, int>> ans;
    for (auto& it : Bi) {
        int v = it.first;
        for (auto& p : adjHeavy[v])
            ans.push_back({p.first, tent[v] + p.second});
    }
    return ans;
}

int bucketIndex = 0;

void deltaStepping(vector<vector<pair<int, int>>>& adj, int s) {
    // reserve
    for (int i = 0; i < lenV; i++) {
        int u, v, w;
        u = i;
        for (auto& p : adj[u]) {
            v = p.first;
            w = p.second;
            if (w <= delta)
                adjLight[u].push_back({v, w});
            else
                adjHeavy[u].push_back({v, w});
        }
    }

    B = vector<unordered_map<int, bool>>(lenB);
    for (int i = 0; i < lenV; i++)
        B[i / delta][i] = true;

    tent = vector<int>(lenV, INT_MAX / 2);
    relax(s, 0);

    while (true) {
        while (bucketIndex < lenB && B[bucketIndex].empty())
            bucketIndex++;
        if (bucketIndex == lenB)
            break;
        unordered_map<int, bool> R;
        while (!B[bucketIndex].empty()) {
            auto req = findLightRequests(B[bucketIndex]);
            for (auto& p : B[bucketIndex])
                if (p.second)
                    R[p.first] = true;
            B[bucketIndex].clear();
            relaxRequest(req);
        }
        auto req = findHeavyRequests(R);
        relaxRequest(req);
    }
}

// ./2 kernelNumber delta
int main(int argc, char** argv) {
    int lenE;
    // if (argc > 2) {
    //     cout << "./2 kernelNumber" << endl;
    //     return 1;
    // }
    kernelNumber = atoi(argv[1]);
    delta = atoi(argv[2]);
    // unordered_set omp kernel number
    omp_set_num_threads(kernelNumber);

    ifstream in("data.txt");
    in >> lenV >> lenE;
    vector<vector<pair<int, int>>> adj(lenV);
    adjLight = vector<vector<pair<int, int>>>(lenV);
    adjHeavy = vector<vector<pair<int, int>>>(lenV);
    lenB = lenV / 2 + 10;

    for (int i = 0; i < lenE; i++) {
        int u, v, w;
        in >> u >> v >> w;
        adj[u].push_back({v, w});
    }

    // start time omp
    auto start = omp_get_wtime();

    deltaStepping(adj, 0);
    auto end = omp_get_wtime();

    cerr << "time: " << end - start << endl;

    int printLength = min(lenV, 10);
    for (int i = 0; i < printLength; i++) {
        if (tent[i] == INT_MAX / 2)
            cout << "inf\n";
        else
            cout << tent[i] << "\n";
    }

    // for (int i = 0; i < lenV; i++)
    //     cout << tent[i] << "\n";
    return 0;
}
