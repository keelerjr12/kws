#include <string>
#include <string_view>
#include <vector>

#include "str_util.h"

namespace KWS::Utility
{

std::string LStrip(std::string_view str, char chr)
{
  const auto loc = str.find_first_not_of(chr);

  if (loc == std::string::npos) {
    return "";
  }

  return std::string(str.substr(loc));
}

std::string RStrip(std::string_view str, char chr)
{
  const auto loc = str.find_last_not_of(chr);

  if (loc == std::string::npos) {
    return "";
  }

  return std::string(str.substr(0, loc + 1));
}

std::string Strip(std::string_view str, char chr)
{
  auto strippedStr = LStrip(str, chr);
  strippedStr = RStrip(strippedStr, chr);

  return strippedStr;
}

std::vector<std::string> Split(std::string_view url, char delim)
{
  std::vector<std::string> segments;

  auto urlStr = Strip(url, delim);
  auto loc = urlStr.find(delim);

  while (loc != std::string::npos) {
    const auto segment = urlStr.substr(0, loc);
    segments.push_back(segment);

    urlStr = urlStr.substr(loc + 1);
    loc = urlStr.find(delim);
  }

  segments.emplace_back(urlStr);

  return segments;
}

}  // namespace KWS::Utility
