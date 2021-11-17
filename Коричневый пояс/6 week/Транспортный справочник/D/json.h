#pragma once

#include <istream>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace Json {

  class Node : std::variant<std::vector<Node>,
                            std::map<std::string, Node>,
                            int, double, bool,
                            std::string> {
  public:
    using variant::variant;

    const auto& AsArray() const {
      return std::get<std::vector<Node>>(*this);
    }

    auto& AsArray() {
      return std::get<std::vector<Node>>(*this);
    }

    bool IsArray() const {
      return std::holds_alternative<std::vector<Node>>(*this);
    }

    const auto& AsMap() const {
      return std::get<std::map<std::string, Node>>(*this);
    }

    auto& AsMap() {
      return std::get<std::map<std::string, Node>>(*this);
    }

    bool IsMap() const {
      return std::holds_alternative<std::map<std::string, Node>>(*this);
    }

    const auto& AsString() const {
      return std::get<std::string>(*this);
    }

    auto& AsString() {
      return std::get<std::string>(*this);
    }

    bool IsString() const { 
      return std::holds_alternative<std::string>(*this); 
    }

    int AsInt() const {
      return std::get<int>(*this);
    }

    int& AsInt() {
      return std::get<int>(*this);
    }

    bool IsInt() const { 
      return std::holds_alternative<int>(*this); 
    }

    double AsDouble() const {
      if (IsDouble())
        return std::get<double>(*this);
      else
        return AsInt();
    }

    double& AsDouble() { 
      return std::get<double>(*this); 
    }

    bool IsDouble() const { 
      return std::holds_alternative<double>(*this); 
    }

    bool AsBoolean() const { 
      return std::get<bool>(*this); 
    }

    bool& AsBoolean() { 
      return std::get<bool>(*this); 
    }

    bool IsBoolean() const { 
      return std::holds_alternative<bool>(*this); 
    }

    std::string ToString() const;
  };

  class Document {
  public:
    explicit Document(Node root);

    const Node& GetRoot() const;

  private:
    Node root;
  };

  Document Load(std::istream& input);

}
