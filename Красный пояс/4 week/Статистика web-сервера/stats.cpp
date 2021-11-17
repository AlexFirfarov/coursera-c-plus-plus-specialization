#include "stats.h"

Stats::Stats() {
  for (const auto& sw : {"GET", "POST", "PUT", "DELETE", "UNKNOWN"}) {
      method_stats[sw] = 0;
  }

  for (const auto& sw : {"/", "/order", "/product", "/basket", "/help", "unknown"}) {
      uri_stats[sw] = 0;
  }
}

void Stats::AddMethod(string_view method) {
  if (method == "GET" || method == "POST" || method == "PUT" || method == "DELETE") {
    ++method_stats[method];
    return;
  }
  ++method_stats["UNKNOWN"];
}

void Stats::AddUri(string_view uri) {
  if (uri == "/" || uri == "/order" || uri == "/product" || uri == "/basket" || uri == "/help") {
    ++uri_stats[uri];
    return;
  }
  ++uri_stats["unknown"];
}

const map<string_view, int>& Stats::GetMethodStats() const {
  return method_stats;
}

const map<string_view, int>& Stats::GetUriStats() const {
  return uri_stats;
}

HttpRequest ParseRequest(string_view line) {
  HttpRequest result;

  size_t new_start = line.find_first_not_of(' ');
  line.remove_prefix(new_start);

  size_t pos_first = line.find(' ');
  result.method = line.substr(0, pos_first);

  size_t pos_last = line.rfind(' ');
  result.uri = line.substr(pos_first + 1, pos_last - pos_first - 1);
  result.protocol = line.substr(pos_last + 1);

  return result;
}