#include "database.h"

Database::Database() {}

void Database::Add(const Date& date, const std::string& event) {
    if (date_event_set[date].count(event)) {
        return;
    }
    date_event_set[date].insert(event);
    date_event[date].push_back(event);

    return;
}

size_t Database::RemoveIf(const std::function<bool(const Date&, const std::string&)>& predicate) {
    size_t count = 0;
    std::vector<Date> to_erase;

    for (auto& [date, events] : date_event) {
        std::vector<std::string>::iterator begin_it = events.begin();
        std::vector<std::string>::iterator end_it = events.end();
        std::vector<std::string>::iterator it = std::stable_partition(begin_it, end_it, [&predicate, &date=date](const std::string& event) {
            return !predicate(date, event);
        });
        count += std::distance(it, end_it);
        
        if (it == begin_it) {
            to_erase.push_back(date);
        } else {
            events.resize(std::distance(begin_it, it));
            date_event_set[date] = std::unordered_set(events.begin(), events.end());
        }
    }

    for (const Date& date : to_erase) {
        date_event.erase(date);
        date_event_set.erase(date); 
    }

    return count;
}

void Database::Print(std::ostream& stream) const {
    for (const auto& [date, events] : date_event) {
        for (const std::string& event : events) {
            stream << date << ' ' << event << '\n';
        }
    }

    return;
}

std::vector<std::pair<Date, std::string>> Database::FindIf(const std::function<bool(const Date&, const std::string&)>& predicate) const {
    std::vector<std::pair<Date, std::string>> result;

    for (const auto& [date, events] : date_event) {
        for (const std::string& event : events) {
            if (predicate(date, event)) {
                result.emplace_back(date, event);
            }
        }
    }

    return result;
}

std::pair<Date, std::string> Database::Last(const Date& date) const {
    auto it = date_event.upper_bound(date);
    if (it == date_event.begin()) {
        throw std::invalid_argument("");
    }

    --it;
    std::pair<Date, std::string> result(it->first, it->second.back());
    return result;
}

std::ostream& operator<<(std::ostream& stream, const std::pair<Date, std::string>& date_event_pair) {
    stream << date_event_pair.first << ' ' << date_event_pair.second;

    return stream;
}