#include "test_runner.h"

#include <vector>

using namespace std;

void TestComputeMedianAgeEmpty() {
  vector<Person> persons;
  ASSERT_EQUAL(ComputeMedianAge(begin(persons), end(persons)), 0);
}

void TestComputeMedianCorrectMiddle() {
  vector<Person> persons = {Person{.age = 5}, Person{.age = 10}};
  ASSERT_EQUAL(ComputeMedianAge(begin(persons), end(persons)), 10);
  ASSERT_EQUAL(ComputeMedianAge(begin(persons), begin(persons) + 1), 5);
}

void TestReadPersonsFieldsOrder() {
  stringstream ss;
  ss << 1 << endl
     << 30 << ' ' << 0 << ' ' << 1 << endl;

  ASSERT_EQUAL(
      ReadPersons(ss),
      vector<Person>({Person{30, Gender::FEMALE, true}})
  );
}

void TestReadPersonsCheckEmployedField() {
  stringstream ss;
  ss << 1 << endl
      << 30 << ' ' << 0 << ' ' << 1 << endl;

  ASSERT_EQUAL(
      ReadPersons(ss),
      vector<Person>({Person{30, Gender::FEMALE, true}})
  );
}


void TestComputeStatsInverseBounds() {
  const vector<Person> persons = {
      Person{50, Gender::MALE, true},
  };
  ASSERT_EQUAL(ComputeStats(persons).employed_males, 50);
}

void TestPrintStatsCheckOrder() {
  ostringstream stream;
  const AgeStats stats{
      .total = 1,
      .females = 2,
      .males = 3,
      .employed_females = 4,
      .unemployed_females = 5,
      .employed_males = 6,
      .unemployed_males = 7
  };
  PrintStats(stats, stream);
  ASSERT_EQUAL(
      stream.str(),
      "Median age = 1\n" \
      "Median age for females = 2\n" \
      "Median age for males = 3\n" \
      "Median age for employed females = 4\n" \
      "Median age for unemployed females = 5\n" \
      "Median age for employed males = 6\n" \
      "Median age for unemployed males = 7\n"
  );
}


int main() {
  TestRunner tr;
  RUN_TEST(tr, TestComputeMedianAgeEmpty);
  RUN_TEST(tr, TestComputeMedianCorrectMiddle);
  RUN_TEST(tr, TestReadPersonsFieldsOrder);
  RUN_TEST(tr, TestReadPersonsCheckEmployedField);
  RUN_TEST(tr, TestComputeStatsInverseBounds);
  RUN_TEST(tr, TestPrintStatsCheckOrder);
}