#include "http_server_impl.h"
#include "../tcp/tcp_server.h"
#include "../tcp/tcp_stream.h"
#include "http_request.h"
#include "http_response.h"
#include "http_route.h"
#include "router.h"

namespace KWS {

HttpServerImpl::HttpServerImpl(const char* host, int port)
  : TcpServer(host, port)
{}

void HttpServerImpl::RegisterRoute(const HttpRoute& route, Handler handler)
{
    router.RegisterRoute(route, std::move(handler));
}

void HttpServerImpl::RegisterErrorHandler(HttpStatusCode code, Handler handler)
{
    router.RegisterErrorHandler(code, std::move(handler));
}

void HttpServerImpl::HandleClient(TcpStream& strm)
{
    auto req = HttpRequest::ParseFrom(strm);

    const auto resp = router.Execute(req);

    strm.Send(resp.Serialize());
}

}  // namespace KWS
