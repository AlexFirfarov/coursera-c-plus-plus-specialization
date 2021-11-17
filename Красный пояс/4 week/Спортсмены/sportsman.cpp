#include <iostream>
#include <algorithm>
#include <list>
#include <unordered_map>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t n, sportsman_num, prev_sportsman_num;
    std::cin >> n;

    std::list<size_t> sportsmans;
    std::unordered_map<size_t, std::list<size_t>::iterator> list_iterators;
    for (size_t i = 0; i < n; ++i) {
        std::cin >> sportsman_num >> prev_sportsman_num;

        std::list<size_t>::iterator it;
        if (list_iterators.count(prev_sportsman_num)) {
            it = list_iterators[prev_sportsman_num];
        } else {
            it = std::find(sportsmans.begin(), sportsmans.end(), prev_sportsman_num);
            if (it != sportsmans.end()) {
                list_iterators[prev_sportsman_num] = it;
            }
        }
        list_iterators[sportsman_num] = sportsmans.insert(it, sportsman_num);
    }   

    for (size_t num : sportsmans) {
        std::cout << num << '\n';
    }

    return 0;
}