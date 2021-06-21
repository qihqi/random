#include <iostream>
using namespace std;

int main() {
    double x, y;
    double r;
    cin >> x >> y;
    cin >> r;

    double r2 = r * r;
    int total = 0;

    for (int px = int(x - r); px <= int(x + r); px++) {
        for (int py = int(y - r); py <= int(y + r); py++) {
            double dx = px - x;
            double dy = py - y;
            if ( dx * dx + dy*dy < r2) {
                total++;
            }
        }
    }
    cout << total << endl;
    return 0;
}
