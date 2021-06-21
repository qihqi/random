#include<iostream>
#include<vector>
#include<climits>
#include<cassert>
#include<utility>
#include<unordered_set>

using namespace std;


struct StationInfo {
    int num = 0;
    int train_time = 0;
    int walk_time = 0;
    int min_time = 0;
    int min_time_so_far = -1;
    bool in_get_min = false;
    vector<StationInfo*> downstream;
    vector<StationInfo*> upstream;

    bool is_dirty;
    // return true if min_time modified
    void update_train_time(int new_time) {
        int old_min = min_time;
        train_time = new_time;
        min_time = std::min(train_time, walk_time);
        if (min_time != old_min) {
            for (auto* s : downstream) {
                s->mark_dirty();
            }
        }
    }

    void mark_dirty() {
        if (!is_dirty) {
            is_dirty = true;
            for (auto* s : downstream) {
                s->mark_dirty();
            }
        }
    }

    int get_min(unordered_set<int>& current_in_stack) {
        in_get_min = true;
        if (is_dirty) {
            int wtime = INT_MAX;
            current_in_stack.insert(num);
            for (auto* s : upstream) {
                if (current_in_stack.find(s->num) !=
                    current_in_stack.end()) {
                    continue;
                }
                wtime = min(wtime, s->get_min(current_in_stack) + 1);
                min_time_so_far = wtime;
            }
            walk_time = wtime;
            min_time = min(walk_time, train_time);
            is_dirty = false;
            current_in_stack.erase(num);
        }


        return min_time;
    }
};

void update_min_time(
        vector<StationInfo>& stations,
        int s1, int s2) {
    assert(s1 < stations.size());
    assert(s2 < stations.size());
    int s1_time = stations[s1].train_time;
    int s2_time = stations[s2].train_time;

    stations[s1].update_train_time(s2_time);
    stations[s2].update_train_time(s1_time);
}


int main() {
    int n, w, d;
    cin >> n >> w >> d;
    vector<StationInfo> stations(n + 1);
    vector<int> in_route(n);

    for (int i = 0; i < w; i++) {
        int from, to;
        cin >> from >> to;
        stations[from].downstream.push_back(
                &stations[to]);
        stations[to].upstream.push_back(
                &stations[from]);
    }

    for (int i = 0; i < n; i++) {
        int s;
        cin >> s;
        stations[s].num = s;
        stations[s].train_time = i;
        stations[s].is_dirty = true;
        in_route[i] = s;
    }

    for (int i = 0; i < d; i++) {
        int s1, s2;
        cin >> s1 >> s2;
        assert(s1 <= n);
        assert(s2 <= n);
        update_min_time(stations, in_route[s1 - 1], in_route[s2 - 1]);
        swap(in_route[s1 - 1], in_route[s2 - 1]);
        unordered_set<int> stack;
        cout << stations[n].get_min(stack) << endl;
    }

    return 0;
}

