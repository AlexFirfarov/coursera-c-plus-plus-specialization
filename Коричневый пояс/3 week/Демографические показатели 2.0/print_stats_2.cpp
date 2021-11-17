#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <unordered_map>

using namespace std;

template <typename Iterator>
class IteratorRange {
public:
  IteratorRange(Iterator begin, Iterator end)
    : first(begin)
    , last(end)
  {
  }

  Iterator begin() const {
    return first;
  }

  Iterator end() const {
    return last;
  }

private:
  Iterator first, last;
};

template <typename Collection>
auto Head(Collection& v, size_t top) {
  return IteratorRange{v.begin(), next(v.begin(), min(top, v.size()))};
}

class DemographicsStats {
public:
    DemographicsStats() = default;

    explicit DemographicsStats(istream& input) : 
        people(ReadPeople(input)), 
        sorted_by_age(SortIteratorsByPredicat([](ConstIterator lhs, 
                                                 ConstIterator rhs) { 
                                                 return lhs->age < rhs->age;
        })),
        sorted_by_income(SortIteratorsByPredicat([](ConstIterator lhs, 
                                                    ConstIterator rhs) { 
                                                    return lhs->income > rhs->income;
        })),
        cumulative_income(CalculateCumulativeIncome()),
        popular_names(FindMostPopularNames()) 
       {}

    size_t AdultPeopleNumber(size_t adult_age) const {
        auto adult_begin = lower_bound(sorted_by_age.begin(), sorted_by_age.end(), adult_age, [](ConstIterator lhs, int age) {
            return lhs->age < age;
        });
        return distance(adult_begin, sorted_by_age.end());
    }

    size_t GetRichestPeopleIncome(size_t count) const {
        if (count > 0) {
            return cumulative_income[count - 1];
        }
        return 0;
    }

    string GetMostPopularMaleName() const {
        return popular_names.most_popular_male_name;
    }

    string GetMostPopularWomanName() const {
        return popular_names.most_popular_woman_name;
    }

private:

    struct Person {
        string name;
        int age, income;
        bool is_male;
    };

    struct MostPopularNames {
        string most_popular_male_name;
        string most_popular_woman_name;
    };
 
    using ConstIterator = vector<Person>::const_iterator;

    vector<Person> ReadPeople(istream& input) const {
        int count;
        input >> count;

        vector<Person> result(count);
        for (Person& p : result) {
            char gender;
            input >> p.name >> p.age >> p.income >> gender;
            p.is_male = gender == 'M';
        }
        return result;
    }

    template <typename Compare>
    vector<ConstIterator> SortIteratorsByPredicat(Compare comp) {
        vector<ConstIterator> result;
        result.reserve(people.size());
        for (auto it = people.cbegin(); it != people.cend(); ++it) {
            result.push_back(it);
        }
        sort(result.begin(), result.end(), comp);
        return result;
    }

    vector<size_t> CalculateCumulativeIncome() const {
        vector<size_t> result(people.size());
        if (!people.empty()) {
            result[0] = sorted_by_income[0]->income;
            for (size_t i = 1; i < people.size(); ++i) {
                result[i] = result[i - 1] + sorted_by_income[i]->income;
            }
        }
        return result;
    }

    MostPopularNames FindMostPopularNames() const {
        unordered_map<string, size_t> male_names;
        unordered_map<string, size_t> woman_names;

        for (const Person& p : people) {
            ++(p.is_male ? male_names[p.name] : woman_names[p.name]);
        }
        string most_popular_male_name;
        string most_popular_woman_name;
        size_t male_cnt = 0;
        size_t woman_cnt = 0;

        for (const auto& name : male_names) {
            if (name.second > male_cnt || (name.second == male_cnt && name.first < most_popular_male_name)) {
                male_cnt = name.second;
                most_popular_male_name = name.first;
            }
        }
        for (const auto& name : woman_names) {
            if (name.second > woman_cnt || (name.second == woman_cnt && name.first < most_popular_woman_name)) {
                woman_cnt = name.second;
                most_popular_woman_name = name.first;
            }
        }

        return {most_popular_male_name, most_popular_woman_name};
    } 
 
    const vector<Person> people;
    const vector<ConstIterator> sorted_by_age;
    const vector<ConstIterator> sorted_by_income;
    const vector<size_t> cumulative_income;
    const MostPopularNames popular_names;
};

int main() {
    DemographicsStats data(cin);
    for (string command; cin >> command; ) {
        if (command == "AGE") {
            int adult_age;
            cin >> adult_age;
            cout << "There are " << data.AdultPeopleNumber(adult_age)
                 << " adult people for maturity age " << adult_age << '\n';
        } else if (command == "WEALTHY") {
            int count;
            cin >> count;
            cout << "Top-" << count << " people have total income " << data.GetRichestPeopleIncome(count) << '\n';
        } else if (command == "POPULAR_NAME") {
            char gender;
            cin >> gender;
            string name = (gender == 'M' ? data.GetMostPopularMaleName() : data.GetMostPopularWomanName());
            if (name.empty()) {
                cout << "No people of gender " << gender << '\n';
            } else {
                cout << "Most popular name among people of gender " << gender << " is "
                     << name << '\n';
            }
        }
    }
}