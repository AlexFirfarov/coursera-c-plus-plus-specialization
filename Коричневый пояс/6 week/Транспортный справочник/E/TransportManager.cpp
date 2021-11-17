#include <cmath>
#include <stdexcept>
#include <iostream>

#include "TransportManager.h"

using namespace std;

void TransportManager::AddBusStop(const string& stop_name, double latitude, double longitude, const unordered_map<string, int>& distances) {
  const auto [it, inserted] = stops_.insert({stop_name, {}});
  if (inserted) {
    it->second.idx = stops_.size() - 1;
  }

  Stop& stop = it->second;
  stop.stats.latitude_rad = latitude * PI / 180.;
  stop.stats.longitude_rad = longitude * PI / 180.;

  for (const auto& [name, dist] : distances) {
    stop.distances[name] = dist;
    const auto [it_, inserted_] = stops_.insert({name, {}});
    if (inserted_) {
      it_->second.idx = stops_.size() - 1;
    }
    auto& distances_ref = (it_->second).distances;
    if (auto it_d = distances_ref.find(stop_name); it_d == distances_ref.end()) {
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
    const auto [it, inserted] = stops_.insert({stop, {}});
    if (inserted) {
      it->second.idx = stops_.size() - 1;
    }
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

void TransportManager::SetRoutingSettings(size_t bus_wait_time_min, double bus_velocity_km_per_h) {
  settings_.bus_wait_time_min = bus_wait_time_min;
  settings_.bus_velocity_km_per_h = bus_velocity_km_per_h;
  settings_.bus_velocity_m_per_min = bus_velocity_km_per_h * 1000 / 60;
}

double TransportManager::PointDistance(const StopStats& lhs, const StopStats& rhs) const {
  return acos(sin(lhs.latitude_rad) * sin(rhs.latitude_rad) + 
              cos(lhs.latitude_rad) * cos(rhs.latitude_rad) * 
                cos(abs(lhs.longitude_rad - rhs.longitude_rad)) 
  ) * EARTH_RADIUS_M;
}

void TransportManager::BuildRoutes() const {
  road_graph = make_unique<Graph::DirectedWeightedGraph<double>>(2 * stops_.size());

  for (const auto& [stop, stop_info] : stops_) {
    road_graph->AddEdge(Graph::Edge<double>{
      .from = 2 * stop_info.idx,
      .to = 2 * stop_info.idx + 1,
      .weight = static_cast<double>(settings_.bus_wait_time_min)
    });
    edges_description.push_back(WaitActivity{
      .stop_name = stop,
      .time = settings_.bus_wait_time_min
    });
  }

  for (const auto& [bus_name, bus_info] : routes_) {
    std::vector<std::string> bus_stops = bus_info.stop_names;
    if (!bus_info.stats.is_ring_route) {
      size_t size = bus_stops.size();
      for (size_t i = size - 1; i > 0; --i) {
        bus_stops.push_back(bus_stops[i - 1]);
      }
    }
    for (size_t i = 0; i < bus_stops.size(); ++i) {
      double time_sum = 0.;
      size_t span_count = 0;
      for (size_t j = i + 1; j < bus_stops.size(); ++j) {
        time_sum += stops_.at(bus_stops[j - 1]).distances.at(bus_stops[j]) / settings_.bus_velocity_m_per_min;
        road_graph->AddEdge(Graph::Edge<double>{
            .from = 2 * stops_.at(bus_stops[i]).idx + 1,
            .to = 2 * stops_.at(bus_stops[j]).idx,
            .weight = time_sum
        });
        edges_description.push_back(BusActivity{
          .bus = bus_name,
          .span_count = ++span_count,
          .time = time_sum
        });
      }
    }
  }
  router = make_unique<Graph::Router<double>>(*road_graph);
}

std::optional<TransportManager::BuildedRouteInfo> TransportManager::GetRoute(const std::string& from, const std::string& to) const {
  if (!router) {
    BuildRoutes();
  }

  size_t from_id = 2 * stops_.at(from).idx;
  size_t to_id = 2 * stops_.at(to).idx;

  auto route_info = router->BuildRoute(from_id, to_id);

  if (!route_info) {
    return nullopt;
  }

  TransportManager::BuildedRouteInfo result;
  auto route_id = route_info.value().id;
  for (size_t i = 0; i < route_info.value().edge_count; ++i) {
    auto edge_id = router->GetRouteEdge(route_id, i);
    result.items.push_back(edges_description.at(edge_id));
  }
  result.total_time = route_info.value().weight;
  return result;
}