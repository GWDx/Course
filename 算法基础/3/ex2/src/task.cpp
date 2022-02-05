#include <fstream>
#include <iostream>
#include "Johnson.cpp"

void solve(const char* inputFileName, const char* outputFileName, int n) {
    int i, j, s, weight;
    vector<vector<int>> d, prev;
    FILE* inputFile = fopen(inputFileName, "r");
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
    johnson(G, d);
    clock_t end = clock();
    clock_t time = end - start;
    std::cerr << time << '\n';

    FOR (s, 0, n - 1) {
        FOR (j, 0, n - 1) {
            if (d[s][j] > INF)
                fprintf(outputFile, "    X");
            else
                fprintf(outputFile, "%5d", d[s][j]);
        }
        fprintf(outputFile, "\n");
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
