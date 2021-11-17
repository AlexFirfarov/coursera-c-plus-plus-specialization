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
    default:
      return nullptr;
  }
}

AddStopRequest::AddStopRequest() : ModifyRequest(Type::MODIFY_STOP) {}

void AddStopRequest::ParseFrom(string_view input) {
  stop_name = ReadToken(input, ": ");
  latitude = ConvertToDouble(ReadToken(input, ", "));
  longitude = ConvertToDouble(input);
}

void AddStopRequest::Process(TransportManager& manager) const {
  manager.AddBusStop(stop_name, latitude, longitude);
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

void AddBusRequest::Process(TransportManager& manager) const {
  manager.AddBusRoute(bus_name, stops, is_ring_route);
}

GetRouteInfoRequest::GetRouteInfoRequest() : ReadRequest(Type::READ_BUS) {}

void GetRouteInfoRequest::ParseFrom(string_view input) {
  bus_name = ReadToken(input, "\n");
}

void GetRouteInfoRequest::Process(const TransportManager& manager) const { 
  optional<const TransportManager::RouteStats*> result = manager.GetRouteStats(bus_name);
  if (!result) {
    cout << "Bus " << bus_name << ": not found\n";
    return;
  }

  cout << setprecision(6) 
       << "Bus " << bus_name << ": "
       << (*result)->stops_count << " stops on route, "
       << (*result)->unique_stops_count << " unique stops, "
       << (*result)->route_length_m.value() << " route length\n";
}

void ProcessRequests(const std::vector<RequestHolder>& requests, TransportManager& manager) {
  for (const auto& request_holder : requests) {
    if (request_holder->type == Request::Type::READ_BUS) {
      const auto& request = static_cast<const GetRouteInfoRequest&>(*request_holder);
      request.Process(manager);
    } else {
      const auto& request = static_cast<const ModifyRequest&>(*request_holder);
      request.Process(manager);
    }
  }
}