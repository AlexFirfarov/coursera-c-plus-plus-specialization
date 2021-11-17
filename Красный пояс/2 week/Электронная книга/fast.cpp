#include <iomanip>
#include <iostream>
#include <utility>
#include <functional>
#include <map>
#include <set>

class ReadingManager {
public:
    ReadingManager() = default;

    void Read(int user_id, int page_count) {
        auto it = user_page_counts.find(user_id);

        if (it != user_page_counts.end()) {
            size_t prev_page_counts = it->second;
            auto prev_it = page_user_counts.find(prev_page_counts);
            if (prev_it->second == 1) {
                page_user_counts.erase(prev_it);
            } else {
                --(prev_it->second);
            }
        }

        user_page_counts[user_id] = page_count;
        ++page_user_counts[page_count];
    }

    double Cheer(int user_id) const {
        auto user_it = user_page_counts.find(user_id);

        if (user_it == user_page_counts.end()) {
            return 0;
        }
        if (user_page_counts.size() == 1) {
            return 1;
        }

        auto pages_it = page_user_counts.find(user_it->second);

        size_t users_count = 0;
        for (auto it = pages_it; it != page_user_counts.begin();) {
            users_count += (--it)->second;
        }

        return 1.0 * users_count / (user_page_counts.size() - 1);
    }


private:
    std::map<size_t, size_t> user_page_counts;
    std::map<size_t, size_t> page_user_counts;
};  

int main() {
    // Для ускорения чтения данных отключается синхронизация
    // cin и cout с stdio,
    // а также выполняется отвязка cin от cout
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    ReadingManager manager;

    int query_count;
    std::cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        std::string query_type;
        std::cin >> query_type;
        int user_id;
        std::cin >> user_id;

        if (query_type == "READ") {
            int page_count;
            std::cin >> page_count;
            manager.Read(user_id, page_count);
        } else if (query_type == "CHEER") {
            std::cout << std::setprecision(6) << manager.Cheer(user_id) << '\n';
        }
    }

    return 0;
}