#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<int> allWrongCount;
vector<string> allP, allT;

#define FOR(i, l, r) for (i = l; i <= (int)r; i++)

vector<int> rabinKarpMatcher(string T, string P, int d, int q) {
    int n, m, i, p, h, s;
    n = T.length() - 1;
    m = P.length() - 1;
    vector<int> ans;
    vector<int> t(n, 0);
    int wrongCount = 0;
    p = 0;
    h = 1;
    FOR (i, 1, m - 1)
        h = (h * d) % q;
    FOR (i, 1, m) {
        p = (d * p + P[i]) % q;
        t[0] = (d * t[0] + T[i]) % q;
    }
    FOR (s, 0, n - m) {
        if (p == t[s]) {
            bool same = true;
            FOR (i, 1, m)
                if (P[i] != T[s + i]) {
                    same = false;
                    break;
                }
            if (same)
                ans.push_back(s + 1);
            else
                wrongCount++;
        }
        if (s < n - m)
            t[s + 1] = ((d * (t[s] - T[s + 1] * h) + T[s + m + 1]) % q + q) % q;
    }
    allWrongCount.push_back(wrongCount);
    return ans;
}

void solve(string T, string P) {
    T = " " + T;
    P = " " + P;
    allWrongCount.clear();
    vector<int> match = rabinKarpMatcher(T, P, 2, 13);
    rabinKarpMatcher(T, P, 2, 1009);
    rabinKarpMatcher(T, P, 10, 13);
    rabinKarpMatcher(T, P, 10, 1009);
    cout << match.size() << '\n';
    for (auto wrongCount : allWrongCount)
        cout << wrongCount << ' ';
    cout << '\n';
    for (auto element : match)
        cout << element << '\t';
    cout << "\n\n";
}

void printTime(int d, int q) {
    int i;
    string P, T;
    cerr << '(' << d << ',' << q << ")\n\t";
    FOR (i, 0, 4) {
        P = allP[i];
        T = allT[i];
        clock_t start = clock();
        rabinKarpMatcher(T, P, d, q);
        clock_t end = clock();
        clock_t time = end - start;
        cerr << time << ' ';
    }
    cerr << '\n';
}

int main() {
    int i;
    string P, T;
    ifstream inputFile("../input/4_2_input.txt");

    FOR (i, 0, 4) {
        inputFile >> P;
        inputFile >> T;
        allP.push_back(P);
        allT.push_back(T);
        solve(T, P);
    }
    printTime(2, 13);
    printTime(2, 1009);
    printTime(10, 13);
    printTime(10, 1009);

    return 0;
}
