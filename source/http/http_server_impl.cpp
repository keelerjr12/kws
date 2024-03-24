#include <string>

#include "http_server_impl.h"

#include <kws/http_method.h>
#include <kws/http_request.h>
#include <kws/http_route.h>
#include <kws/http_status_code.h>

#include "../infrastructure/logger.h"
#include "../tcp/tcp_server.h"
#include "../tcp/tcp_stream.h"
#include "kws/http_response.h"
#include "router.h"

namespace KWS::Http
{

HttpServerImpl::HttpServerImpl(const std::string& host, const int port)
    : TcpServer(host, port)
{
}

void HttpServerImpl::RegisterRoute(const HttpRoute& route,
                                   const Handler& handler)
{
  router_.RegisterRoute(route, handler);
  KWS::Infrastructure::Log(KWS::Infrastructure::LogLevel::Info,
                           "Registered route {0} {1}",
                           ToString(route.Method()),
                           route.Path());
}

void HttpServerImpl::RegisterErrorHandler(HttpStatusCode code,
                                          const Handler& handler)
{
  router_.RegisterErrorHandler(code, handler);
  KWS::Infrastructure::Log(KWS::Infrastructure::LogLevel::Info,
                           "Registered error handler for {0}",
                           static_cast<int>(code));
}

void HttpServerImpl::HandleClient(TCP::TcpStream& strm)
{
  auto req = HttpRequest::ParseFrom(strm);
  KWS::Infrastructure::Log(KWS::Infrastructure::LogLevel::Info,
                           "Parsed request: {0} {1} {2}",
                           ToString(req.Method()),
                           req.Path(),
                           req.Version());

  const auto resp = router_.Execute(req);

  strm.Send(resp.Serialize());
}

}  // namespace KWS::Http
