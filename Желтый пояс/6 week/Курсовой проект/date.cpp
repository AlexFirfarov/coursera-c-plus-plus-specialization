#include "date.h"



bool operator<(const Date& lhs, const Date& rhs) {
    return std::tuple{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} <
           std::tuple{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

bool operator<=(const Date& lhs, const Date& rhs) {
    return std::tuple{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} <=
           std::tuple{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

bool operator>(const Date& lhs, const Date& rhs) {
    return std::tuple{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} >
           std::tuple{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

bool operator>=(const Date& lhs, const Date& rhs) {
    return std::tuple{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} >=
           std::tuple{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

bool operator==(const Date& lhs, const Date& rhs) {
    return std::tuple{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} ==
           std::tuple{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

bool operator!=(const Date& lhs, const Date& rhs) {
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& stream, const Date& date) {
    stream << std::setw(4) << std::setfill('0') << date.GetYear() <<
       "-" << std::setw(2) << std::setfill('0') << date.GetMonth() <<
       "-" << std::setw(2) << std::setfill('0') << date.GetDay();
    return stream;
}

Date ParseDate(std::istream& date_stream) {
    int year, month, day;

    bool ok = true;

    ok = ok && (date_stream >> year);
    ok = ok && (date_stream.peek() == '-');
    date_stream.ignore(1);

    ok = ok && (date_stream >> month);
    ok = ok && (date_stream.peek() == '-');
    date_stream.ignore(1);

    ok = ok && (date_stream >> day);

    if (!ok) {
        throw std::logic_error("Wrong date format ");
    }

    return Date(year, month, day);
}
