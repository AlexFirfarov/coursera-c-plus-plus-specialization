#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <string_view>

using namespace std;

bool IsSubdomain(const string_view subdomain, const string_view domain) {
  int64_t i = subdomain.size() - 1;
  int64_t j = domain.size() - 1;
  while (i >= 0 && j >= 0) {
    if (subdomain[i--] != domain[j--]) {
      return false;
    }
  }
  return (j < 0 && (i < 0 || subdomain[i] == '.'));
}

bool IsSubdomainReversed(const string_view subdomain, const string_view domain) {
  size_t i = 0;
  size_t j = 0;
  while (i < subdomain.size() && j < domain.size()) {
    if (subdomain[i++] != domain[j++]) {
      return false;
    }
  }
  return (j == domain.size() && (i == subdomain.size() || subdomain[i] == '.'));
}

vector<string> ReadDomains(istream& stream) {
  size_t count;
  stream >> count;
  stream.ignore(1);

  vector<string> domains;
  domains.reserve(count);
  for (size_t i = 0; i < count; ++i) {
    string domain;
    getline(stream, domain, '\n');
    domains.push_back(move(domain));
  }
  return domains;
}

void RemoveSubdomains(vector<string>& domains) {
  for (string& domain : domains) {
    reverse(domain.begin(), domain.end());
  }
  
  sort(domains.begin(), domains.end());

  size_t insert_pos = 0;
  for (string& domain : domains) {
    if (insert_pos == 0 || !IsSubdomainReversed(domain, domains[insert_pos - 1])) {
      swap(domains[insert_pos++], domain);
    }
  }
  domains.resize(insert_pos);

  for (string& domain : domains) {
    reverse(domain.begin(), domain.end());
  }
}

int main() {
  vector<string> banned_domains = ReadDomains(cin);
  const vector<string> domains_to_check = ReadDomains(cin);

  RemoveSubdomains(banned_domains);

  for (const string& domain : domains_to_check) {
    bool bad = false;
    for (const string& bad_domain : banned_domains) {
      if (IsSubdomain(domain, bad_domain)) {
        bad = true;
        break;
      }
    }
    cout << (bad ? "Bad" : "Good") << '\n';
  }
  
  return 0;
}