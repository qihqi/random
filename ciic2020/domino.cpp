#define NDEBUG
#include <cassert>
#include <map>
#include <iostream>
#include <vector>
#include <iterator>

using namespace std;

const int64_t MOD = 1000000007;

// true == blanco
struct Table {
    vector<bool> cells;
    Table(int n) : cells(2*n, true) {}
    bool get(int i, int j) const {
        return cells[j * 2 + i];
    }

    bool flip(int i, int j) {
        cells[j * 2 + i] = !cells[j*2 + i];
        return cells[j*2 + i];
    }

    int num_blancos(int j) const {
        return get(0, j) + get(1, j);
    }

    int signo(int j) const {
        int total = 0;
        if (get(0, j)) {
            total += j;
        }
        if (get(1, j)) {
            total += j + 1;
        }
        return total % 2;
    }
};

void tipo1(Table& table, map<int, int>& intervals, int i, int j) {
    auto int_iter = intervals.upper_bound(j);
    int num_blancos = table.num_blancos(j);
    int min_col = intervals.begin()->first;
    if (int_iter == intervals.end() || int_iter->first != min_col) {
        --int_iter;
        int start = int_iter->first;
        int end = int_iter->second;

        // dentro de algun intervalo
        if (start < j && end > j) {
            assert(num_blancos == 2);
            intervals.erase(int_iter);
            intervals[start] = j;
            intervals[j] = end;
        } else if (start == j) {
            if (j == min_col) {
                if (table.get(i, j) && num_blancos == 1) {
                    // blanco -> negro
                    intervals.erase(int_iter);
                    intervals[j + 1] = end;
                } else {
                    // negro -> blanco
                    // nada cambia
                }
            } else {
                auto anterior = prev(int_iter);
                if (j == anterior->second) {
                    // fusionar
                    if (!table.get(i, j) && num_blancos == 1) {
                        // negro a blanco
                        int new_start = anterior->first;
                        intervals.erase(int_iter);
                        intervals.erase(anterior);
                        intervals[new_start] = end;
                    } else {
                        // un negro -> 2 negros
                        intervals.erase(int_iter);
                        intervals[j + 1] = end;
                        anterior->second--;
                    }
                }
            }
        } else if (j == end) {
            auto posterior = next(int_iter);
            if (posterior == intervals.end()) {
                if (table.get(i, j) && num_blancos == 1) {
                    // blanco -> negro
                    int_iter->second--;
                } else {
                    // negro -> blanco
                    // nada cambia
                }
            }
        } else {
            // 2 negros
            if (j == end + 1) {
                if ( table.get(0, end) && table.get(1, end)) {
                    // unir
                    int_iter->second++;
                } else {
                    intervals[end] = j;
                }
            }
            auto posterior = next(int_iter);
            if (posterior == intervals.end() || posterior->first != j + 1) {
                intervals[j] = j;
            } else {
                if (table.get(0, j + 1) && table.get(1, j + 1)) {
                    int newend = posterior->second;
                    intervals.erase(posterior);
                    intervals[j] = newend;
                } else {
                    intervals[j] = posterior->first;
                }
            }
        }
    } else {
        intervals[j] = j;
    }
    table.flip(i, j);
}


void fib(int x, vector<int64_t>* ans) {
    (*ans)[0] = 1;
    (*ans)[1] = 1;
    for (int i = 2; i <= x; i++) {
        (*ans)[i] = ((*ans)[i - 1] + (*ans)[i - 2]) % MOD;
    }
}

int64_t tipo2(
        const vector<int64_t>& fibs,
        const Table& table,
        const map<int, int>& intervals, int desde, int hasta) {

    int min_col = intervals.begin()->first;
    auto desde_iter = intervals.upper_bound(desde);
    auto hasta_iter = intervals.upper_bound(hasta);
    if (hasta_iter== intervals.end() || hasta_iter->first != min_col) {
        --hasta_iter;
    }
    if (desde_iter == intervals.end() || desde_iter->first != min_col) {
        --desde_iter;
    }
    if (hasta_iter->second < hasta) {
        hasta = hasta_iter->second;
    }

    int start = desde;
    int end = desde_iter->second;
    if (end > hasta) {
        end = hasta;
    }
    if (table.num_blancos(start) == 2 && table.num_blancos(end) != 2) {
        end -= 1;
    }
    int64_t accumulador = 1;

    while (start <= hasta) {
        // processar el intervalo
        if (table.num_blancos(start) == 2) {
            if (table.num_blancos(end) == 2) {
                accumulador *= fibs[end - start + 1];
                accumulador = accumulador % MOD;
            } else {
                return 0;
            }
        } else if (table.num_blancos(start) == 1) {
            if (table.signo(start) == table.signo(end)) {
                return 0;
            }
        }

        start = end + 1;
        if (start < desde_iter->second) {
            end = desde_iter->second;
        } else {
            ++desde_iter;
            if (desde_iter == intervals.end()) {
                break;
            }
            if (start < desde_iter->first) {
                start = desde_iter->first;
            }
            end = desde_iter->second;

        }
        if (end > hasta) {
            end = hasta;
        }
        if (table.num_blancos(start) == 2
                && table.num_blancos(end) != 2) {
            end -= 1;
        }
    }
    return accumulador;
}


int main() {
    int n, q;
    cin >> n >> q;
    Table table(n);
    map<int, int> intervals;
    intervals[0] = n - 1;
    vector<int64_t> fibs(n + 1);
    fib(n, &fibs);

    for (int i = 0; i < q; i++) {
        int t, x, y;
        cin >> t >> x >> y;
        if (t == 1) {
            tipo1(table, intervals, x - 1, y - 1);
        } else if (t == 2) {
            cout << tipo2(fibs, table, intervals, x - 1, y - 1) << endl;
        }
    }
}
