#pragma once

#include <set>
#include <string>
#include <vector>
#include <memory>
#include <variant>
#include <optional>
#include <unordered_map>
#include <unordered_set>

#include "graph.h"
#include "router.h"

class TransportManager {
public:
  struct RouteStats {
    size_t stops_count;
    size_t unique_stops_count;
    bool is_ring_route;
    std::optional<int> route_length_m = std::nullopt;
    double curvature;
  };

  struct StopStats {
    double latitude_rad;
    double longitude_rad;
  };

  struct WaitActivity {
    std::string stop_name;
    size_t time;
  };

  struct BusActivity {
    std::string bus;
    size_t span_count;
    double time;
  };

  struct BuildedRouteInfo {
    double total_time;
    std::vector<std::variant<WaitActivity, BusActivity>> items;
  };

  TransportManager() = default;

  void AddBusStop(const std::string& stop_name, double latitude, double longitude, const std::unordered_map<std::string, int>& distances);

  void AddBusRoute(const std::string& bus_name, const std::vector<std::string>& bus_stops, bool is_ring_route);

  std::optional<const RouteStats*> GetRouteStats(const std::string& bus_name) const;

  std::optional<const std::set<std::string>*> GetStopStats(const std::string& stop_name) const;

  void SetRoutingSettings(size_t bus_wait_time_min, double bus_velocity_km_per_h);

  std::optional<BuildedRouteInfo> GetRoute(const std::string& from, const std::string& to) const;

private:
  struct RoutingSettings {
    size_t bus_wait_time_min = 2.;
    double bus_velocity_km_per_h = 60.;
    double bus_velocity_m_per_min = 60. * 1000. / 60.;
  };

  struct Stop {
    StopStats stats;
    std::set<std::string> bus_names;
    std::unordered_map<std::string, int> distances;
    size_t idx;
  };

  struct Route {
    mutable RouteStats stats;
    std::vector<std::string> stop_names;
  };

  double PointDistance(const StopStats& lhs, const StopStats& rhs) const;
  void BuildRoutes() const;

  inline static constexpr double PI = 3.1415926535;
  inline static constexpr int EARTH_RADIUS_M = 6'371'000;

  std::unordered_map<std::string, Stop> stops_;
  std::unordered_map<std::string, Route> routes_;

  RoutingSettings settings_;
  mutable std::unique_ptr<Graph::DirectedWeightedGraph<double>> road_graph = nullptr;
  mutable std::unique_ptr<Graph::Router<double>> router = nullptr;
  mutable std::vector<std::variant<WaitActivity, BusActivity>> edges_description;
};