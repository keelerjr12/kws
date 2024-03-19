#include "http_server_impl.h"
#include <kws/http_server.h>
#include <memory>

namespace KWS::Http {

HttpServer::HttpServer(const std::string& host, const int port) :
  impl_(std::make_unique<KWS::Http::HttpServerImpl>(host, port))
{}

HttpServer::~HttpServer() = default;

void HttpServer::Serve()
{
    impl_->Serve();
}

void HttpServer::RegisterRoute(const HttpRoute& route, const Handler& handler)
{
    impl_->RegisterRoute(route, handler);
}

void HttpServer::RegisterErrorHandler(HttpStatusCode code, const Handler& handler)
{
    impl_->RegisterErrorHandler(code, handler);
}

}  // namespace KWS::Http
