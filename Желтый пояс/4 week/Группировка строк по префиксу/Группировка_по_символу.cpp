#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

template <typename RandomIt>
std::pair<RandomIt, RandomIt> FindStartsWith(
    RandomIt range_begin, RandomIt range_end,
    char prefix) {

    std::string prefix_str(1, prefix);
    auto comp = [](const std::string first, const std::string second) {
        return first[0] < second[0];
    }; 
    return std::equal_range(range_begin, range_end, prefix_str, comp);
}


int main() {
  const std::vector<std::string> sorted_strings = {"moscow", "murmansk", "vologda"};
  
  const auto m_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), 'm');
  for (auto it = m_result.first; it != m_result.second; ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
  
  const auto p_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), 'p');
  std::cout << (p_result.first - begin(sorted_strings)) << " " <<
      (p_result.second - begin(sorted_strings)) << std::endl;
  
  const auto z_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), 'z');
  std::cout << (z_result.first - begin(sorted_strings)) << " " <<
      (z_result.second - begin(sorted_strings)) << std::endl;
  
  return 0;
}
