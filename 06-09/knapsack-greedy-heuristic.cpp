#include <bits/stdc++.h>
using namespace std;


int constructive_greedy(int n, int Q, const vector<pair<int, int>> &v, vector<int> &s) {
    int total_profit = 0;

    vector<int> c(n);
    iota(c.begin(), c.end(), 0);
    sort(c.begin(), c.end(), [&](const int x, const int y) {
        float qx = v[x].first / (float) v[x].second,
              qy = v[y].first / (float) v[y].second;
        return qx > qy;
    });

    for (int g : c) {
        if (v[g].second > Q) {
            continue;
        }

        total_profit += v[g].first;
        Q -= v[g].second;
        s.push_back(g);
    }

    return total_profit;
}

int main() {
    freopen("knapsack-input.txt", "r", stdin);
    freopen("knapsack-greedy-heuristic-output.txt", "w", stdout);

    int n, Q;
    cin >> n >> Q;

    vector<pair<int, int>> v(n);    
    for (int i=0; i<n; i++) {
        int p;
        cin >> p;
        v[i].first = p;
    }
    for (int i=0; i<n; i++) {
        int w;
        cin >> w;
        v[i].second = w;
    }
    
    vector<int> s;
    int profit = constructive_greedy(n, Q, v, s);

    cout << profit << endl;
    for (auto x : s) {
        cout << x << " ";
    }
    cout << endl;

    return 0;
}
