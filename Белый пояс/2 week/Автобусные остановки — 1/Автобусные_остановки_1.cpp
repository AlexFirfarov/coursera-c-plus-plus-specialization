#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

int main() {
    int q;
    std::cin >> q;
    std::map<std::string, std::vector<std::string>> bus_stop;
    std::map<std::string, std::vector<std::string>> stop_bus;
    while(q--) {
        std::string command;
        std::cin >> command;
        if (command == "NEW_BUS") {
            std::string bus;
            int stop_count;
            std::cin >> bus >> stop_count;
            for (int i = 0; i < stop_count; ++i) {
                std::string stop;
                std::cin >> stop;
                bus_stop[bus].push_back(stop);
                stop_bus[stop].push_back(bus);
            }
        } else if (command == "BUSES_FOR_STOP") {
            std::string stop;
            std::cin >> stop;
            if (stop_bus.count(stop)) {
                for (const std::string& bus: stop_bus[stop]) {
                    std::cout << bus << " ";
                }
                std::cout << '\n';
            } else {
                std::cout << "No stop\n";
            }
        } else if (command == "STOPS_FOR_BUS") {
            std::string bus;
            std::cin >> bus;
            if (bus_stop.count(bus)) {
                for (const std::string& stop: bus_stop[bus]) {
                    std::cout << "Stop " << stop << ": ";
                    if (stop_bus.count(stop) && stop_bus[stop].size() > 1) {
                        for (const std::string& bs: stop_bus[stop]) {
                            if (bs != bus) {
                                std::cout << bs << " ";
                            }
                        }
                    } else {
                        std::cout << "no interchange";
                    }
                    std::cout << '\n';
                }
            } else {
                std::cout << "No bus\n";
            }
        } else if (command == "ALL_BUSES") {
            if (bus_stop.size()) {
                std::vector<std::string> buses;
                for (const auto& el: bus_stop) {
                    buses.push_back(el.first);
                }
                std::sort(begin(buses), end(buses));
                for (const std::string& bus: buses) {
                    std::cout << "Bus " << bus << ": ";
                    for (const std::string& stop: bus_stop[bus]) {
                        std::cout << stop << " ";
                    }
                    std::cout << "\n";
                }
            } else {
                std::cout << "No buses\n";
            }
        }
    } 
}