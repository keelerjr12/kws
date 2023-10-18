#include "http_method.h"
#include <algorithm>
#include <stdexcept>
#include <unordered_map>

namespace KWS {

  static const std::unordered_map<std::string, HttpMethod> method_map = {
    {"GET", HttpMethod::GET},
    {"POST", HttpMethod::POST},
    {"PUT", HttpMethod::PUT},
    {"DELETE", HttpMethod::DELETE}
  };
  
  // TODO: move this to a utility class
  std::string toUpper(std::string str) {
    std::transform(std::begin(str), std::end(str), std::begin(str), [](unsigned char chr) {
        return std::toupper(chr);
    });

    return str;
  }

  HttpMethod ToHttpMethod(const std::string& str) {
    auto method_it = method_map.find(toUpper(str));
    if (method_it == std::end(method_map)) {
      throw std::invalid_argument(str + " is in invalid HTTP method");
    }

    return method_it->second;
  }

  std::string ToString(HttpMethod method) {
    for (const auto& pair : method_map) {
      if (pair.second == method) {
        return pair.first;
      }
    }

    throw std::invalid_argument("Method cannot be converted to a string");
  }

}
