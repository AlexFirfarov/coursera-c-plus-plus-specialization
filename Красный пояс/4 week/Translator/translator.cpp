#include "test_runner.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include <string_view>

using namespace std;

class Translator {
public:
  Translator() = default;

  void Add(string_view source, string_view target) {
    auto [it_source, bool_source] = words.insert(string(source.begin(), source.end()));
    auto [it_target, bool_target] = words.insert(string(target.begin(), target.end()));

    forward_map[string_view(it_source->data(), it_source->size())].emplace_back(it_target->data(), it_target->size());
    backward_map[string_view(it_target->data(), it_target->size())].emplace_back(it_source->data(), it_source->size());

    return;
  }

  string_view TranslateForward(string_view source) const {
    if (forward_map.count(source)) {
      return forward_map.at(source).back();
    }
    return string_view("");
  }

  string_view TranslateBackward(string_view target) const {
    if (backward_map.count(target)) {
      return backward_map.at(target).back();
    }
    return string_view("");
  }

private:
  map<string_view, vector<string_view>> forward_map;
  map<string_view, vector<string_view>> backward_map;
  set<string> words;
};

void TestSimple() {
  Translator translator;
  translator.Add(string("okno"), string("window"));
  translator.Add(string("stol"), string("table"));

  ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
  ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
  ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSimple);
  return 0;
}
