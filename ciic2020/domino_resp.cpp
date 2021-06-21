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

class Backtracking {
    Table table;

public:
    Backtracking(int n):table(n) {}
    void tipo1(int i, int j) {
        table.flip(i, j);
    }

    int64_t tipo2(int desde, int hasta) {
        // caso 1: poner vertical
        if (desde == hasta) {
            if (table.num_blancos(desde) == 2 ||
                table.num_blancos(desde) == 0) {
                return 1;
            }
            return 0;
        }
        if (table.get(0, desde)) {
            int64_t total = 0;
            if (table.num_blancos(desde) == 2) {
                total += tipo2(desde + 1, hasta);
                total %= MOD;
            }
            if (table.get(0, desde) && table.get(0, desde + 1)) {
                if (table.get(1, desde) == table.get(1, desde + 1)) {
                    if (desde + 2 <= hasta) {
                        total += tipo2(desde + 2, hasta);
                    } else {
                        total += 1;
                    }
                } else if (!table.get(1, desde)) {
                    table.flip(0, desde + 1);
                    total += tipo2(desde + 1, hasta);
                    total %= MOD;
                    table.flip(0, desde + 1);
                }
            }
            return total;
        } else {
            if (table.get(1, desde)) {
                if (table.get(1, desde + 1)) {
                    table.flip(1, desde + 1);
                    int total = tipo2(desde + 1, hasta);
                    table.flip(1, desde + 1);
                    return total;
                }
            } else {
                return tipo2(desde + 1, hasta);
            }

        }
        return 0;
    }
};
// A = 2 blancos
// B = negro/blanco
// C = blanco/negro
// D = 2 negros
enum Tipo {
    A, B, C, D
};

class DP_Fibonacci {

    vector<Tipo> tabla;
    vector<int64_t> fibonaccis;


public:
    DP_Fibonacci(int n) : tabla(n), fibonaccis(n + 1) {
        fibonaccis[0] = 1;
        fibonaccis[1] = 1;
        for (int i = 2; i <= n; i++) {
            fibonaccis[i] = fibonaccis[i - 1] + fibonaccis[i - 2];
            fibonaccis[i] %= MOD;
        }
    }

    void tipo1(int i, int j) {
        // flip i = 0
        // A -> B, B -> A
        // C -> D, D -> C
        if (i == 0) {
            if (tabla[j] == A) {
                tabla[j] = B;
            }
            else if (tabla[j] == B) {
                tabla[j] = A;
            }
            else if (tabla[j] == C) {
                tabla[j] = D;
            }
            else if (tabla[j] == D) {
                tabla[j] = C;
            }
        } else {
            // A -> C, C -> A
            // B -> D, D -> B
            if (tabla[j] == A) {
                tabla[j] = C;
            }
            else if (tabla[j] == B) {
                tabla[j] = D;
            }
            else if (tabla[j] == C) {
                tabla[j] = A;
            }
            else if (tabla[j] == D) {
                tabla[j] = B;
            }
        }
    }

    int64_t tipo2(int desde, int hasta) {
        int64_t total = 1;
        int start = desde;
        int current = desde;
        while (current <= hasta) {
            if (tabla[current] == A) {
                while (tabla[current] == A && current < hasta) { current++; }
                // current != A
                int num_of_A = current - start;
                if (current == hasta && tabla[current] == A) {
                    num_of_A += 1;
                }
                total *= fibonaccis[num_of_A];
                total %= MOD;
                start = current;
                if (current == hasta && tabla[current] == A) {
                    return total;
                }
            } else if (tabla[current] == B) {
                if (current == hasta) {
                    return 0;
                }
                current += 1;
                while (tabla[current] == A && current < hasta) { current++; }
                if ((tabla[current] == B && ((current - start) % 2 == 1))
                    ||(tabla[current] == C && ((current - start) % 2 == 0))) {
                    current += 1;
                    start = current;
                } else {
                    return 0;
                }
            } else if (tabla[current] == C) {
                if (current == hasta) {
                    return 0;
                }
                current += 1;
                while (tabla[current] == A && current < hasta) { current++; }
                if ((tabla[current] == B && ((current - start) % 2 == 0))
                    ||(tabla[current] == C && ((current - start) % 2 == 1))) {
                    current += 1;
                    start = current;
                } else {
                    return 0;
                }
            } else if (tabla[current] == D) {
                current += 1;
                start = current;
            }
        }

        return total;
    }
};



class BlancoCompleto {
    // [intervalos a -> b]
    struct Segment {
        int begin;
        int end;
        int64_t middle;
        bool una_sola;
        static Segment neutro() {
           return Segment{
               .begin=0,
               .end=0,
               .middle=1,
               .una_sola=true,
           };
       } 
    };
    vector<Segment> segtree;
    vector<Tipo> tabla;
    int size;
    vector<int64_t> fibonaccis;
    
    Segment combinar(const Segment& s1, const Segment& s2) {
        if (s1.una_sola) {
            if (s2.una_sola) {
                return Segment{
                    .begin=s1.begin + s2.begin,
                    .end=0,
                    .middle=1,
                    .una_sola=true
                };
            } else {
                return Segment{
                    .begin=s1.begin + s2.begin,
                    .end=s2.end,
                    .middle=s2.middle,
                    .una_sola=false
                };
            }
        } else {
            if (s2.una_sola) {
                return Segment{
                    .begin=s1.begin,
                    .end=s1.end + s2.begin,
                    .middle=s1.middle,
                    .una_sola=false
                };
            } else {
                return Segment{
                    .begin=s1.begin,
                    .end=s2.end,
                    .middle=(((s2.middle * s1.middle) % MOD) * fibonaccis[s1.end + s2.begin]) % MOD,
                    .una_sola=false
                };
            }
        }
        return s1;
    }
    
    void modify(int p, const Segment& seg) {
        p += size;
        segtree[p] = seg;
        p /= 2;
        while (p > 0) {
           segtree[p] = combinar(segtree[2*p], segtree[2*p + 1]);
           p /= 2;
        }
    }

    Segment query(int l, int r) {
         Segment resl = Segment::neutro();
         Segment resr = Segment::neutro();
         l += size;
         r += size;
         while (l < r) {
             if (l % 2 == 1) {
                 resl = combinar(resl, segtree[l]);
                 l += 1;
             }
             if (r % 2 == 1) {
                 resr = combinar(segtree[r - 1], resr);
                 r--;
             }
             r /= 2;
             l /= 2;
         }
         return combinar(resl, resr);
    }
    
    int64_t producto(const Segment& seg) {
        if (seg.una_sola) {
            return fibonaccis[seg.begin];
        }
        else {
            return (((fibonaccis[seg.begin] * fibonaccis[seg.end]) % MOD) * seg.middle) % MOD;
        }
    }

public:
    BlancoCompleto(int n) : tabla(n), size(n), segtree(2 * n), fibonaccis(n + 1) {
        fibonaccis[0] = 1;
        fibonaccis[1] = 1;
        for (int i = 2; i <= n; i++) {
            fibonaccis[i] = fibonaccis[i - 1] + fibonaccis[i - 2];
            fibonaccis[i] %= MOD;
        }
        for (int i = n; i < 2*n; i++) {
            segtree[i] = Segment{
                .begin=1, 
                .end=0, 
                .middle=1, 
                .una_sola=true
            };
        }
        for (int i = n - 1; i > 0; i--) {
            segtree[i] = combinar(segtree[2*i], segtree[2*i + 1]);
        }
    }
    void tipo1(int i, int j) {
        // flip i = 0
        // A -> B, B -> A
        // C -> D, D -> C
        Tipo antes = tabla[j];
        if (i == 0) {
            if (tabla[j] == A) {
                tabla[j] = B;
            }
            else if (tabla[j] == B) {
                tabla[j] = A;
            }
            else if (tabla[j] == C) {
                tabla[j] = D;
            }
            else if (tabla[j] == D) {
                tabla[j] = C;
            }
        } else {
            // A -> C, C -> A
            // B -> D, D -> B
            if (tabla[j] == A) {
                tabla[j] = C;
            }
            else if (tabla[j] == B) {
                tabla[j] = D;
            }
            else if (tabla[j] == C) {
                tabla[j] = A;
            }
            else if (tabla[j] == D) {
                tabla[j] = B;
            }
        }
        
        if (tabla[j] == A) {
            // separar intervalo
            modify(j, Segment{
                .begin=1, .end=0, .middle=1, .una_sola=true});
        }
        if (tabla[j] == D) {
            modify(j, Segment{
                .begin=0, .end=0, .middle=1, .una_sola=false});
        }
    }

    int64_t tipo2(int desde, int hasta) {
        Segment s = query(desde, hasta + 1);
        return producto(s);
    }

};


class CasoGeneral {
    // [intervalos a -> b]
    struct Segment {
        int begin;
        int end;
        int64_t producto_par;
        int64_t producto_impar;
        // true = par
        bool paridad;
        bool una_sola;
        
        // estado[i] == estado que llegaria
        // si procesa la cadena comenzando desde i
        vector<int> estados;

        static Segment neutro() {
            return Segment{
                .begin=0,
                .end=0,
                .producto_par=1,
                .producto_impar=1,
                .paridad=true,
                .una_sola=true,
                .estados={0,1,2,3}
            };
        }
        
        static Segment A() {
            return Segment{
                .begin=1,
                .end=0,
                .producto_par=1,
                .producto_impar=1,
                .paridad=true,
                .una_sola=true,
                .estados={0,2,1,3},
            };
        }
        static Segment B() {
            return Segment{
                .begin=0,
                .end=0,
                .producto_par=1,
                .producto_impar=1,
                .paridad=false,
                .una_sola=false,
                .estados={1,0,3,3},
            };
        }
        static Segment C() {
            return Segment{
                .begin=0,
                .end=0,
                .producto_par=1,
                .producto_impar=1,
                .paridad=false,
                .una_sola=false,
                .estados={2,3,0,3},
            };
        }
        static Segment D() {
            return Segment{
                .begin=0,
                .end=0,
                .producto_par=1,
                .producto_impar=1,
                .paridad=true,
                .una_sola=false,
                .estados={0,3,3,3},
            };
        }

    };
    vector<Segment> segtree;
    vector<Tipo> tabla;
    int size;
    vector<int64_t> fibonaccis;
    
    vector<int> combinar_estados(const vector<int>& estado1, const vector<int>& estado2) {
        vector<int> resp(4);
        for (int i = 0; i < 4; i++){
            int first = estado1[i];
            int second = estado2[first];
            resp[i] = second;
            // resp[i] = estado2[estado1[i]];
        }
        return resp;
    }
    
    Segment combinar(const Segment& s1, const Segment& s2) {

        vector<int> estados = combinar_estados(
            s1.estados, s2.estados
        );
        if (s1.una_sola) {
            if (s2.una_sola) {
                return Segment{
                    .begin=s1.begin + s2.begin,
                    .end=0,
                    .producto_par=1,
                    .producto_impar=1,
                    .paridad=true,
                    .una_sola=true,
                    .estados=estados
                };
            } else {
                return Segment{
                    .begin=s1.begin + s2.begin,
                    .end=s2.end,
                    .producto_par=s2.producto_par,
                    .producto_impar=s2.producto_impar,
                    .paridad=s2.paridad,
                    .una_sola=false,
                    .estados=estados
                };
            }
        } else {
            if (s2.una_sola) {
                return Segment{
                    .begin=s1.begin,
                    .end=s1.end + s2.begin,
                    .producto_par=s1.producto_par,
                    .producto_impar=s1.producto_impar,
                    .paridad=s1.paridad,
                    .una_sola=false,
                    .estados=estados
                };
            } else {
                int64_t producto_par;
                int64_t producto_impar;
                
                if (s1.paridad) {
                    producto_par = (s1.producto_par * s2.producto_par) % MOD;
                    producto_par *= (fibonaccis[s1.end + s2.begin]);
                    producto_par %= MOD;
                    producto_impar = (s1.producto_impar * s2.producto_impar) % MOD;
                } else {
                    producto_par = (s1.producto_par * s2.producto_impar) % MOD;
                    producto_impar = (s1.producto_impar * s2.producto_par) % MOD;
                    producto_impar *= (fibonaccis[s1.end + s2.begin]);
                    producto_impar %= MOD;
                }

                return Segment{
                    .begin=s1.begin,
                    .end=s2.end,
                    .producto_par=producto_par,
                    .producto_impar=producto_impar,
                    .paridad=(s1.paridad == s2.paridad),
                    .una_sola=false,
                    .estados=estados
                };
            }
        }
        return s1;
    }
    
    void modify(int p, const Segment& seg) {
        p += size;
        segtree[p] = seg;
        p /= 2;
        while (p > 0) {
           segtree[p] = combinar(segtree[2*p], segtree[2*p + 1]);
           p /= 2;
        }
    }

    Segment query(int l, int r) {
         Segment resl = Segment::neutro();
         Segment resr = Segment::neutro();
         l += size;
         r += size;
         while (l < r) {
             if (l % 2 == 1) {
                 resl = combinar(resl, segtree[l]);
                 l += 1;
             }
             if (r % 2 == 1) {
                 resr = combinar(segtree[r - 1], resr);
                 r--;
             }
             r /= 2;
             l /= 2;
         }
         return combinar(resl, resr);
    }
    
    int64_t producto(const Segment& seg) {
        if (seg.estados[0] != 0) {
            return 0;
        }
        if (seg.una_sola) {
            return fibonaccis[seg.begin];
        }
        else {
            int64_t producto = seg.producto_par;
            producto *= fibonaccis[seg.begin];
            producto %= MOD;
            if (seg.paridad) {
                producto *= fibonaccis[seg.end];
                producto %= MOD;
            }
            return producto;
        }
    }

public:
    CasoGeneral(int n) : tabla(n), size(n), segtree(2 * n), fibonaccis(n + 1) {
        fibonaccis[0] = 1;
        fibonaccis[1] = 1;
        for (int i = 2; i <= n; i++) {
            fibonaccis[i] = fibonaccis[i - 1] + fibonaccis[i - 2];
            fibonaccis[i] %= MOD;
        }
        for (int i = n; i < 2*n; i++) {
            segtree[i] = Segment::A();
        }
        for (int i = n - 1; i > 0; i--) {
            segtree[i] = combinar(segtree[2*i], segtree[2*i + 1]);
        }
    }
    void tipo1(int i, int j) {
        // flip i = 0
        // A -> B, B -> A
        // C -> D, D -> C
        Tipo antes = tabla[j];
        Segment result = Segment::neutro();
        if (i == 0) {
            if (tabla[j] == A) {
                tabla[j] = B;
                result = Segment::B();
            }
            else if (tabla[j] == B) {
                tabla[j] = A;
                result = Segment::A();
            }
            else if (tabla[j] == C) {
                tabla[j] = D;
                result = Segment::D();
            }
            else if (tabla[j] == D) {
                tabla[j] = C;
                result = Segment::C();
            }
        } else {
            // A -> C, C -> A
            // B -> D, D -> B
            if (tabla[j] == A) {
                tabla[j] = C;
                result = Segment::C();
            }
            else if (tabla[j] == B) {
                tabla[j] = D;
                result = Segment::D();
            }
            else if (tabla[j] == C) {
                tabla[j] = A;
                result = Segment::A();
            }
            else if (tabla[j] == D) {
                tabla[j] = B;
                result = Segment::B();
            }
        }
        
        modify(j, result);
    }

    int64_t tipo2(int desde, int hasta) {
        Segment s = query(desde, hasta + 1);
        return producto(s);
    }

};


int main() {
    int n, q;
    cin >> n >> q;

    // subtarea 1
    // Backtracking domino(n);
    // subtarea 2
    // DP_Fibonacci domino(n);
    // subtarea 3
    // BlancoCompleto domino(n);
    // caso general
    CasoGeneral domino(n);

    for (int i = 0; i < q; i++) {
        int t, x, y;
        cin >> t >> x >> y;
        if (t == 1) {
            domino.tipo1(x - 1, y - 1);
        } else if (t == 2) {
            cout << domino.tipo2(x - 1, y - 1) << endl;
        }
    }
}
