#include <bits/stdc++.h>
using namespace std;


pair<int, int> constructive_greedy_randomized(int n, int Q, const vector<pair<int, int>> &v, float t, float a, vector<int> &s) {
    auto start = chrono::high_resolution_clock::now();
    
    int total_profit = 0;
    int curr_Q = Q;

    vector<float> values;
    for (int i=0; i<n; i++) {
        float value = v[i].first / (float) v[i].second;
        values.push_back(value);
    }

    vector<int> c(v.size()), c_value_ordered(v.size());
    iota(c.begin(), c.end(), 0);
    iota(c_value_ordered.begin(), c_value_ordered.end(), 0);

    sort(c.rbegin(), c.rend(), [&](const int x, const int y) {
        return v[x].second < v[y].second;
    });

    sort(c_value_ordered.rbegin(), c_value_ordered.rend(), [&](const int x, const int y) {
        return values[x] < values[y];
    });

    while (c.size() > 0 && v[*c.begin()].second > curr_Q) {
        c_value_ordered.erase(
            remove(c_value_ordered.begin(), c_value_ordered.end(), *c.begin()),
            c_value_ordered.end()
        );
        c.erase(c.begin());
    }

    while (c.size() > 0) {
        auto current = chrono::high_resolution_clock::now();
        if (chrono::duration<float>(current - start).count() > t)
            break;

        float min_value = values[c_value_ordered.back()],
              max_value = values[c_value_ordered.front()];

        float threshold = max_value - a * (max_value - min_value);

        auto rc_end = lower_bound(
            c_value_ordered.begin(),
            c_value_ordered.end(),
            threshold,
            [&](const int x, const float threshold) {
                return values[x] >= threshold;
            }
        );

        int g = c_value_ordered[rand() % (distance(c_value_ordered.begin(), rc_end))];

        total_profit += v[g].first;
        curr_Q -= v[g].second;
        s.push_back(g);
        
        c.erase(remove(c.begin(), c.end(), g), c.end());
        c_value_ordered.erase(remove(c_value_ordered.begin(), c_value_ordered.end(), g), c_value_ordered.end());

        while (c.size() > 0 && v[*c.begin()].second > curr_Q) {
            c_value_ordered.erase(
                remove(c_value_ordered.begin(), c_value_ordered.end(), *c.begin()),
                c_value_ordered.end()
            );
            c.erase(c.begin());
        }
    }

    return { total_profit, Q - curr_Q };
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
        pair<int, int> r = constructive_greedy_randomized(n, Q, v, 1.0, a, s);

        cout << "[a = " << fixed << setprecision(2) << a << "]: "
             << "profit: " << r.first << ", weight: " << r.second << ", " 
             << "solution: ( ";
        
        for (auto x : s)
            cout << x << " ";

        cout << ");" << endl;

        s.clear();
    }

    return 0;
}
