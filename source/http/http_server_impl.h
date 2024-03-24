#ifndef HTTP_SERVER_IMPL_H
#define HTTP_SERVER_IMPL_H

#include <functional>

#include <kws/http_status_code.h>

#include "../tcp/tcp_server.h"
#include "router.h"

namespace KWS::TCP
{
class TcpStream;
}  // namespace KWS::TCP

namespace KWS::Http
{

class HttpRequest;
class HttpResponse;
class HttpRoute;

class HttpServerImpl : public TCP::TcpServer
{
public:
  using Handler = std::function<HttpResponse(const HttpRequest&)>;

  HttpServerImpl(const std::string& host, int port);

  void RegisterRoute(const HttpRoute& route, const Handler& handler);
  void RegisterErrorHandler(HttpStatusCode code, const Handler& handler);

protected:
  void HandleClient(TCP::TcpStream& strm) override;

private:
  Router router_;
};
}  // namespace KWS::Http

#endif  // HTTP_SERVER_IMPL_H
