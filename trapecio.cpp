#include <iostream>
#include <vector>
using namespace std;

// 200 * 100 * 10000 = 2 e 8 =

int main() {
    int n;  // 10000
    cin >> n;
    vector<int> heights(n + 1);  // hasta 100
    vector<int> bases(n);
    for (int i = 0; i < n + 1; i++) {
        cin >> heights[i];
    }
    for (int i = 0; i < n; i++) {
        cin >> bases[i];
    }

    int64_t total = 0;
    for (int i = 0; i < n; i++) {
        int64_t area = (heights[i] + heights[i + 1]) * bases[i];
        total += area;
    }
    cout << total / 2;
    if (total % 2 == 1) {
        cout << ".5";
    }
    cout << endl;

    return 0;
}
