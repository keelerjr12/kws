#include "http_server.h"
#include "http_server_impl.h"
#include "../tcp/tcp_server.h"
#include <stdexcept>
#include <memory>

namespace KWS {

  HttpServer::HttpServer(const char* host, int port) : impl_(std::make_unique<HttpServerImpl>(host, port)) { }

  HttpServer::~HttpServer() {
    // Empty to allow the use of unique_ptr with incomplete type
  }

  void HttpServer::Serve() {
    impl_->Serve();
  }

  void HttpServer::RegisterRoute(const HttpRoute& route, Handler handler) {
    impl_->RegisterRoute(route, handler);
  }

  void HttpServer::RegisterErrorHandler(HttpStatusCode code, Handler handler) {
    impl_->RegisterErrorHandler(code, handler);
  }

}
