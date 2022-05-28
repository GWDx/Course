// import random
// import os
// import sys
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>

// lenV = int(sys.argv[1])
// lenE = int(sys.argv[2])

using namespace std;

int main(int argc, char** argv) {
    int lenV = atoi(argv[1]);
    int lenE = atoi(argv[2]);

    // with open('data.txt') as f:
    //     lines = f.readlines()

    ifstream in("data.txt");
    vector<pair<int, int>> edges;
    for (int i = 0; i < lenE; i++) {
        int u, v;
        in >> u >> v;
        edges.push_back({u, v});
    }
    in.close();

    // with open('data.txt', 'w') as f:
    //     f.write(f'{lenV} {lenE}\n')
    //     for line in lines:
    //         u, v = line.split()
    //         w = random.randint(10, 100)
    //         f.write(f'{u} {v} {w}\n')

    ofstream out("data.txt");
    out << lenV << " " << lenE << endl;
    for (auto& p : edges) {
        int u = p.first;
        int v = p.second;
        int w = rand() % 90 + 10;
        out << u << " " << v << " " << w << endl;
    }
    out.close();
}
