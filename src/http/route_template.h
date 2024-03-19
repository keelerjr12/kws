#ifndef ROUTE_TEMPLATE_H
#define ROUTE_TEMPLATE_H

#include "kws/http_method.h"
#include <string>
#include <string_view>
#include <vector>

namespace KWS::Http {

class RouteTemplate
{
  public:
    RouteTemplate(const HttpMethod& method, std::string pattern);

    const std::string& Pattern() const;
    const std::vector<std::string>& Segments() const;
    const std::vector<std::string>& Parameters() const;

    bool Matches(std::string_view path, const HttpMethod& method) const;

    std::vector<std::pair<std::string, std::string>>
    ExtractParams(std::string_view path) const;

  private:
    void Parse();

    HttpMethod method_;
    std::string pattern_;
    std::vector<std::string> segments_;
    std::vector<std::string> params_;
};

bool operator==(const RouteTemplate& lhs, const RouteTemplate& rhs);
}  // namespace KWS::Http

namespace std {

template<>
struct hash<KWS::Http::RouteTemplate>
{
    size_t operator()(const KWS::Http::RouteTemplate& rt) const
    {
        const auto uri_hash_fn = hash<std::string>{};

        return uri_hash_fn(rt.Pattern());
    }
};

}  // namespace std

#endif  // ROUTE_TEMPLATE_H
