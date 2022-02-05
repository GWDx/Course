#include <fstream>
#include <iostream>
#include "BellmanFord.cpp"

void solve(const char* fileName, const char* outputFileName, int n) {
    int i, j, weight;
    int s = 0;
    FILE* inputFile = fopen(fileName, "r");
    FILE* outputFile = fopen(outputFileName, "w");
    if (inputFile == NULL || outputFile == NULL)
        exit(1);

    Graph G(n);
    FOR (i, 0, n - 1)
        FOR (j, 0, n - 1) {
            fscanf(inputFile, "%d,", &weight);
            if (weight)
                G.adj[i][j] = weight;
        }
    fclose(inputFile);

    clock_t start = clock();
    bellmanFord(G, s);
    clock_t end = clock();
    clock_t time = end - start;
    std::cerr << time << '\n';

    FOR (j, 0, n - 1)
        if (G.nodes[j].d < INF) {
            vector<int> path;
            int v = j;
            while (G.nodes[v].prev >= 0) {
                v = G.nodes[v].prev;
                path.push_back(v);
            }
            fprintf(outputFile, "%d,%d,%d;\t", s, j, G.nodes[j].d);

            for (i = path.size() - 1; i >= 0; i--)
                fprintf(outputFile, "%d,", path[i]);
            fprintf(outputFile, "%d\n", j);
        }
    fclose(outputFile);
}

int main() {
    solve("../input/input11.txt", "../output/result11.txt", 27);
    solve("../input/input12.txt", "../output/result12.txt", 27);
    solve("../input/input21.txt", "../output/result21.txt", 81);
    solve("../input/input22.txt", "../output/result22.txt", 81);
    solve("../input/input31.txt", "../output/result31.txt", 243);
    solve("../input/input32.txt", "../output/result32.txt", 243);
    solve("../input/input41.txt", "../output/result41.txt", 729);
    solve("../input/input42.txt", "../output/result42.txt", 729);
    return 0;
}
