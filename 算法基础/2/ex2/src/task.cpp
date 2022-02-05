#include <fstream>
#include <iostream>
#include <vector>
#include "DisjointSet.cpp"

#define FOR(i, l, r) for (i = l; i <= r; i++)

#define N 50

using std::cerr;
using std::cout;
using std::vector;

std::ifstream inputFile;
int n, count, operateCount;
vector<vector<int>> associate(N, vector<int>(N));

void printTime(void (*function)()) {
    clock_t start = clock();

    function();

    clock_t end = clock();

    clock_t time = end - start;
    cerr << time << " / " << operateCount << '\n';
}

void core() {
    int i, j;
    count = 0;
    vector<Node> set(n);
    operateCount = n;

    FOR (i, 0, n - 1)
        FOR (j, 0, n - 1)
            if (associate[i][j] == 1) {
                combine(&set[i], &set[j]);
                operateCount++;
            }
    FOR (i, 0, n - 1)
        if (set[i].p == &set[i])
            count++;
}

void familyNumber(int m) {
    int i, j;
    n = m;
    FOR (i, 0, n - 1)
        FOR (j, 0, n - 1)
            inputFile >> associate[i][j];
    printTime(core);
    cout << count << '\n';
}

int main() {
    inputFile.open("../input/2_2_input.txt");
    if (!inputFile.is_open())
        exit(1);

    familyNumber(10);
    familyNumber(15);
    familyNumber(20);
    familyNumber(25);
    familyNumber(30);

    if (!inputFile.is_open())
        exit(1);

    inputFile.close();

    return 0;
}
