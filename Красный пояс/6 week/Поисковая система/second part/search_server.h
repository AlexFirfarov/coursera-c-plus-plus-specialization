#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <vector>
#include <string>
#include <future>
#include <unordered_map>

#include "synchronized.h"

using namespace std;

class InvertedIndex {
public:
  InvertedIndex() {
    docs.reserve(10000);
  }

  void Add(string document);
  const vector<pair<size_t, size_t>>& Lookup(string word) const;

  const string& GetDocument(size_t id) const {
    return docs[id];
  }

  size_t GetDocumentsNumber() const {
    return docs.size();
  }
 
private:
  unordered_map<string, vector<pair<size_t, size_t>>> index;
  vector<string> docs;
  vector<pair<size_t, size_t>> empty_vector;
};

class SearchServer {
public:
  SearchServer() {
    futures.reserve(10000);
  }
  explicit SearchServer(istream& document_input);
  void UpdateDocumentBase(istream& document_input);
  void AddQueriesStream(istream& query_input, ostream& search_results_output);
  void QueriesStreamThread(istream& query_input, ostream& search_results_output);

private:
  Synchronized<InvertedIndex> index;
  vector<future<void>> futures;
};
