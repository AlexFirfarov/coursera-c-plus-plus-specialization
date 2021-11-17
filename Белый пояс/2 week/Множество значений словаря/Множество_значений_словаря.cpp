#include <string>
#include <set>
#include <map>

using namespace std;

set<string> BuildMapValuesSet(const map<int, string>& m) {
  set<string> values;
  for (const auto& item : m) {
    values.insert(item.second);
  }
  return values;
}

int main() {}