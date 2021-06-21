#include <vector>
#include <tuple>
#include <iostream>
#include <algorithm>
#include <set>
using namespace std;

struct compare {
    bool operator()(const tuple<int, int, int>& lhs,
            const tuple<int, int, int>& rhs) {
        int p1, f1, t1;
        int p2, f2, t2;
        tie(p1, f1, t1) = lhs;
        tie(p2, f2, t2) = rhs;
        return t1 * f2 < t2 * f1;
    }
};

struct comp2 {
    bool operator()(const tuple<int, int, int>& lhs,
            const tuple<int, int, int>& rhs) {
        int p1, f1, t1;
        int p2, f2, t2;
        tie(p1, f1, t1) = lhs;
        tie(p2, f2, t2) = rhs;
        return (double(p1 - f1) / t1) < (double(p2 - f2) / t2);
    }

};

void printvec(vector<tuple<int, int, int>> rows) {
    for (const auto& r : rows) {
        cout << std::get<0>(r) << ", ";
    }
    cout << endl;
}

using Vec = set<tuple<int, int, int>>;

bool is_better(const tuple<int, int, int>& lhs,
        const tuple<int, int, int>& rhs, int T) {
    int p1, f1, t1;
    int p2, f2, t2;
    tie(p1, f1, t1) = lhs;
    tie(p2, f2, t2) = rhs;
    if (t1 + t2 > T) {  // solo hacer uno
        return (p1 - (f1 * t1)) > (p2 - (f2 * t2));
    }
    return t1 * f2 < t2 * f1;
}


Vec::iterator findnext(const Vec& rows, int T) {
    Vec::iterator current = rows.begin();
    for (auto start = rows.begin(); start != rows.end(); ++start) {
        if (get<2>(*start) > T) {
            continue;
        }
        if (is_better(*start, *current, T)) {
            current = start;
        }
    }
    return current;
}

int main() {
    int N, T;
    cin >> N >> T;
    Vec rows;
    for (int i = 0; i < N; i++) {
        int p, f, t;
        cin >> p >> f >> t;
        rows.emplace(p, f, t);
    }

    int64_t acc = 0;
    int64_t tacc = 0;
    while (!rows.empty()) {
        auto best = findnext(rows, T - tacc);
        int p, f, t;
        tie(p, f, t) = *best;
        cout << "best is " << p << endl;
        tacc += t;
        if (tacc > T) break;
        acc += p;
        acc -= f * tacc;
        rows.erase(best);
    }

    cout << acc << endl;
}
