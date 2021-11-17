#include <cmath>

#include "TransportManager.h"

using namespace std;

void TransportManager::AddBusStop(const string& name, double latitude, double longitude) {
    stops_[name].latitude_rad = latitude * PI / 180.;
    stops_[name].longitude_rad = longitude * PI / 180.;
}

void TransportManager::AddBusRoute(const string& bus_name, const vector<string>& bus_stops, bool is_ring_route) {
  Route& info = routes_[bus_name];
  info.stats.stops_count = is_ring_route ? bus_stops.size() : bus_stops.size() * 2 - 1;
  info.stats.unique_stops_count = unordered_set<string_view>(bus_stops.begin(), bus_stops.end()).size();
  info.stats.is_ring_route = is_ring_route;
  info.stop_names = bus_stops;

  for (const string& stop : bus_stops) {
    auto it = stops_.insert({stop, {}}).first;
    it->second.bus_names.insert(bus_name);
  }
}

optional<const TransportManager::RouteStats*> TransportManager::GetRouteStats(const string& bus_name) const {
  if (!routes_.count(bus_name)) {
    return nullopt;
  }
  const Route& route = routes_.at(bus_name);
  RouteStats& stats = route.stats;
  if (!stats.route_length_m) {
    stats.route_length_m = 0.;
    for (size_t i = 1; i < route.stop_names.size(); ++i) {
      *stats.route_length_m += PointDistance(stops_.at(route.stop_names[i - 1]), 
                                             stops_.at(route.stop_names[i]));
    }
    if (!stats.is_ring_route) {
      *stats.route_length_m *= 2;
    }
  }
  return &stats;
}

std::optional<const TransportManager::StopStats*> TransportManager::GetStopStats(const std::string& stop_name) const {
  if (!stops_.count(stop_name)) {
    return nullopt;
  }
  return &stops_.at(stop_name);
} 

double TransportManager::PointDistance(const StopStats& lhs, const StopStats& rhs) const {
  return acos(sin(lhs.latitude_rad) * sin(rhs.latitude_rad) + 
              cos(lhs.latitude_rad) * cos(rhs.latitude_rad) * 
                cos(abs(lhs.longitude_rad - rhs.longitude_rad)) 
  ) * EARTH_RADIUS_M;
}