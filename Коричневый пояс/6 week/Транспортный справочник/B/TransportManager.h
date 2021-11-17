#pragma once

#include <set>
#include <string>
#include <vector>
#include <optional>
#include <unordered_map>
#include <unordered_set>

class TransportManager {
public:
  struct RouteStats {
    size_t stops_count;
    size_t unique_stops_count;
    bool is_ring_route;
    std::optional<double> route_length_m = std::nullopt;
  };

  struct StopStats {
    double latitude_rad;
    double longitude_rad;
    std::set<std::string> bus_names;
  };

  TransportManager() = default;

  void AddBusStop(const std::string& name, double latitude, double longitude);

  void AddBusRoute(const std::string& bus_name, const std::vector<std::string>& bus_stops, bool is_ring_route);

  std::optional<const RouteStats*> GetRouteStats(const std::string& bus_name) const;

  std::optional<const StopStats*> GetStopStats(const std::string& stop_name) const;

private:
  struct Route {
    mutable RouteStats stats;
    std::vector<std::string> stop_names;
  };

  double PointDistance(const StopStats& lhs, const StopStats& rhs) const;

  inline static constexpr double PI = 3.1415926535;
  inline static constexpr int EARTH_RADIUS_M = 6371000;

  std::unordered_map<std::string, StopStats> stops_;
  std::unordered_map<std::string, Route> routes_;
};