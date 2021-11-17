#include <sstream>

#include "json.h"

using namespace std;

namespace Json {

  Document::Document(Node root) : root(move(root)) {
  }

  const Node& Document::GetRoot() const {
    return root;
  }

  Node LoadNode(istream& input);

  Node LoadArray(istream& input) {
    vector<Node> result;

    for (char c; input >> c && c != ']'; ) {
      if (c != ',') {
        input.putback(c);
      }
      result.push_back(LoadNode(input));
    }

    return Node(move(result));
  }

  Node LoadInt(istream& input) {
    int result = 0;
    while (isdigit(input.peek())) {
      result *= 10;
      result += input.get() - '0';
    }
    return Node(result);
  }

  Node LoadString(istream& input) {
    string line;
    getline(input, line, '"');
    return Node(move(line));
  }

  Node LoadDict(istream& input) {
    map<string, Node> result;

    for (char c; input >> c && c != '}'; ) {
      if (c == ',') {
        input >> c;
      }

      string key = LoadString(input).AsString();
      input >> c;
      result.emplace(move(key), LoadNode(input));
    }

    return Node(move(result));
  }

  std::variant<int, double> ReadNumber(std::istream& is) {
    int int_part;
    double double_part;
    is >> int_part;
    if (is.peek() == '.') {
      is >> double_part;
      return int_part + double_part;
    } else {
      return int_part;
    }
  }

  Node LoadNumber(istream& input) {
    auto result = ReadNumber(input);
    if (std::holds_alternative<int>(result))
      return Node(std::get<int>(result));
    else
      return Node(std::get<double>(result));
  } 

  Node LoadBoolean(istream& input) {
    bool result;
    input >> boolalpha >> result;
    return Node(result);
  }

  Node LoadNode(istream& input) {
    char c;
    input >> c;

    if (c == '[') {
      return LoadArray(input);
    } else if (c == '{') {
      return LoadDict(input);
    } else if (c == '"') {
      return LoadString(input);
    } else if (std::isdigit(c) || c == '-') {
      input.putback(c);
      return LoadNumber(input);
    } else {
      input.putback(c);
      return LoadBoolean(input);
    }
  }

  Document Load(istream& input) {
    return Document{LoadNode(input)};
  }

  string Node::ToString() const {
    stringstream ss;
    if (IsArray()) {
      ss << '[';
      for (auto& x : AsArray()) {
        ss << x.ToString() << ',';
      } 
      auto out = ss.str();
      if (out == "[") {
        out.push_back(']');
      }
      else {
        out.back() = ']';
      }
      return out;
    } else if (IsMap()) {
      ss << '{';
      for (const auto& [key, value] : AsMap()) {
        ss << "\"" << key << "\":" << value.ToString() << ',';  
      }
      auto out = ss.str();
      if (out == "{") {
        out.push_back('}');
      } else {
        out.back() = '}';
      }
      return out;
    } else if (IsDouble()) {
      ss << AsDouble();
      return ss.str();
    } else if (IsInt()) {
      ss << AsInt();
      return ss.str();
    } else if (IsString()) {
      ss << '"' << AsString() << '"';
      return ss.str();
    } else if (IsBoolean()) {
      return AsBoolean() ? "true" : "false";
    } else {
      return "";
    }  
  }
}
