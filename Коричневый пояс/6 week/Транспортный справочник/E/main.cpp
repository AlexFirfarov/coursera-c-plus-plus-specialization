#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <string_view>
#include <unordered_map>

#include "request.h"
#include "TransportManager.h"
#include "json.h"

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  TransportManager manager;
  Json::Document document = Json::Load(cin);
  const auto& settings = document.GetRoot().AsMap().at("routing_settings").AsMap();
  manager.SetRoutingSettings(settings.at("bus_wait_time").AsInt(),
                             settings.at("bus_velocity").AsDouble());

  const auto modify_requests = ReadRequests<ModifyRequest>(document);
  const auto read_requests = ReadRequests<ReadRequest<void>>(document);
  ProcessJsonRequests(modify_requests, manager);
  const auto result_nodes = ProcessJsonRequests(read_requests, manager);

  if (result_nodes) {
    cout << Json::Node(*result_nodes).ToString();
  }

  return 0;
}