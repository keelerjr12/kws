#include <kws/http_server.h>
#include "http_server_impl.h"
#include <memory>

namespace KWS {

HttpServer::HttpServer(const char* host, int port)
  : impl_(std::make_unique<HttpServerImpl>(host, port))
{}

HttpServer::~HttpServer() = default;

void HttpServer::Serve()
{
    impl_->Serve();
}

void HttpServer::RegisterRoute(const HttpRoute& route, Handler handler)
{
    impl_->RegisterRoute(route, std::move(handler));
}

void HttpServer::RegisterErrorHandler(HttpStatusCode code, Handler handler)
{
    impl_->RegisterErrorHandler(code, std::move(handler));
}

}  // namespace KWS
