#pragma once

#include <memory>
#include <string>
#include <vector>
#include <optional>
#include <string_view>
#include <unordered_map>

#include "TransportManager.h"

struct Request;
using RequestHolder = std::unique_ptr<Request>;

struct Request {
  enum class Type {
    MODIFY_STOP,
    MODIFY_BUS,
    READ_BUS
  };

  Request(Type type) : type(type) {}
  static RequestHolder Create(Type type);
  virtual void ParseFrom(std::string_view input) = 0;
  virtual ~Request() = default;

  const Type type;
};

template <typename ResultType>
struct ReadRequest : Request {
  using Request::Request;
  virtual ResultType Process(const TransportManager& manager) const = 0;

  inline static const std::unordered_map<std::string_view, Request::Type> STR_TO_REQUEST_TYPE = {
    {"Bus", Request::Type::READ_BUS}
  };
};

struct ModifyRequest : Request {
  using Request::Request;
  virtual void Process(TransportManager& manager) const = 0;

  inline static const std::unordered_map<std::string_view, Request::Type> STR_TO_REQUEST_TYPE = {
    {"Stop", Request::Type::MODIFY_STOP},
    {"Bus", Request::Type::MODIFY_BUS}
  };
};

struct AddStopRequest : ModifyRequest {
public:
  AddStopRequest();
  void ParseFrom(std::string_view input) override;
  void Process(TransportManager& manager) const override;
private:
  std::string stop_name;
  double latitude;
  double longitude;
};

struct AddBusRequest : ModifyRequest {
public:
  AddBusRequest();
  void ParseFrom(std::string_view input) override;
  void Process(TransportManager& manager) const override;
private:
  std::string bus_name;
  std::vector<std::string> stops;
  bool is_ring_route;
};

struct GetRouteInfoRequest : ReadRequest<void> { // 
public:
  GetRouteInfoRequest();
  void ParseFrom(std::string_view input) override;
  void Process(const TransportManager& manager) const override;
private:
  std::string bus_name;
};

std::pair<std::string_view, std::optional<std::string_view>> SplitTwoStrict(std::string_view s, std::string_view delimiter = " ");
std::pair<std::string_view, std::string_view> SplitTwo(std::string_view s, std::string_view delimiter = " ");
std::string_view ReadToken(std::string_view& s, std::string_view delimiter = " ");
int ConvertToInt(std::string_view str);
double ConvertToDouble(std::string_view str);

template <typename Number>
Number ReadNumberOnLine(std::istream& stream) {
  Number number;
  stream >> number;
  std::string dummy;
  getline(stream, dummy);
  return number;
}

template <typename RequestType>
std::optional<Request::Type> ConvertRequestTypeFromString(std::string_view type_str) {
  if (const auto it = RequestType::STR_TO_REQUEST_TYPE.find(type_str);
      it != RequestType::STR_TO_REQUEST_TYPE.end()) {
    return it->second;
  } else {
    return std::nullopt;
  }
}

template <typename RequestType>
RequestHolder ParseRequest(std::string_view request_str) {
  const auto request_type = ConvertRequestTypeFromString<RequestType>(ReadToken(request_str));
  if (!request_type) {
    return nullptr;
  }
  RequestHolder request = Request::Create(*request_type);
  if (request) {
    request->ParseFrom(request_str);
  };
  return request;
}

template <typename RequestType>
std::vector<RequestHolder> ReadRequests(std::istream& in_stream = std::cin) {
  const size_t request_count = ReadNumberOnLine<size_t>(in_stream);

  std::vector<RequestHolder> requests;
  requests.reserve(request_count);

  for (size_t i = 0; i < request_count; ++i) {
    std::string request_str;
    getline(in_stream, request_str);
    if (auto request = ParseRequest<RequestType>(request_str)) {
      requests.push_back(move(request));
    }
  }
  return requests;
}

void ProcessRequests(const std::vector<RequestHolder>& requests, TransportManager& manager);
