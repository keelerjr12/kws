#ifndef HTTP_H
#define HTTP_H

#include "tcp_stream.h"
#include <string>
#include <unordered_map>

namespace KWS {

  enum class HttpMethod {
    GET,
    POST,
    PUT,
    DELETE
  };

  HttpMethod ToHttpMethod(const std::string& str);
  std::string ToString(HttpMethod method);

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

  class HttpRequest {
   public:
    HttpRequest() = default;
    HttpRequest(HttpMethod method, const std::string& uri);

    static HttpRequest ParseFrom(TcpStream& stream);

    HttpMethod Method() const;
    std::string URI() const;
    std::string Version() const;
    std::string Header(const std::string& header) const;

   private:
    static void ParseRequestLine(HttpRequest& req, TcpStream& stream);
    static void ParseHeaders(HttpRequest& req, TcpStream& stream);

    HttpMethod method_;
    std::string uri_;
    std::string version_;

    std::unordered_map<std::string, std::string> headers_;
  };

  class HttpResponse {
   public:
     HttpResponse(const std::string& content);

     std::string Serialize() const;

   private:
     std::string content_;
  };

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

#endif // HTTP_H
