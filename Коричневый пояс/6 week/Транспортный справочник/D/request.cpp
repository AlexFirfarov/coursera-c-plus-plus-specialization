#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <iomanip>

#include "request.h"

using namespace std;

pair<string_view, optional<string_view>> SplitTwoStrict(string_view s, string_view delimiter) {
  const size_t pos = s.find(delimiter);
  if (pos == s.npos) {
    return {s, nullopt};
  } else {
    return {s.substr(0, pos), s.substr(pos + delimiter.length())};
  }
}

pair<string_view, string_view> SplitTwo(string_view s, string_view delimiter) {
  const auto [lhs, rhs_opt] = SplitTwoStrict(s, delimiter);
  return {lhs, rhs_opt.value_or("")};
}

string_view ReadToken(string_view& s, string_view delimiter) {
  const auto [lhs, rhs] = SplitTwo(s, delimiter);
  s = rhs;
  return lhs;
}

int ConvertToInt(string_view str) {
  size_t pos;
  const int result = stoi(string(str), &pos);
  if (pos != str.length()) {
    std::stringstream error;
    error << "string " << str << " contains " << (str.length() - pos) << " trailing chars";
    throw invalid_argument(error.str());
  }
  return result;
}

double ConvertToDouble(string_view str) {
  size_t pos;
  const double result = stod(string(str), &pos);
  if (pos != str.length()) {
    std::stringstream error;
    error << "string " << str << " contains " << (str.length() - pos) << " trailing chars";
    throw invalid_argument(error.str());
  }
  return result;
}

RequestHolder Request::Create(Request::Type type) {
  switch (type) {
    case Request::Type::MODIFY_STOP:
      return make_unique<AddStopRequest>();
    case Request::Type::MODIFY_BUS:
      return make_unique<AddBusRequest>();
    case Request::Type::READ_BUS:
      return make_unique<GetRouteInfoRequest>();
    case Request::Type::READ_STOP:
      return make_unique<GetStopInfoRequest>();
    default:
      return nullptr;
  }
}

AddStopRequest::AddStopRequest() : ModifyRequest(Type::MODIFY_STOP) {}

void AddStopRequest::ParseFrom(string_view input) {
  stop_name = ReadToken(input, ": ");
  latitude = ConvertToDouble(ReadToken(input, ", "));
  if (input.find(',') == input.npos) {
    longitude = ConvertToDouble(input);
    return;
  }
  longitude = ConvertToDouble(ReadToken(input, ", "));
  while (!input.empty()) {
    int dist = ConvertToInt(ReadToken(input, "m to "));
    string name = string(ReadToken(input, ", "));
    distances[move(name)] = dist;
  }
}

void AddStopRequest::ParseFrom(const Json::Node& request_node) {
  const auto& request_info = request_node.AsMap();
  stop_name = request_info.at("name").AsString();
  latitude = request_info.at("latitude").AsDouble();
  longitude = request_info.at("longitude").AsDouble();

  const auto& road_distances = request_info.at("road_distances").AsMap();
  for (const auto& [stop, dist] : road_distances) {
    distances[stop] = dist.AsInt();
  }
}

void AddStopRequest::Process(TransportManager& manager) const {
  manager.AddBusStop(stop_name, latitude, longitude, distances);
}

AddBusRequest::AddBusRequest() : ModifyRequest(Type::MODIFY_BUS) {}

void AddBusRequest::ParseFrom(string_view input) {
  bus_name = ReadToken(input, ": ");
  string delimeter = " ";
  char del = *find_if(input.begin(), input.end(), [](char c){
    return c == '>' || c == '-';
  });
  delimeter.push_back(del);
  delimeter.push_back(' ');
  while(!input.empty()) {
    stops.push_back(string(ReadToken(input, delimeter)));
  }
  is_ring_route = del == '>' ? true : false;
}

void AddBusRequest::ParseFrom(const Json::Node& request_node) {
  const auto& request_info = request_node.AsMap();
  bus_name = request_info.at("name").AsString();
  is_ring_route = request_info.at("is_roundtrip").AsBoolean();

  const auto& stops_array = request_info.at("stops").AsArray();
  stops.reserve(stops_array.size());
  for (const auto& stop : stops_array) {
    stops.push_back(stop.AsString());
  }
}

void AddBusRequest::Process(TransportManager& manager) const {
  manager.AddBusRoute(bus_name, stops, is_ring_route);
}

GetRouteInfoRequest::GetRouteInfoRequest() : ReadRequest(Type::READ_BUS) {}

void GetRouteInfoRequest::ParseFrom(string_view input) {
  bus_name = ReadToken(input, "\n");
}

void GetRouteInfoRequest::ParseFrom(const Json::Node& request_node) {
  const auto& request_info = request_node.AsMap();
  bus_name = request_info.at("name").AsString();
  id = request_info.at("id").AsInt();
}

Json::Node GetRouteInfoRequest::Process(const TransportManager& manager) const { 
  map<string, Json::Node> result_map;
  optional<const TransportManager::RouteStats*> stats = manager.GetRouteStats(bus_name);

  result_map["request_id"] = Json::Node(id);
  if (!stats) {
    result_map["error_message"] = Json::Node("not found"s);
    return Json::Node(move(result_map));
  }
  result_map["stop_count"] = Json::Node(static_cast<int>((*stats)->stops_count));
  result_map["unique_stop_count"] = Json::Node(static_cast<int>((*stats)->unique_stops_count));
  result_map["route_length"] = Json::Node((*stats)->route_length_m.value());
  result_map["curvature"] = Json::Node((*stats)->curvature);

  return Json::Node(move(result_map));
}

GetStopInfoRequest::GetStopInfoRequest() : ReadRequest(Type::READ_STOP) {}

void GetStopInfoRequest::ParseFrom(string_view input) {
  stop_name = ReadToken(input, "\n");
}

void GetStopInfoRequest::ParseFrom(const Json::Node& request_node) {
  const auto& request_info = request_node.AsMap();
  stop_name = request_info.at("name").AsString();
  id = request_info.at("id").AsInt();
}

Json::Node GetStopInfoRequest::Process(const TransportManager& manager) const {
  map<string, Json::Node> result_map;
  std::optional<const std::set<std::string>*> stats = manager.GetStopStats(stop_name);

  result_map["request_id"] = Json::Node(id);
  if (!stats) {
    result_map["error_message"] = Json::Node("not found"s);
    return Json::Node(move(result_map));
  }
  vector<Json::Node> buses_array;
  buses_array.reserve((*stats)->size());
  for (const string& bus : *(*stats)) {
    buses_array.push_back(Json::Node(bus));
  }
  result_map["buses"] = Json::Node(buses_array);

  return Json::Node(move(result_map));
}

optional<vector<Json::Node>> ProcessJsonRequests(const std::vector<RequestHolder>& requests, TransportManager& manager) {
  vector<Json::Node> results;
  for (const auto& request_holder : requests) {
    if (request_holder->type == Request::Type::READ_BUS) {
      const auto& request = static_cast<const GetRouteInfoRequest&>(*request_holder);
      results.push_back(request.Process(manager));
    } else if (request_holder->type == Request::Type::READ_STOP) {
      const auto& request = static_cast<const GetStopInfoRequest&>(*request_holder);
      results.push_back(request.Process(manager));
    } else {
      const auto& request = static_cast<const ModifyRequest&>(*request_holder);
      request.Process(manager);
    }
  }
  if (results.empty()) {
    return nullopt;
  } else {
    return results;
  }
}

void ProcessRequests(const vector<RequestHolder>& requests, TransportManager& manager) {
  for (const auto& request_holder : requests) {
    if (request_holder->type == Request::Type::READ_BUS) {
      const auto& request = static_cast<const GetRouteInfoRequest&>(*request_holder);
      request.Process(manager);
    } else if (request_holder->type == Request::Type::READ_STOP) {
      const auto& request = static_cast<const GetStopInfoRequest&>(*request_holder);
      request.Process(manager);
    } else {
      const auto& request = static_cast<const ModifyRequest&>(*request_holder);
      request.Process(manager);
    }
  }
}