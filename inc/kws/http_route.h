#ifndef HTTP_ROUTE_H
#define HTTP_ROUTE_H

#include <kws/http_method.h>
#include <string>

namespace KWS {

class HttpRoute
{
  public:
    HttpRoute(HttpMethod method, const std::string& path)
      : method_(method)
      , path_(path)
    {}

    HttpMethod Method() const
    {
        return method_;
    }

    std::string Path() const
    {
        return path_;
    }

  private:
    HttpMethod method_;
    std::string path_;
};

bool operator==(const HttpRoute& lhs, const HttpRoute& rhs);

}  // namespace KWS

namespace std {

template<>
struct hash<KWS::HttpRoute>
{
    size_t operator()(const KWS::HttpRoute& route) const
    {
        const auto method_hash_fn = hash<int>{};
        const auto uri_hash_fn = hash<std::string>{};

        return method_hash_fn((int)route.Method()) ^ uri_hash_fn(route.Path());
    }
};

}  // namespace std

#endif  // HTTP_ROUTE_H
