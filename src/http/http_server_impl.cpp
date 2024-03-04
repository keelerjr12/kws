#include "http_server_impl.h"
#include "../tcp/tcp_server.h"
#include "../tcp/tcp_stream.h"
#include "kws/http_response.h"
#include "router.h"
#include <kws/http_request.h>
#include <kws/http_route.h>
#include "../infrastructure/logger.h"

namespace KWS {

HttpServerImpl::HttpServerImpl(const char* host, int port)
  : TcpServer(host, port)
{}

void HttpServerImpl::RegisterRoute(const HttpRoute& route, Handler handler)
{
    router.RegisterRoute(route, std::move(handler));
    LOG_DEBUG("Registered route " + ToString(route.Method()) + " " + route.Path());
}

void HttpServerImpl::RegisterErrorHandler(HttpStatusCode code, Handler handler)
{
    router.RegisterErrorHandler(code, std::move(handler));
    LOG_DEBUG("Registered error handler for " + std::to_string(static_cast<int>(code)));
}

void HttpServerImpl::HandleClient(TcpStream& strm)
{
    auto req = HttpRequest::ParseFrom(strm);
    LOG_DEBUG("Parsed request: " + ToString(req.Method()) + " " + req.Path() + " " + req.Version());

    const auto resp = router.Execute(req);

    strm.Send(resp.Serialize());
}

}  // namespace KWS
