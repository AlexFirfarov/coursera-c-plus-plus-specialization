#include <iostream>
#include <string>
#include <map>
#include <set>
#include <queue>

class HotelBooking {
public:
    HotelBooking() = default;

    void Book(const std::string& hotel_name, long long time, size_t client_id, size_t room_count) {
        last_time = time;
        long long border = last_time - seconds_per_day;

        requests.push(Reservation(hotel_name, time, client_id, room_count));
        clients[hotel_name][client_id] += room_count;
        hotel_room_count[hotel_name] += room_count;

        while (!requests.empty() && requests.front().time <= border) {
            Reservation& top = requests.front();
            auto it = clients[top.hotel_name].find(top.client_id);
            if (it->second == top.room_count) {
                clients[top.hotel_name].erase(it);
            } else {
                it->second -= top.room_count;
            }
            hotel_room_count[top.hotel_name] -= top.room_count;
            requests.pop();
        }
    }

    size_t Clients(const std::string& hotel_name) {
        return clients[hotel_name].size();
    }

    size_t Rooms(const std::string& hotel_name)  {
        return hotel_room_count[hotel_name];
    }

private:
    class Reservation {
    public:
        Reservation(const std::string& hotel_name, long long time, size_t client_id, size_t room_count) 
            : hotel_name(hotel_name), time(time), client_id(client_id), room_count(room_count) {}

        const std::string hotel_name;
        const long long time;
        const size_t client_id;
        const size_t room_count;
    };

    std::queue<Reservation> requests;
    std::map<std::string, size_t> hotel_room_count;
    std::map<std::string, std::map<size_t, size_t>> clients;
    long long last_time;

    const long long seconds_per_day = 86400;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    HotelBooking hb;

    int query_count;
    std::cin >> query_count;

    std::string query_type, hotel_name;
    for (int query_id = 0; query_id < query_count; ++query_id) {
        std::cin >> query_type;

        if (query_type == "BOOK") {
            long long time;
            size_t client_id;
            size_t room_count;

            std::cin >> time >> hotel_name >> client_id >> room_count;
            hb.Book(hotel_name, time, client_id, room_count);
        } else if (query_type == "CLIENTS") {
            std::cin >> hotel_name;
            std::cout << hb.Clients(hotel_name) << '\n';
        } else if (query_type == "ROOMS") {
            std::cin >> hotel_name;
            std::cout << hb.Rooms(hotel_name) << '\n';
        }
    }

    return 0;      
}