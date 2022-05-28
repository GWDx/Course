#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <unordered_set>

using namespace std;

// ./generate 1000 10000 10000
int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Usage: ./generate m n notZeroCount" << endl;
        return 0;
    }

    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    int notZeroCount = atoi(argv[3]);

    vector<int> row, col, val;
    vector<unordered_set<int>> appearCol(m + 1);
    // row is random number between 1 and m, length is notZeroCount
    // col is random number between 1 and n, length is notZeroCount
    // (row, col) is unique
    // val is random number between 1 and 100, length is notZeroCount
    for (int i = 0; i < notZeroCount; i++) {
        while (true) {
            int row_tmp = rand() % m + 1;
            int col_tmp = rand() % n + 1;
            if (appearCol[row_tmp].find(col_tmp) == appearCol[row_tmp].end()) {
                appearCol[row_tmp].insert(col_tmp);

                row.push_back(row_tmp);
                col.push_back(col_tmp);
                val.push_back(rand() % 100 + 1);

                break;
            }
        }
    }

    // sort row and col
    // sort(row.begin(), row.end());

    // export test.mtx
    ofstream out("matrix.mtx");
    out << m << " " << n << " " << notZeroCount << "\n";
    for (int i = 0; i < notZeroCount; i++)
        out << row[i] << " " << col[i] << " " << val[i] << "\n";

    // generate x
    vector<int> x(n, 0);
    for (int i = 0; i < n; i++)
        x[i] = rand() % 100 + 1;

    ofstream outX("x.txt");
    for (int i = 0; i < n; i++)
        outX << x[i] << "\n";

    out.close();
    outX.close();

    return 0;
}
