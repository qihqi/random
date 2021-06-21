#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>

using namespace std;

using P = pair<int, int>;

int main() {
    int n;
    cin >> n;
    vector<P> points;
    for (int i = 0; i < n; i++) {
        points.emplace_back();
        cin >> points.back().first >> points.back().second;
    }

    sort(points.begin(), points.end());

    vector<P> rest;
    int pareto = 0;
    while (!points.empty()) {
        P current = points.back();
        points.pop_back();
        bool is_current_unique = true;
        while (points.size() >= 1 &&
            points.back() == current) {
            is_current_unique = false;
            points.pop_back();
        }
        if (is_current_unique) {
            pareto += 1;
        }
        for (const auto& p : points) {
            if (p.second > current.second) {
                rest.push_back(p);
            }
        }
        swap(rest, points);
        rest.clear();
    }

    cout << pareto << endl;
    return 0;
}






