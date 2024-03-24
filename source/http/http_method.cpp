#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include <kws/http_method.h>

namespace
{
const std::unordered_map<std::string, KWS::Http::HttpMethod>& GetHttpMethodMap()
{
  static const std::unordered_map<std::string, KWS::Http::HttpMethod>
      MethodMap {{"GET", KWS::Http::HttpMethod::GET},
                 {"POST", KWS::Http::HttpMethod::POST},
                 {"PUT", KWS::Http::HttpMethod::PUT},
                 {"DELETE", KWS::Http::HttpMethod::DELETE}};

  return MethodMap;
}
}  // namespace

namespace KWS::Http
{

// TODO: move this to a utility class
std::string ToUpper(std::string str)
{
  std::transform(std::begin(str),
                 std::end(str),
                 std::begin(str),
                 [](unsigned char chr) { return std::toupper(chr); });

  return str;
}

HttpMethod ToHttpMethod(const std::string& str)
{
  const auto& methodMap = GetHttpMethodMap();

  auto methodIt = methodMap.find(ToUpper(str));
  if (methodIt == std::end(methodMap)) {
    throw std::invalid_argument(str + " is in invalid HTTP method");
  }

  return methodIt->second;
}

std::string ToString(HttpMethod method)
{
  for (const auto& pair : GetHttpMethodMap()) {
    if (pair.second == method) {
      return pair.first;
    }
  }

  throw std::invalid_argument("Method cannot be converted to a string");
}

}  // namespace KWS::Http
