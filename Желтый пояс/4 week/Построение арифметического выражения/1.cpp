#include <iostream>
#include <string>

int main() {
    int first_number;
    size_t N;

    std::cin >> first_number >> N;
    std::string equation(std::to_string(first_number));

    std::string op;
    int value;

    for (size_t i = 0; i < N; ++i) {
        equation.insert(begin(equation), '(');
        equation.insert(end(equation), ')');

        std::cin >> op >> value;
        equation += " " + op + " " + std::to_string(value); 
    }
    std::cout << equation << "\n";
}