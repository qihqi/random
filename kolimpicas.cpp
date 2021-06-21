#include <iostream>
#include <unordered_map>

using namespace std;

int main() {
    int n, k;
    cin >> n >> k;

    unordered_map<int, int> counts;

    for (int i = 0; i < n; i++) {
        int p;
        cin >> p;
        int residue = p % k;
        counts[residue]++;
    }

    int64_t total = 0;

    for (const auto& p : counts) {
        total += (p.second * (p.second - 1) / 2);
    }

    cout << total << endl;


    return 0;
}
