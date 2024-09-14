#include <bits/stdc++.h>
using namespace std;


int constructive_greedy_randomized(int n, int Q, const vector<pair<int, int>> &v, float a, vector<int> &s) {
    int total_profit = 0;

    vector<float> values;
    float min_value = numeric_limits<float>::max(),
          max_value = numeric_limits<float>::min();

    for (int i=0; i<n; i++) {
        float value = v[i].first / (float) v[i].second;

        min_value = min(min_value, value);
        max_value = max(max_value, value);

        values.push_back(value);
    }

    float treshold = min_value + (1 - a) * (max_value - min_value);
    vector<int> rc, nrc;
    for (int i=0; i<n; i++) {
        if (values[i] > treshold) rc.push_back(i);
        else nrc.push_back(i);
    }

    sort(rc.begin(), rc.end(), [&](const int x, const int y) {
        return v[x].second < v[y].second;
    });

    sort(nrc.begin(), nrc.end(), [&](const int x, const int y) {
        return values[x] > values[y];
    });

    while (rc.size() > 0 && Q < v[(*rc.rbegin())].second) {
        rc.pop_back();
    }

    while (rc.size() > 0 && Q >= v[rc[0]].first) {
        auto g = rc.begin() + (rand() % rc.size());
        s.push_back(*g);
        total_profit += v[*g].first;
        Q -= v[*g].second;
        rc.erase(g);

        while (rc.size() > 0 && Q < v[(*rc.rbegin())].second) {
           rc.pop_back();
        }
    }

    for (int g : nrc) {
        if (v[g].second > Q) continue;

        total_profit += v[g].first;
        Q -= v[g].second;
        s.push_back(g);
    }

    return total_profit;
}

int main() {
    freopen("knapsack-input.txt", "r", stdin);
    freopen("knapsack-greedy-randomized-heuristic-output.txt", "w", stdout);

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
    for (int i=0; i<=100; i++) {
        float a = 0.01 * i;
        int profit = constructive_greedy_randomized(n, Q, v, a, s);

        cout << "[a = " << fixed << setprecision(2) << a << "]: " << profit << " - solution: ( ";
        for (auto x : s) {
            cout << x << " ";
        }
        cout << ");" << endl;

        s.clear();
    }

    return 0;
}
