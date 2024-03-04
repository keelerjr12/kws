#ifndef HTTP_SERVER_IMPL_H
#define HTTP_SERVER_IMPL_H

#include "../tcp/tcp_server.h"
#include <kws/http_status_code.h>
#include "router.h"
#include <functional>

namespace KWS {

class HttpRequest;
class HttpResponse;
class HttpRoute;
class TcpStream;

class HttpServerImpl : public TcpServer
{
  public:
    using Handler = std::function<HttpResponse(const HttpRequest&)>;

    HttpServerImpl(const char* host, int port);

    void RegisterRoute(const HttpRoute& route, Handler handler);
    void RegisterErrorHandler(HttpStatusCode code, Handler handler);

  protected:
    void HandleClient(TcpStream& strm) override;

  private:
    Router router;
};

}  // namespace KWS

#endif  // HTTP_SERVER_IMPL_H
