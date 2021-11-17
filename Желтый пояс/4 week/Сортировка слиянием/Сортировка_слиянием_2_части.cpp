#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    int size = std::distance(range_begin, range_end);

    if (size < 2) 
        return;
    
    std::vector<typename RandomIt::value_type> elements(range_begin, range_end);
    MergeSort(begin(elements), begin(elements) + size / 2);
    MergeSort(begin(elements) + size / 2, end(elements));

    std::merge(
        begin(elements), begin(elements) + size / 2,
        begin(elements) + size / 2, end(elements),
        range_begin
    );
}
