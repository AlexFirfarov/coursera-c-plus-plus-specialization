#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <chrono>

class Date {
public:

    Date(int year, int month, int day)
        : year_(year), month_(month), day_(day) {}

    Date(const std::tuple<int, int, int>& data_tuple)
        : year_(std::get<0>(data_tuple)) , month_(std::get<1>(data_tuple)), day_(std::get<2>(data_tuple)) {}

    time_t Timestamp() const {
        tm t;
        t.tm_sec  = 0;
        t.tm_min  = 0;
        t.tm_hour = 0;
        t.tm_mday = day_;
        t.tm_mon  = month_ - 1;
        t.tm_year = year_ - 1900;
        t.tm_isdst = 0;
        return mktime(&t);
    }

private:
    int year_;
    int month_;
    int day_;

};

std::tuple<int, int, int> ParseDate(const std::string& data) {
    int year, month, day;

    size_t first_delimiter_pos = data.find('-');
    size_t second_delimiter_pos = data.rfind('-');

    year = std::stoi(data.substr(0, first_delimiter_pos));
    month = std::stoi(data.substr(first_delimiter_pos + 1, second_delimiter_pos - first_delimiter_pos - 1));
    day = std::stoi(data.substr(second_delimiter_pos + 1));

    return std::make_tuple(year, month, day);
}

int DaysDiff(const Date& to, const Date& from) {
    const time_t timestamp_to = to.Timestamp();
    const time_t timestamp_from = from.Timestamp();
    return (timestamp_to - timestamp_from) / (60 * 60 * 24);
}

const Date START_DATE(ParseDate("1700-01-01"));
const Date END_DATE(ParseDate("2100-01-01"));
const size_t DAY_COUNT = DaysDiff(END_DATE, START_DATE);

int main() {
    std::cout.precision(25);
    std::vector<std::uint64_t> money(DAY_COUNT, 0);
    std::vector<std::uint64_t> partial_sums(DAY_COUNT, 0);
    
    size_t q, e;
    std::cin >> e;

    std::string date, date_from, date_to;
    std::uint64_t value;

    for (size_t i = 0; i < e; ++i) {
        std::cin >> date >> value;
        size_t indx = DaysDiff(ParseDate(date), START_DATE);
        money[indx] += value;
    }

    std::partial_sum(begin(money), end(money), begin(partial_sums));
    std::cin >> q;

    for (size_t i = 0; i < q; ++i) {
        std::cin >> date_from >> date_to;
        Date from(ParseDate(date_from));
        Date to(ParseDate(date_to));

        size_t start_indx = DaysDiff(from, START_DATE);
        size_t end_indx = DaysDiff(to, START_DATE);

        std::cout << partial_sums[end_indx] - (start_indx > 0 ? partial_sums[start_indx - 1] : 0) << "\n";
    }

    return 0;
}