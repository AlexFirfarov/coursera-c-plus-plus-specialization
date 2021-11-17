#include <cmath>

#include "TransportManager.h"

using namespace std;

void TransportManager::AddBusStop(const string& stop_name, double latitude, double longitude, const unordered_map<string, int>& distances) {
  Stop& stop = stops_[stop_name];
  stop.stats.latitude_rad = latitude * PI / 180.;
  stop.stats.longitude_rad = longitude * PI / 180.;

  for (const auto& [name, dist] : distances) {
    stop.distances[name] = dist;
    auto& distances_ref =  stops_[name].distances;
    if (auto it = distances_ref.find(stop_name); it == distances_ref.end()) {
      distances_ref[stop_name] = dist;
    }
  }
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
    double geo_route_length_m = 0.;
    stats.route_length_m = 0.;
    for (size_t i = 1; i < route.stop_names.size(); ++i) {
      geo_route_length_m += PointDistance(stops_.at(route.stop_names[i - 1]).stats, 
                                          stops_.at(route.stop_names[i]).stats);
      *stats.route_length_m += stops_.at(route.stop_names[i - 1]).distances.at(route.stop_names[i]);  
      if (!stats.is_ring_route) {
        *stats.route_length_m += stops_.at(route.stop_names[i]).distances.at(route.stop_names[i - 1]);  
      }                         
    }
    if (!stats.is_ring_route) {
      geo_route_length_m *= 2;
    }
    stats.curvature = *stats.route_length_m / geo_route_length_m;
  }
  return &stats;
}

std::optional<const std::set<std::string>*> TransportManager::GetStopStats(const std::string& stop_name) const {
  if (!stops_.count(stop_name)) {
    return nullopt;
  }
  return &stops_.at(stop_name).bus_names;
} 

double TransportManager::PointDistance(const StopStats& lhs, const StopStats& rhs) const {
  return acos(sin(lhs.latitude_rad) * sin(rhs.latitude_rad) + 
              cos(lhs.latitude_rad) * cos(rhs.latitude_rad) * 
                cos(abs(lhs.longitude_rad - rhs.longitude_rad)) 
  ) * EARTH_RADIUS_M;
}