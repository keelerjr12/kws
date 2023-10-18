#ifndef HTTP_ROUTE_H
#define HTTP_ROUTE_H

#include <string>
#include <unordered_map>
#include "http_method.h"

namespace KWS {

  class TcpStream;

  class HttpRoute {
   public:

    HttpRoute(HttpMethod method, const std::string& uri) : method_(method), uri_(uri) { }

    HttpMethod Method() const {
      return method_;
    }

    std::string URI() const {
      return uri_;
    }

   private:
    HttpMethod method_;
    std::string uri_;
  };

  bool operator==(const HttpRoute& lhs, const HttpRoute& rhs);

}

namespace std {

  template <>
  struct hash<KWS::HttpRoute> {

    size_t operator()(const KWS::HttpRoute& route) const noexcept {
      const auto method_hash_fn = hash<int>{};
      const auto uri_hash_fn = hash<std::string>{};
      
      return method_hash_fn((int)route.Method()) ^ uri_hash_fn(route.URI());
    }

  };

}

#endif // HTTP_ROUTE_H
