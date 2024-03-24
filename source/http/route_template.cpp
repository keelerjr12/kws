#include <cstddef>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "route_template.h"

#include <kws/http_method.h>

#include "../utility/str_util.h"

namespace KWS::Http
{

// TODO: need to account for http method
bool operator==(const RouteTemplate& lhs, const RouteTemplate& rhs)
{
  return lhs.Pattern() == rhs.Pattern();
}

bool IsParameter(const std::string& segment)
{
  return !segment.empty() && segment.front() == '{' && segment.back() == '}';
}

RouteTemplate::RouteTemplate(const HttpMethod& method, std::string pattern)
    : method_(method)
    , pattern_(std::move(pattern))
{
  Parse();
}

const std::string& RouteTemplate::Pattern() const
{
  return pattern_;
}

const std::vector<std::string>& RouteTemplate::Segments() const
{
  return segments_;
}

const std::vector<std::string>& RouteTemplate::Parameters() const
{
  return params_;
}

bool RouteTemplate::Matches(std::string_view path,
                            const HttpMethod& method) const
{
  if (method_ != method) {
    return false;
  }

  const auto pathTokens = Utility::Split(path, '/');

  if (pathTokens.size() != segments_.size()) {
    return false;
  }

  for (std::size_t i = 0; i < pathTokens.size(); ++i) {
    if (!IsParameter(segments_[i]) && (segments_[i] != pathTokens[i])) {
      return false;
    }
  }

  return true;
}

// TODO: need to get rid of magic values
std::vector<std::pair<std::string, std::string>> RouteTemplate::ExtractParams(
    std::string_view path) const
{
  std::vector<std::pair<std::string, std::string>> params;

  const auto pathTokens = Utility::Split(path, '/');
  for (std::size_t i = 0; i < pathTokens.size(); ++i) {
    if (IsParameter(segments_[i])) {
      auto key = Utility::Strip(segments_[i], '{');
      key = Utility::Strip(key, '}');
      params.emplace_back(key, pathTokens[i]);
    }
  }

  return params;
}

void RouteTemplate::Parse()
{
  segments_ = Utility::Split(pattern_, '/');

  for (const auto& segment : segments_) {
    if (IsParameter(segment)) {
      params_.push_back(segment);
    }
  }
}
}  // namespace KWS::Http
