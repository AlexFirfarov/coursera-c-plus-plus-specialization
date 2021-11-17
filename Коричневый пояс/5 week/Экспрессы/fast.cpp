#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <set>

class RouteManager {
public:
    RouteManager() = default;

    void AddRoute(int start, int finish) {
        reachable_lists_[start].insert(finish);
        reachable_lists_[finish].insert(start);
    }

    int FindNearestFinish(int start, int finish) const {
        int result = std::abs(start - finish);
        auto start_it = reachable_lists_.find(start);
        if (start_it == reachable_lists_.end() || start_it->second.empty()) {
            return result;
        }

        const std::set<int>& reachable_stations = start_it->second;

        auto lower_bound_it = reachable_stations.lower_bound(finish);

        if (lower_bound_it != reachable_stations.end()) {
            result = std::min(result, std::abs(finish - *lower_bound_it));
        } 
        if (lower_bound_it != reachable_stations.begin()) {
            result = std::min(result, std::abs(finish - *prev(lower_bound_it)));
        }

        return result;
    }

private:
    std::map<int, std::set<int>> reachable_lists_;
};

int main() {
    RouteManager routes;

    int query_count;
    std::cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        std::string query_type;
        std::cin >> query_type;
        int start, finish;
        std::cin >> start >> finish;
        if (query_type == "ADD") {
            routes.AddRoute(start, finish);
        } else if (query_type == "GO") {
            std::cout << routes.FindNearestFinish(start, finish) << "\n";
        }
    }

    return 0;
}