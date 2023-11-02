#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include "http_method.h"
#include <string>
#include <unordered_map>

namespace KWS {

  class TcpStream;

  class HttpRequest {
   public:
    HttpRequest() = default;
    HttpRequest(HttpMethod method, std::string path);

    static HttpRequest ParseFrom(TcpStream& stream);

    HttpMethod Method() const;
    std::string Path() const;
    std::string Version() const;
    std::string Header(const std::string& header) const;

   private:
    static void ParseRequestLine(HttpRequest& req, TcpStream& stream);
    static void ParseHeaders(HttpRequest& req, TcpStream& stream);

    HttpMethod method_;
    std::string path_;
    std::string version_;

    std::unordered_map<std::string, std::string> headers_;
  };

}

#endif // HTTP_REQUEST_H
