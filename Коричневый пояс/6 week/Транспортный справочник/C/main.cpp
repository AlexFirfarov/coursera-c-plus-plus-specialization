#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <string_view>
#include <unordered_map>

#include "request.h"
#include "TransportManager.h"

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  TransportManager manager;
  const auto modify_requests = ReadRequests<ModifyRequest>();
  const auto read_requests = ReadRequests<ReadRequest<void>>();
  ProcessRequests(modify_requests, manager);
  ProcessRequests(read_requests, manager);
}