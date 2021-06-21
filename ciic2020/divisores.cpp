#include <iostream>
#include <cstdio>
using namespace std;

const int64_t MOD = 1000000000;

int64_t mypow(int64_t base, unsigned int exp)
{
    int64_t res = 1;
    while (exp) {
        if (exp & 1) {
            res *= base;
            res = res % MOD;
        }
        exp >>= 1;
        base *= base;
        base = base % MOD;
    }
    return res;
}
/*
int64_t mypow(int64_t base, int exp) {
    if (exp == 1) {
        return base;
    }

    if (base == 0 || base == 1) {
        return base;
    }

    if (exp % 2 == 0) {
        return mypow((base*base) % MOD, exp >> 1);
    } else {
        return (mypow(base, exp - 1) * base) % MOD;
    }

}
*/

int main() {
    int64_t total = 0;

    int64_t N;
    int k;
    cin >> N >> k;
    int64_t end = N;
    if (end == MOD) end --;
    for (int64_t i = 1; i <= end; i++) {
        total += (mypow(i, k) * (N / i)) % MOD;
        total = total % MOD;
    }
    cout << total << endl;

}
