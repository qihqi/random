#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <climits>
#include <unordered_set>
#include <map>
using namespace std;


// P, W, D;
// P - c <= D ==> no camina
// P - c > D ==> (P - c - D)
// P - c > D ==> tiempo = (P - c - D) * w

// [s, e]
//
//
//
//   i10________)____x_(____(___________j4     z15
//   N * 10
//   M * 4
//   x -> x - 1
//   (| pi - c| -  di) * wi = (c - pi - di) * wi
//                           = wi * c - pi*wi - di*wi
//   (| pi - c| -  di) * wi = (pi - c - di) * wi
//                           = -wi * c - pi*wi - di*wi
//  suma(wi) - suma(wj)
int64_t tiempo_total(
        const vector<int>& p,
        const vector<int>& w,
        const vector<int>& d,
        int c) {
    int64_t total = 0;
    for (int i = 0; i < p.size(); i++) {
        int diff = std::abs(p[i] - c) - d[i];
        if (diff < 0) {
            continue;
        }
        total += diff * w[i];
    }
    return total;
}

int main() {
    int n;
    cin >> n;
    vector<int64_t> p(n);
    vector<int64_t> w(n);
    vector<int64_t> d(n);


    // bool entrar = true, salir es false
    map<int, vector<pair<int, bool>>> posibles;
    map<int, int> persons;
    for (int i = 0; i < n; i++) {
        cin >> p[i] >> w[i] >> d[i];

        posibles[p[i] + d[i]].emplace_back(i, false);
        posibles[p[i] - d[i]].emplace_back(i, true);
        persons[p[i]] = i;
    }

    int64_t weights_left = 0;
    int64_t weights_right = 0;
    int64_t other = 0;

    for (int i = 0; i < n; i++) {
        weights_right += w[i];
        other += (p[i] - d[i])*w[i];
    }

    // 10^9 == 2^30
    // W + 1000 ==2 ^ 10


    uint64_t current = LLONG_MAX;
    for (auto ahora = posibles.begin(); ahora != posibles.end(); ++ahora) {
        for (auto c : ahora->second) {
            if (c.second) {
                // entrar
                other -= (p[c.first] - d[c.first]) * w[c.first];
                weights_right -= w[c.first];
            } else {
                other += (-p[c.first]  - d[c.first]) * w[c.first];
                weights_left += w[c.first];
            }
        }
        uint64_t t = (weights_left - weights_right) * ahora->first + other;
        if (t < current) {
            current = t;
        }
    }
    cout << current << endl;
}
