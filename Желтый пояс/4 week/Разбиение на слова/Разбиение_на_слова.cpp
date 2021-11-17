#include <algorithm>
#include <string>
#include <vector>

std::vector<std::string> SplitIntoWords(const std::string& str) {

    std::string::const_iterator str_begin = cbegin(str);
    std::string::const_iterator str_end = cend(str); 

    std::vector<std::string> result;

    while (true) {
        std::string::const_iterator it = std::find(str_begin, str_end, ' ');
        result.push_back(std::string(str_begin, it));
        if (it == str_end) {
            break;
        }
        str_begin = it + 1;
    }
    return result;
}

