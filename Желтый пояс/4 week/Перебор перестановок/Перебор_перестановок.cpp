#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T> v) {
    bool first = true;
    for (const T& el : v) {
        if (!first) {
            stream << ' ';
        }
        stream << el;
        first = false;
    }
    return stream;
}

int main() {
    int N;
    std::cin >> N;

    std::vector<int> nums(N);
    for (int i = 0; i < N; ++i) {
        nums[i] = N - i;
    }

    do {
        std::cout << nums << "\n";
    } while (std::prev_permutation(begin(nums), end(nums)));
}   