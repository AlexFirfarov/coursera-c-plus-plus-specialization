#include "Common.h"

#include <list>
#include <mutex>
#include <unordered_map>

using namespace std;

class LruCache : public ICache {
public:
  LruCache(
      shared_ptr<IBooksUnpacker> books_unpacker,
      const Settings& settings
  ) : books_unpacker_(move(books_unpacker)), settings_(settings) {}

  BookPtr GetBook(const string& book_name) override {
    lock_guard<mutex> guard(m);

    if (auto it = cache_books_.find(book_name); it != cache_books_.end()) {
      auto book_it = it->second;
      ordered_by_rank_.push_back(move(*book_it));
      ordered_by_rank_.erase(book_it);
      it->second = prev(ordered_by_rank_.end());
      return ordered_by_rank_.back();
    }

    BookPtr book = books_unpacker_->UnpackBook(book_name);
    const string& book_content = book->GetContent();

    while (!ordered_by_rank_.empty() && used_memory + book_content.size() > settings_.max_memory) {
      BookPtr& old_book = ordered_by_rank_.front();
      size_t old_book_size = old_book->GetContent().size();
      cache_books_.erase(old_book->GetName());
      ordered_by_rank_.pop_front();
      used_memory -= old_book_size;
    }

    if (used_memory + book_content.size() <= settings_.max_memory) {
      used_memory += book_content.size();
      ordered_by_rank_.push_back(book);
      cache_books_[book->GetName()] = prev(ordered_by_rank_.end());
    }

    return book;
  }

private:
  const shared_ptr<IBooksUnpacker> books_unpacker_;
  const Settings settings_;

  list<BookPtr> ordered_by_rank_;
  unordered_map<string, list<BookPtr>::iterator> cache_books_;

  size_t used_memory = 0;
  mutex m;
};


unique_ptr<ICache> MakeCache(
    shared_ptr<IBooksUnpacker> books_unpacker,
    const ICache::Settings& settings
) {
  return make_unique<LruCache>(move(books_unpacker), settings);
}
