#include "test_runner.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <string_view>

using namespace std;

struct Record {
  string id;
  string title;
  string user;
  int timestamp;
  int karma;
};

// Реализуйте этот класс
class Database {
public:
  bool Put(const Record& record) {
    auto [it, inserted] = records_by_id.insert({record.id, record});
    if (inserted) {
      const string_view id = it->first;
      const string_view user = it->second.user;
      Record* ptr = &(it->second);

      timestamp_it[id] = records_by_timestamp.insert({record.timestamp, ptr});
      karma_it[id] = records_by_karma.insert({record.karma, ptr});
      user_it[id] = record_by_user.insert({user, ptr});
    }
    return inserted;
  }

  const Record* GetById(const string& id) const {
    auto it = records_by_id.find(id);
    if (it != records_by_id.end()) {
      return &(it->second);
    }
    return nullptr;
  }

  bool Erase(const string& id) {
    bool erased = false;
    auto it = records_by_id.find(id);
    if (it != records_by_id.end()) {
      erased = true;
      
      records_by_timestamp.erase(timestamp_it[id]);
      records_by_karma.erase(karma_it[id]);
      record_by_user.erase(user_it[id]);
      records_by_id.erase(it);
    }

    return erased;
  }

  template <typename Callback>
  void RangeByTimestamp(int low, int high, Callback callback) const {
    Range(records_by_timestamp, low, high, callback);
  }

  template <typename Callback>
  void RangeByKarma(int low, int high, Callback callback) const {
    Range(records_by_karma, low, high, callback);
  }

  template <typename Callback>
  void AllByUser(const string& user, Callback callback) const {
    auto [lower_it, upper_it] = record_by_user.equal_range(user);
    if (lower_it != upper_it) {
      for (auto it = lower_it; it != upper_it; ++it) {
        if (bool ok = callback(*(it->second)); !ok) {
          break;
        }
      }
    }
  }

private:
  template <typename Callback>
  void Range(const multimap<int, Record*>& m, int low, int high, Callback callback) const {
    auto lower_it = m.lower_bound(low);
    auto upper_it = m.upper_bound(high);
    for (auto it = lower_it; it != upper_it; ++it) {
      if (bool ok = callback(*(it->second)); !ok) {
        break;
      }
    }
  }

  unordered_map<string, Record> records_by_id;
  multimap<int, Record*> records_by_timestamp;
  multimap<int, Record*> records_by_karma;
  multimap<string_view, Record*> record_by_user;
  
  unordered_map<string_view, multimap<int, Record*>::iterator> timestamp_it;
  unordered_map<string_view, multimap<int, Record*>::iterator> karma_it;
  unordered_map<string_view, multimap<string_view, Record*>::iterator> user_it;
};

void TestRangeBoundaries() {
  const int good_karma = 1000;
  const int bad_karma = -10;

  Database db;
  db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
  db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});

  int count = 0;
  db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestSameUser() {
  Database db;
  db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
  db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

  int count = 0;
  db.AllByUser("master", [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestReplacement() {
  const string final_body = "Feeling sad";

  Database db;
  db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
  db.Erase("id");
  db.Put({"id", final_body, "not-master", 1536107260, -10});

  auto record = db.GetById("id");
  ASSERT(record != nullptr);
  ASSERT_EQUAL(final_body, record->title);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestRangeBoundaries);
  RUN_TEST(tr, TestSameUser);
  RUN_TEST(tr, TestReplacement);
  return 0;
}
