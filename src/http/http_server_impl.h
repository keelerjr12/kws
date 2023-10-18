#ifndef HTTP_SERVER_IMPL_H
#define HTTP_SERVER_IMPL_H

#include <functional>
#include <unordered_map>
#include "http_route.h"
#include "http_status_code.h"
#include "../tcp/tcp_server.h"

namespace KWS {

  class HttpRequest;
  class HttpResponse;
  class TcpStream;

  class HttpServerImpl : public TcpServer {

   public:

    using Handler = std::function<HttpResponse(const HttpRequest&)>;

    HttpServerImpl(const char* host, int port);

    void RegisterRoute(const HttpRoute& route, Handler handler);
    void RegisterErrorHandler(HttpStatusCode code, Handler handler);

   protected:

    void HandleClient(TcpStream& strm) override;

   private:

    bool HandlerExists(const HttpRoute& route) const;
    bool ErrorHandlerExists(HttpStatusCode code) const;

    Handler GetHandler(const HttpRoute& route) const;
    Handler GetErrorHandler(HttpStatusCode code) const;

    std::unordered_map<HttpRoute, Handler> handlers_;
    std::unordered_map<HttpStatusCode, Handler> error_handlers_;
  };

}

#endif // HTTP_SERVER_IMPL_H
