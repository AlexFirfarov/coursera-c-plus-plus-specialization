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
    double curvature;
  };

  struct StopStats {
    double latitude_rad;
    double longitude_rad;
  };

  TransportManager() = default;

  void AddBusStop(const std::string& stop_name, double latitude, double longitude, const std::unordered_map<std::string, int>& distances);

  void AddBusRoute(const std::string& bus_name, const std::vector<std::string>& bus_stops, bool is_ring_route);

  std::optional<const RouteStats*> GetRouteStats(const std::string& bus_name) const;

  std::optional<const std::set<std::string>*> GetStopStats(const std::string& stop_name) const;

private:
  struct Stop {
    StopStats stats;
    std::set<std::string> bus_names;
    std::unordered_map<std::string, int> distances;
  };

  struct Route {
    mutable RouteStats stats;
    std::vector<std::string> stop_names;
  };

  double PointDistance(const StopStats& lhs, const StopStats& rhs) const;

  inline static constexpr double PI = 3.1415926535;
  inline static constexpr int EARTH_RADIUS_M = 6371000;

  std::unordered_map<std::string, Stop> stops_;
  std::unordered_map<std::string, Route> routes_;
};