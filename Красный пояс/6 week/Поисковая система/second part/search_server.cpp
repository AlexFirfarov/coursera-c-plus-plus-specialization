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
  index.GetAccess().ref_to_value = move(new_index);
}

void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output) {
  futures.push_back(async([this, &query_input, &search_results_output](){
    QueriesStreamThread(query_input, search_results_output);
  }));
}

void SearchServer::QueriesStreamThread(
  istream& query_input, ostream& search_results_output
) {
  vector<size_t> docid_count(50000);
  vector<size_t> docid_idx(50000);

  for (string current_query; getline(query_input, current_query); ) {
    size_t curr_idx = 0;
    for (auto word : SplitIntoWords(current_query)) {
      for (const auto& [id, count] : index.GetAccess().ref_to_value.Lookup(move(word))) {
        if (docid_count[id] == 0) {
          docid_idx[curr_idx++] = id;
        }
        docid_count[id] += count;
      }
    }

    vector<pair<size_t, size_t>> search_result;
    for (size_t doc_id = 0; doc_id < curr_idx; ++doc_id) {
      size_t count = 0;
      size_t id = 0;
      swap(count, docid_count[docid_idx[doc_id]]);
      swap(id, docid_idx[doc_id]);
      search_result.emplace_back(id, count);
    }
    
    partial_sort(
      search_result.begin(),
      search_result.begin() + min<size_t>(5, search_result.size()),
      search_result.end(),
      [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
        int64_t lhs_docid = lhs.first;
        auto lhs_hit_count = lhs.second;
        int64_t rhs_docid = rhs.first;
        auto rhs_hit_count = rhs.second;
        return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
      }
    );

    search_results_output << current_query << ':';
    for (const auto& [docid, hitcount]: Head(search_result, 5)) {
        search_results_output << " {"
          << "docid: " << docid << ", "
          << "hitcount: " << hitcount << '}';
    }
    search_results_output << '\n';
  }
}

void InvertedIndex::Add(string document) {
    docs.push_back(std::move(document));
    const size_t docid = docs.size() - 1;

    for (string word : SplitIntoWords(docs.back())) {
      auto& vec_p= index[move(word)];
      if(!vec_p.empty() && vec_p.back().first == docid) {
        ++vec_p.back().second;
      } else {
        vec_p.emplace_back(docid, 1);
      }
    }
}

const vector<pair<size_t, size_t>>& InvertedIndex::Lookup(string word) const {
  if (auto it = index.find(move(word)); it != index.end()) {
    return it->second;
  } else {
    return empty_vector;
  }
}
