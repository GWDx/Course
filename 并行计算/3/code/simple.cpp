#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;

int main() {
    ifstream in("matrix.mtx");
    if (!in) {
        cout << "Error opening file" << endl;
        std::abort();
    }

    // read matrix
    string line;
    std::stringstream ss;

    while (getline(in, line))
        if (line[0] != '%')
            ss << line << " ";

    in.close();

    int m, n, notZeroCount;
    ss >> m >> n >> notZeroCount;

    vector<vector<double>> matrix(m, vector<double>(n, 0));

    for (int i = 0; i < notZeroCount; i++) {
        int row, col;
        double val;
        ss >> row >> col >> val;
        matrix[row - 1][col - 1] = val;
    }

    vector<int> x(n, 0);
    ifstream in2("x.txt");
    if (!in2) {
        cout << "Error opening file" << endl;
        std::abort();
    }
    for (int i = 0; i < n; i++)
        in2 >> x[i];

    vector<double> y(m, 0);
    // y = matrix * x
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            y[i] += matrix[i][j] * x[j];

    for (int i = 0; i < n; i++)
        cout << y[i] << "\n";

    return 0;
}
