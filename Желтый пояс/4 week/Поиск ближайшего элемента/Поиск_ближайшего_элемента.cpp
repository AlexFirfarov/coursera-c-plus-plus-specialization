#include <iostream>
#include <set>

std::set<int>::const_iterator FindNearestElement(
    const std::set<int>& numbers,
    int border) {
        if (numbers.empty()) {
            return cend(numbers);
        } 
        auto [lower_it, upper_it] = numbers.equal_range(border);
        if (lower_it != upper_it || lower_it == begin(numbers)) {
            return lower_it;
        }
        if (lower_it == end(numbers)) {
            return prev(lower_it);
        }
        return border - *(prev(lower_it)) <= *lower_it - border ? prev(lower_it) : lower_it;
}


int main() {
  std::set<int> numbers = {1, 4, 6};
  std::cout <<
      *FindNearestElement(numbers, 0) << " " <<
      *FindNearestElement(numbers, 3) << " " <<
      *FindNearestElement(numbers, 5) << " " <<
      *FindNearestElement(numbers, 6) << " " <<
      *FindNearestElement(numbers, 100) << std::endl;
      
  std::set<int> empty_set;
  
  std::cout << (FindNearestElement(empty_set, 8) == end(empty_set)) << std::endl;
  return 0;
}