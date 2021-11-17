#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <sstream>
#include <chrono>
#include <optional>

using namespace std;

pair<string, string> SplitTwoStrict(string s, string delimiter) {
  const size_t pos = s.find(delimiter);
  if (pos == s.npos) {
    return {move(s), ""};
  } else {
    return {s.substr(0, pos), s.substr(pos + delimiter.length())};
  }
}

pair<string, string> SplitTwo(string s, string delimiter) {
  const auto [lhs, rhs] = SplitTwoStrict(move(s), move(delimiter));
  return {lhs, rhs};
}

string ReadToken(string& s, string delimiter = " ") {
  const auto [lhs, rhs] = SplitTwo(s, move(delimiter));
  s = rhs;
  return lhs;
}

int ConvertToInt(string str) {
  size_t pos;
  const int result = stoi(str, &pos);
  if (pos != str.length()) {
    stringstream error;
    error << "string " << str << " contains " << (str.length() - pos) << " trailing chars";
    throw invalid_argument(error.str());
  }
  return result;
}

class Date {
public:
  Date(int year, int month, int day)
    : year_(year), month_(month), day_(day) {}

  static Date FromString(string str) {
    const int year = ConvertToInt(ReadToken(str, "-"));
    const int month = ConvertToInt(ReadToken(str, "-"));
    const int day = ConvertToInt(move(str));
    return Date(year, month, day);
  }

  static int ComputeDaysDiff(const Date& date_to, const Date& date_from) {
    const time_t timestamp_to = date_to.AsTimestamp();
    const time_t timestamp_from = date_from.AsTimestamp();
    return (timestamp_to - timestamp_from) / SECONDS_IN_DAY;
  }

  time_t AsTimestamp() const {
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
  static constexpr int SECONDS_IN_DAY = 60 * 60 * 24;

  int year_;
  int month_;
  int day_;
};

class BudgetManager {
public:
  BudgetManager(const Date& start_date = Date::FromString(MIN_START_DATE_STR), 
                const Date& end_date = Date::FromString(MAX_END_DATE_STR))
    : start_date_(start_date),
      end_date_(end_date),
      money_(Date::ComputeDaysDiff(end_date_, start_date_), .0) {}

  double ComputeIncome(const Date& from, const Date& to) const {
    const auto [start_indx, finish_indx] = GetIndexes(from, to);
    return accumulate(money_.begin() + start_indx, money_.begin() + finish_indx + 1, 0.);
  }

  void AddEarnedMoney(const Date& from, const Date& to, double value) {
    const auto [start_indx, finish_indx] = GetIndexes(from, to);
    value /= Date::ComputeDaysDiff(to, from) + 1;
    for (int i = start_indx; i <= finish_indx; ++i) {
      money_[i] += value;
    }
  }

  void PayTax(const Date& from, const Date& to) {
    const auto [start_indx, finish_indx] = GetIndexes(from, to);
    for (int i = start_indx; i <= finish_indx; ++i) {
      money_[i] *= (1 - TAX);
    }
  }

private:
  pair<int, int> GetIndexes(const Date& from, const Date& to) const {
    auto start_indx = Date::ComputeDaysDiff(from, start_date_);
    auto finish_indx = Date::ComputeDaysDiff(to, start_date_);
    return {start_indx, finish_indx};
  }

  inline static const string MIN_START_DATE_STR = "2000-01-01";
  inline static const string MAX_END_DATE_STR = "2100-01-01";
  inline static const double TAX = 0.13;

  const Date start_date_;
  const Date end_date_;

  vector<double> money_;
};

int main() {
  cout.precision(25);
  BudgetManager budget;
  size_t num_queries;
  cin >> num_queries;

  for (size_t i = 0; i < num_queries; ++i) {
    string command, date_from, date_to;
    cin >> command >> date_from >> date_to;
    Date from = Date::FromString(date_from);
    Date to = Date::FromString(date_to);

    if (command == "Earn") {
      double value;
      cin >> value;
      budget.AddEarnedMoney(from, to, value);
    } else if (command == "ComputeIncome") {
      cout << budget.ComputeIncome(from, to) << '\n';
    } else {
      budget.PayTax(from, to);
    }
  }

  return 0;
}