#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "http.h"
#include <functional>
#include <string>
#include <unordered_map>

namespace KWS {

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
