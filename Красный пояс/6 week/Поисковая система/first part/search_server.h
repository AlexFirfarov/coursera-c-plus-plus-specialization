#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <string>
using namespace std;

class InvertedIndex {
public:
  void Add(string document);
  const vector<pair<size_t, size_t>>& Lookup(string word) const;

  const string& GetDocument(size_t id) const {
    return docs[id];
  }

  size_t GetDocumentsNumber() const {
    return docs.size();
  }
 
private:
  map<string, vector<pair<size_t, size_t>>> index;
  vector<string> docs;
  vector<pair<size_t, size_t>> empty_vector;
};

class SearchServer {
public:
  SearchServer() = default;
  explicit SearchServer(istream& document_input);
  void UpdateDocumentBase(istream& document_input);
  void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
  InvertedIndex index;
};
