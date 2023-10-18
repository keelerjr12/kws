#include "http_server_impl.h"
#include "http_request.h"
#include "http_response.h"
#include "http_route.h"
#include "../tcp/tcp_server.h"
#include "../tcp/tcp_stream.h"
#include <stdexcept>

namespace KWS {

  HttpServerImpl::HttpServerImpl(const char* host, int port) : TcpServer(host, port) { }

  void HttpServerImpl::RegisterRoute(const HttpRoute& route, Handler handler) {
    if (HandlerExists(route)) {
      throw std::invalid_argument("[Error]: HttpRoute already exists\n");
    }

    handlers_.insert({route, std::move(handler)});
  }

  void HttpServerImpl::RegisterErrorHandler(HttpStatusCode code, Handler handler) {
    error_handlers_.insert({code, std::move(handler)});
  }

  void HttpServerImpl::HandleClient(TcpStream& strm) {
    auto req = HttpRequest::ParseFrom(strm);

    const auto route = HttpRoute(req.Method(), req.URI());

    if (HandlerExists(route)) {
      const auto handler = GetHandler(route);
      const auto resp = handler(req);
      strm.Send(resp.Serialize());
    } else {
      const auto err_handler = GetErrorHandler(HttpStatusCode::BAD_REQUEST);
      const auto resp = err_handler(req);
      strm.Send(resp.Serialize());

      // TODO: this needs to be moved up for exceptional cases
      //throw std::invalid_argument("[Error]: HttpRoute does not exist\n");
    }
  }

  bool HttpServerImpl::HandlerExists(const HttpRoute& route) const {
    return handlers_.find(route) != std::end(handlers_);
  }

  HttpServerImpl::Handler HttpServerImpl::GetHandler(const HttpRoute& route) const {
    const auto route_handler_it = handlers_.find(route);
    return route_handler_it->second;
  }

  HttpServerImpl::Handler HttpServerImpl::GetErrorHandler(HttpStatusCode code) const {
    const auto err_handler_it = error_handlers_.find(code);
    return err_handler_it->second;
  }

}

