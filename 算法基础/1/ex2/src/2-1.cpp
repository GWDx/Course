#include <iostream>
#include <set>
#include <string>
#include <vector>

#define LeftUp 3
#define Left 2
#define Up 1

#define FOR(i, l, r) for (i = l; i <= r; i++)

using namespace std;

set<string> allStringLCS(vector<vector<int>> b, vector<vector<int>> c, string X, int m, int n) {
    int i, j;
    vector<vector<set<string>>> LCSSet(m + 1, vector<set<string>>(n + 1));

    FOR (i, 0, m)
        LCSSet[i][0].insert("");
    FOR (j, 1, n)
        LCSSet[0][j].insert("");

    FOR (i, 1, m)
        FOR (j, 1, n) {
            set<string> temp;
            if (b[i][j] == LeftUp)
                for (auto item : LCSSet[i - 1][j - 1])
                    temp.insert(item + X[i - 1]);
            if (c[i][j] == c[i - 1][j])
                temp.insert(LCSSet[i - 1][j].begin(), LCSSet[i - 1][j].end());
            if (c[i][j] == c[i][j - 1])
                temp.insert(LCSSet[i][j - 1].begin(), LCSSet[i][j - 1].end());
            LCSSet[i][j] = temp;
        }
    return LCSSet[m][n];
}

void LCSLength(string X, string Y, vector<vector<int>>& b, vector<vector<int>>& c) {
    int m, n, i, j;
    m = X.length();
    n = Y.length();

    FOR (i, 1, m)
        FOR (j, 1, n) {
            if (X[i - 1] == Y[j - 1]) {
                c[i][j] = c[i - 1][j - 1] + 1;
                b[i][j] = LeftUp;
            } else if (c[i - 1][j] >= c[i][j - 1]) {
                c[i][j] = c[i - 1][j];
                b[i][j] = Up;
            } else {
                c[i][j] = c[i][j - 1];
                b[i][j] = Left;
            }
        }
    return;
}

int main(int argc, char* argv[]) {
    int m, n, epoch, i;
    string X, Y;
    set<string> allLCS;

    cin >> n >> X >> Y;
    m = X.length();
    n = Y.length();

    vector<vector<int>> b(m + 1, vector<int>(n + 1));
    vector<vector<int>> c(m + 1, vector<int>(n + 1));

    if (argc > 1)
        epoch = stoi(argv[1]);
    else
        epoch = 1;

    FOR (i, 1, epoch)  // {
        LCSLength(X, Y, b, c);
    allLCS = allStringLCS(b, c, X, m, n);
    //}

    cout << allLCS.size() << '\n';
    for (auto i : allLCS)
        cout << i << '\n';
    return 0;
}
