#include <iostream>
#include <string>
#include <deque>

int main() {
    std::ios_base::sync_with_stdio(false);

    long long first_number;
    size_t N;

    std::cin >> first_number >> N;
    std::deque<std::string> equation_deque(1, std::to_string(first_number));

    std::string op;
    long long value;
    bool high_pr = true;

    for (size_t i = 0; i < N; ++i) {
        std::cin >> op >> value;
        if (op == "+" || op == "-") {
            high_pr = false;
        } else if (!high_pr) {
            equation_deque.emplace_front("(");
            equation_deque.emplace_back(")");
            high_pr = true;
        }
        equation_deque.emplace_back(" " + op + " " + std::to_string(value));
    }
    for (const std::string& str : equation_deque) {
        std::cout << str;
    }
    std::cout << "\n";
}