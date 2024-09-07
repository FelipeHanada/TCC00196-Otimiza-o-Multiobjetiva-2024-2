#include <bits/stdc++.h>
using namespace std;


int constructive_random(int n, int Q, const vector<pair<int, int>> &v, vector<int> &s) {
    int total_profit = 0;
    
    vector<int> c(n);
    iota(c.begin(), c.end(), 0);
    sort(c.begin(), c.end(), [&](const int x, const int y) {
        return v[x].second < v[y].second;
    });

    while (c.size() > 0 && Q < v[(*c.rbegin())].second) {
        c.pop_back();
    }

    while (c.size() > 0 && Q >= v[c[0]].first) {
        auto g = c.begin() + (rand() % c.size());
        s.push_back(*g);
        total_profit += v[*g].first;
        Q -= v[*g].second;
        c.erase(g);

        while (c.size() > 0 && Q < v[(*c.rbegin())].second) {
           c.pop_back();
        }
    }

    return total_profit;
}

int constructive_random_punitive(int n, int Q, const vector<pair<int, int>> &v, vector<int> &s) {
    int penalty = 0;

    int total_profit = 0;
    for (int g=0; g<n; g++) {
        penalty += v[g].second;
        
        if (rand() % 2) {
            total_profit += v[g].first;
            Q -= v[g].second;
            s.push_back(g);
        }
    }

    if (Q < 0) {
        total_profit -= penalty;
    }

    return total_profit;
}

void print_solution(int profit, vector<int> &s) {
    cout << profit << endl;
    for (auto x : s) {
        cout << x << " ";
    }
    cout << endl;
}

int random_heuristic(
    int t, int n, int Q, vector<pair<int, int>> v, vector<int> &s, float &avg,
    int (*constructive_method) (int, int, const vector<pair<int, int>>&, vector<int>&)
) {
    int max_profit = INT32_MIN;
    float sum_profit = 0;
    
    for (int i=0; i<t; i++) {
        vector<int> s_temp;
        int profit = constructive_method(n, Q, v, s_temp);
        sum_profit += profit;
 
        if (profit > max_profit) {
            max_profit = profit;
            s.clear();
            for (auto x : s_temp) s.push_back(x);
        }
    }

    avg = sum_profit / (float) t;
    return max_profit;
}


int main() {
    freopen("knapsack-input.txt", "r", stdin);
    freopen("knapsack-random-heuristic-output.txt", "w", stdout);

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
    float avg;
    int profit = random_heuristic(100, n, Q, v, s, avg, constructive_random);
    cout << "[ CONSTRUCTIVE RANDOM ]" << endl
         << "avg: " << avg << endl
         << "best solution:" << endl;
    print_solution(profit, s);
    cout << endl;

    profit = random_heuristic(100, n, Q, v, s, avg, constructive_random_punitive);
    cout << "[ CONSTRUCTIVE RANDOM with PENALTY ]" << endl
         << "avg: " << avg << endl
         << "best solution:" << endl;
    print_solution(profit, s);

    return 0;
}
