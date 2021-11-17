#pragma once

#include "date.h"

#include <map>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>
#include <unordered_set>
#include <set>

class Database {
public:
    Database();

    void Add(const Date& date, const std::string& event);

    size_t RemoveIf(const std::function<bool(const Date&, const std::string&)>& predicate);

    void Print(std::ostream& stream) const;

    std::vector<std::pair<Date, std::string>> FindIf(const std::function<bool(const Date&, const std::string&)>& predicate) const;

    std::pair<Date, std::string> Last(const Date& date) const;

private:
    std::map<Date, std::vector<std::string>> date_event;
    std::map<Date, std::unordered_set<std::string>> date_event_set;
};

std::ostream& operator<<(std::ostream& stream, const std::pair<Date, std::string>& date_event_pair);