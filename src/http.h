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

#endif // HTTP_H
