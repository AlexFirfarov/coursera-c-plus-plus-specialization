#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <numeric>

vector<string> SplitIntoWords(const string& line) {
  istringstream words_input(line);
  return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

SearchServer::SearchServer(istream& document_input) {
  UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
  InvertedIndex new_index;

  for (string current_document; getline(document_input, current_document); ) {
    new_index.Add(move(current_document));
  }
  index = move(new_index);
}

void SearchServer::AddQueriesStream(
  istream& query_input, ostream& search_results_output
) {
  for (string current_query; getline(query_input, current_query); ) {
    vector<size_t> docid_count(index.GetDocumentsNumber(), 0);
    for (auto word : SplitIntoWords(current_query)) {
      for (const auto& docinf : index.Lookup(move(word))) {
        docid_count[docinf.first] += docinf.second;
      }
    }

    vector<int64_t> docid_v(index.GetDocumentsNumber());
    iota(docid_v.begin(), docid_v.end(), 0);
    
    partial_sort(
      docid_v.begin(),
      docid_v.begin() + min(docid_v.size(), static_cast<size_t>(5)),
      docid_v.end(),
      [&docid_count](int64_t lhs_docid, int64_t rhs_docid) {
        auto lhs_hit_count = docid_count[lhs_docid];
        auto rhs_hit_count = docid_count[rhs_docid];
        return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
      }
    );

    search_results_output << current_query << ':';
    for (auto docid: Head(docid_v, 5)) {
      if (docid_count[docid] > 0) {
        search_results_output << " {"
          << "docid: " << docid << ", "
          << "hitcount: " << docid_count[docid] << '}';
      }
    }
    search_results_output << endl;
  }
}

void InvertedIndex::Add(string document) {
  const size_t docid = docs.size();
  map<string, size_t> document_index;
  for (string word : SplitIntoWords(document)) {
    ++document_index[move(word)];
  }
  for (auto& [word, number] : document_index) {
    index[move(word)].push_back({docid, number});
  }
  docs.push_back(move(document));
}

const vector<pair<size_t, size_t>>& InvertedIndex::Lookup(string word) const {
  if (auto it = index.find(move(word)); it != index.end()) {
    return it->second;
  } else {
    return empty_vector;
  }
}
