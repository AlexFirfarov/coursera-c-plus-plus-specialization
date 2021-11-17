#include <algorithm>
#include <string>
#include <vector>
#include <set>

using namespace std;

class Learner {
 private:
  set<string> dict;

 public:
  int Learn(const vector<string>& words) {
    size_t cur_dict_size = dict.size();
    dict.insert(words.begin(), words.end());

    return dict.size() - cur_dict_size;
  }

  vector<string> KnownWords() {
    return vector<string>(dict.begin(), dict.end());
  }
};
