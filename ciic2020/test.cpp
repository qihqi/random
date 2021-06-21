#include<map>
#include<iostream>

using namespace std;

int main() {
    map<int, int> x;
    x[3] = 10;
    x[4] = 10;
    x[11] = 10;
    x[12] = 10;
    cout << x.upper_bound(10)->first << endl;
}
