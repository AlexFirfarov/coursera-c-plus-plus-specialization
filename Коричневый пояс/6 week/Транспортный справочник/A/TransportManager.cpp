#include <cmath>

#include "TransportManager.h"

using namespace std;

void TransportManager::AddBusStop(const string& stop_name, double latitude, double longitude) {
    stops_[stop_name] = {
      .latitude_rad = latitude * PI / 180.,
      .longitude_rad = longitude * PI / 180.
    };
}

void TransportManager::AddBusRoute(const string& bus_name, const vector<string>& bus_stops, bool is_ring_route) {
  Route& info = routes_[bus_name];
  info.stats.stops_count = is_ring_route ? bus_stops.size() : bus_stops.size() * 2 - 1;
  info.stats.unique_stops_count = unordered_set<string_view>(bus_stops.begin(), bus_stops.end()).size();
  info.stats.is_ring_route = is_ring_route;

  info.stop_names.reserve(bus_stops.size());
  for (const string& stop : bus_stops) {
    string_view bus_stop = (stops_.insert({stop, {}}).first)->first;
    info.stop_names.push_back(bus_stop);
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
      *stats.route_length_m += PointDistance(stops_.at(route.stop_names[i - 1].data()), 
                                             stops_.at(route.stop_names[i].data()));
    }
    if (!stats.is_ring_route) {
      *stats.route_length_m *= 2;
    }
  }
  return &stats;
}

double TransportManager::PointDistance(const StopStats& lhs, const StopStats& rhs) const {
  return acos(sin(lhs.latitude_rad) * sin(rhs.latitude_rad) + 
              cos(lhs.latitude_rad) * cos(rhs.latitude_rad) * 
                cos(abs(lhs.longitude_rad - rhs.longitude_rad)) 
  ) * EARTH_RADIUS_M;
}