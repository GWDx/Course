#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define FOR(i, l, r) for (i = l; i <= (int)r; i++)

vector<int> computePrefixFunction(string P) {
    int m, q, k;
    m = P.length() - 1;
    vector<int> pi(m + 1, 0);
    k = 0;
    FOR (q, 2, m) {
        while (k > 0 and P[k + 1] != P[q])
            k = pi[k];
        if (P[k + 1] == P[q])
            k++;
        pi[q] = k;
    }
    return pi;
}

vector<int> kmpMatcher(string T, string P) {
    int n, m, i, q;
    vector<int> ans;
    n = T.length() - 1;
    m = P.length() - 1;
    q = 0;
    vector<int> pi = computePrefixFunction(P);
    FOR (i, 1, n) {
        while (q > 0 and P[q + 1] != T[i])
            q = pi[q];
        if (P[q + 1] == T[i])
            q++;
        if (q == m) {
            ans.push_back(i - m + 1);
            q = pi[q];
        }
    }
    return ans;
}

void solve(string T, string P) {
    clock_t start = clock();
    int i;
    T = " " + T;
    P = " " + P;
    vector<int> pi = computePrefixFunction(P);
    vector<int> match = kmpMatcher(T, P);
    clock_t end = clock();
    clock_t time = end - start;
    cout << match.size() << '\n';
    FOR (i, 1, pi.size() - 1)
        cout << pi[i] << ' ';
    cout << '\n';
    for (auto element : match)
        cout << element << '\t';
    cout << "\n\n";
    cerr << time << '\n';
}

int main() {
    int i;
    string P, T;
    ifstream inputFile("../input/4_1_input.txt");

    FOR (i, 1, 5) {
        inputFile >> P;
        inputFile >> T;
        solve(T, P);
    }
    return 0;
}
