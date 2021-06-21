#include <iostream>
#include <string>
#include <string_view>
#include <unordered_set>
#include <set>
#include <unordered_map>


using namespace std;


unordered_set<string> all_names;

string_view intern(string res) {
    auto p = all_names.insert(std::move(res));
    return *p.first;
}

int main() {
    int n;
    cin >> n;
    unordered_map<string_view, unordered_set<string_view>> padre_a_hijo;
    unordered_map<string_view, unordered_set<string_view>> abu_a_nieto;
    for (int i = 0; i < n; i++) {
        string hijo, padre;
        cin >> hijo >> padre;
        padre_a_hijo[intern(padre)].insert(intern(hijo));
    }

    set<string_view> mayor;
    int mayor_nietos = 0;
    for (const auto& p : padre_a_hijo) {
        for (auto s : p.second) {
            const auto& hh = padre_a_hijo[s];
            abu_a_nieto[p.first].insert(hh.begin(), hh.end());
        }
        for (auto s : p.second) {
            abu_a_nieto[p.first].erase(s);
        }
        int nietos = abu_a_nieto[p.first].size();
        if (mayor_nietos < nietos) {
            mayor = {p.first};
            mayor_nietos = nietos;
        } else if (mayor_nietos == nietos) {
            mayor.insert(p.first);
        }
    }
    for (auto s : mayor) {
        cout << s << " ";
    }
    cout << endl;
}
