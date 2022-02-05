#include <iostream>
#include <limits>

#define N 30
#define FOR(i, l, r) for (i = l; i <= r; i++)

using namespace std;

void printOptimalParens(int s[N][N], int i, int j) {
    if (i == j)
        cout << " A" << to_string(i);
    else {
        cout << '(';
        printOptimalParens(s, i, s[i][j]);
        printOptimalParens(s, s[i][j] + 1, j);
        cout << ')';
    }
}

void matrixChainOrder(long long p[N], int n, long long m[N][N], int s[N][N]) {
    int l, i, j, k;
    long long q;
    FOR (i, 1, n)
        m[i][i] = 0;
    FOR (l, 2, n)
        FOR (i, 1, n) {
            j = i + l - 1;
            m[i][j] = numeric_limits<long long>::max();
            FOR (k, i, j - 1) {
                q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
                if (q < m[i][j]) {
                    m[i][j] = q;
                    s[i][j] = k;
                }
            }
        }
}

void printMS(long long m[N][N], int s[N][N], int n) {
    int i, j;
    cout.setf(ios::left);
    FOR (i, 1, n) {
        FOR (j, 1, n) {
            cout.width(16);
            cout << m[i][j];
        }
        cout << '\n';
    }
    cout << '\n';
    FOR (i, 1, n) {
        FOR (j, 1, n) {
            cout.width(16);
            cout << s[i][j];
        }
        cout << '\n';
    }
    cout << '\n';
}

int main(int argc, char* argv[]) {
    int n, i, j, epoch;
    long long p[N];
    long long m[N][N];
    int s[N][N];

    cin >> n;
    FOR (i, 0, n)
        cin >> p[i];

    if (argc > 1)
        epoch = stoi(argv[1]);
    else
        epoch = 1;
    FOR (i, 1, epoch)
        matrixChainOrder(p, n, m, s);

    if (argc > 2)
        printMS(m, s, n);

    cout << m[1][n] << '\n';
    printOptimalParens(s, 1, n);
    cout << '\n';
    return 0;
}
