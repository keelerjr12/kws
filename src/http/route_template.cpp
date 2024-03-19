#include "route_template.h"
#include <string>
#include "../utility/str_util.h"
#include <vector>

namespace KWS::Http {

// TODO: need to account for http method
bool operator==(const RouteTemplate& lhs, const RouteTemplate& rhs)
{
    return lhs.Pattern() == rhs.Pattern();
}

bool IsParameter(const std::string& segment)
{
    return !segment.empty() && segment.front() == '{' && segment.back() == '}';
}

RouteTemplate::RouteTemplate(const HttpMethod& method, std::string pattern) :
  method_(method), pattern_(std::move(pattern))
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
    if (method_ != method)
    {
        return false;
    }

    const auto path_tokens = Utility::Split(path, '/');

    if (path_tokens.size() != segments_.size())
    {
        return false;
    }

    for (std::size_t i = 0; i < path_tokens.size(); ++i)
    {
        if (!IsParameter(segments_[i]) && (segments_[i] != path_tokens[i]))
        {
            return false;
        }
    }

    return true;
}

// TODO: need to get rid of magic values
std::vector<std::pair<std::string, std::string>>
RouteTemplate::ExtractParams(std::string_view path) const
{
    std::vector<std::pair<std::string, std::string>> params;

    const auto path_tokens = Utility::Split(path, '/');
    for (std::size_t i = 0; i < path_tokens.size(); ++i)
    {
        if (IsParameter(segments_[i]))
        {
            auto key = Utility::Strip(segments_[i], '{');
            key = Utility::Strip(key, '}');
            params.push_back({key, path_tokens[i]});
        }
    }

    return params;
}

void RouteTemplate::Parse()
{
    segments_ = Utility::Split(pattern_, '/');

    for (const auto& segment : segments_)
    {
        if (IsParameter(segment))
        {
            params_.push_back(segment);
        }
    }
}
}  // namespace KWS::Http
