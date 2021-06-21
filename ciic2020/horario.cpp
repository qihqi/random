#include <iostream>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <utility>

using namespace std;

struct Bus {
    int desde;
    int hasta;
    int64_t tiempo_recorrido;
    int64_t tiempo_salida;
    int64_t intervalo;

    int64_t tiempo_salida_para_llegar_antes_de(
            int64_t tiempo_llegada) {
        if (tiempo_salida + tiempo_recorrido > tiempo_llegada) {
            return -1;
        }
        int64_t k = (tiempo_llegada - tiempo_salida - tiempo_recorrido) / intervalo;
        return tiempo_salida + intervalo * k;
    }
};

struct Estacion {
    int num;
    vector<int> salidas;
    vector<int> entrandes;
};


int main() {
    vector<Bus> buses;
    int num_paradas, num_buses;
    int64_t tiempo_llegada;
    cin >> num_paradas >> num_buses >> tiempo_llegada;

    vector<Estacion> estaciones(num_paradas);


    for (int i = 0; i < num_buses; i++) {
        Bus bus;
        cin >> bus.desde >> bus.hasta >> bus.tiempo_recorrido
            >> bus.tiempo_salida >> bus.intervalo;
        estaciones[bus.desde].salidas.push_back(i);
        estaciones[bus.hasta].entrandes.push_back(i);
        buses.emplace_back(std::move(bus));
    }


    // busqueda de amplitud
    //
    // first es donde estoy ahorita; second es el tiempo
    // q debo llegar
    queue<pair<int, int64_t>> q;

    unordered_map<int, int64_t> estacion_salida;

    q.emplace(num_paradas - 1, tiempo_llegada);

    while (!q.empty()) {
        auto current = q.front();
        q.pop();
        for (int bus_id : estaciones[current.first].entrandes) {
            auto& bus = buses[bus_id];
            int64_t tiempo_a_subir =
                bus.tiempo_salida_para_llegar_antes_de(current.second);
            auto iter = estacion_salida.find(bus.desde);
            if (iter == estacion_salida.end() || iter->second < tiempo_a_subir) {
                estacion_salida[bus.desde] = tiempo_a_subir;
                q.emplace(bus.desde, tiempo_a_subir);
            }
        }
    }

    auto iter = estacion_salida.find(0);
    if (iter == estacion_salida.end() || iter->second < 0) {
        cout << -1 << endl;
    } else {
        cout << iter->second << endl;
    }

}
