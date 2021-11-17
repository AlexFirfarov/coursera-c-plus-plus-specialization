#include <algorithm>

template <typename T>
void RemoveDuplicates(vector<T>& elements) {
    std::sort(begin(elements), end(elements));
    elements.erase(
        std::unique(begin(elements), end(elements)), 
        end(elements));
}

int main() {
    
}