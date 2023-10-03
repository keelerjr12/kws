#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "tcp_stream.h"
#include <algorithm>
#include <cctype>
#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include <iostream>

namespace Keeler {

  enum class HttpMethod {
    GET,
    POST,
    PUT,
    DELETE
  };

  std::string toUpper(std::string str);
  HttpMethod ToHttpMethod(const std::string& str);
  std::string ToString(HttpMethod method);

  class HttpRequest {
   public:
    HttpRequest() { }

    HttpRequest(HttpMethod method, const std::string& uri) : 
      method_(method), uri_(uri) { }

    static HttpRequest ParseFrom(TcpStream& stream) {

      HttpRequest req;

      ParseRequestLine(req, stream);
      ParseHeaders(req, stream);

      return req;
    }

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
     std::string data;

     std::string Serialize() const {
       std::string resp;
       resp += "HTTP/1.1 200 OK\r\n";
       resp += "Content-Type: text/html\r\n";
       resp += "Content-Length: " + std::to_string(data.size()) + "\r\n\r\n";
       resp += data;

       std::cout << resp;
       return resp;
     }

   private:
  };

  class HttpServer {
   public:
    HttpServer(const char* host, int port);
    ~HttpServer();

    void RegisterHandler(HttpMethod method, const std::string& uri, const std::function<HttpResponse(const HttpRequest&)>& handler);
    void Serve();

   private:

    void Bind();
    void Listen(int backlog = 0) const;
    int Accept() const;
    std::string ReadRequest(int clientfd);

    const char* host_;
    int port_;
    int sockfd_;

    using MethodHandlerMap = std::unordered_map<HttpMethod, std::function<HttpResponse(const HttpRequest&)>>;
    std::unordered_map<std::string, MethodHandlerMap> handlers_;
  };

}

#endif // HTTP_SERVER_H
