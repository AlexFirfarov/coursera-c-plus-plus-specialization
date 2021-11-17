#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <future>

using namespace std;

struct Stats {
  map<string, int> word_frequences;

  void operator += (const Stats& other) {
    for (const auto& [key, value] : other.word_frequences) {
      word_frequences[key] += value;
    } 
  }
};


Stats ExploreKeyWordsSingleThread(const set<string>& key_words, 
                                  std::vector<string>::iterator begin, 
                                  std::vector<string>::iterator end) {
  Stats stats;
  for (;begin != end; ++begin) {
    if (key_words.count(*begin)) {
      ++stats.word_frequences[move(*begin)];
    }
  }
  return stats;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
  const size_t thread_number = 32;
  vector<string> words;
  string word;

  while (input >> word) {
    words.push_back(move(word));
  }

  vector<future<Stats>> futures;
  size_t mod = words.size() % thread_number;
  size_t base_vector_size = words.size() / thread_number;

  for (std::vector<string>::iterator vector_begin = words.begin(); vector_begin != words.end();) {
    size_t vector_size = base_vector_size;
    if (mod > 0) {
      ++vector_size;
      --mod;
    }
    std::vector<string>::iterator vector_end = vector_begin + vector_size;
    futures.push_back(async(ExploreKeyWordsSingleThread, ref(key_words), vector_begin, vector_end));
    vector_begin = vector_end;
  }

  Stats stats;
  for (auto& f : futures) {
    stats += f.get();
  }

  return stats;
}

void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {
    {"yangle", 6},
    {"rocks", 2},
    {"sucks", 1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
}
