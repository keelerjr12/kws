#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "http.h"
#include "tcp_server.h"
#include <functional>
#include <stdexcept>

namespace KWS {

  class HttpRequest;
  class HttpResponse;
  class HttpRoute;
  class TcpStream;

  class HttpServer : public TcpServer {

   public:

    using Handler = std::function<HttpResponse(const HttpRequest&)>;

    HttpServer(const char* host, int port);

    void RegisterRoute(const HttpRoute& route, Handler handler);
    void RegisterErrorHandler(HttpStatusCode code, Handler handler);

   protected:

    void HandleClient(TcpStream& strm) override;

   private:

    bool HandlerExists(const HttpRoute& route) const;

    Handler GetHandler(const HttpRoute& route) const;
    Handler GetErrorHandler(HttpStatusCode code) const;

    std::unordered_map<HttpRoute, std::function<HttpResponse(const HttpRequest&)>> handlers_;
    std::unordered_map<HttpStatusCode, std::function<HttpResponse(const HttpRequest&)>> error_handlers_;
  };

}

#endif // HTTP_SERVER_H
