#include <algorithm>

template <class ForwardIterator, class UnaryPredicate>
ForwardIterator max_element_if(ForwardIterator first, ForwardIterator last, UnaryPredicate p) {
    ForwardIterator maxElemIt = std::find_if(first, last, p);
    for (ForwardIterator cur = maxElemIt; cur != last; ++cur) {
        if (cur != maxElemIt && p(*cur) && *maxElemIt < *cur) {
            maxElemIt = cur;
        }
    }
    return maxElemIt;
}