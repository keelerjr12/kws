#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "http.h"
#include "tcp_stream.h"
#include "tcp_server.h"
#include <functional>
#include <stdexcept>

namespace KWS {

  template<class Request, class Response>
  class HttpServer : public TcpServer {

   public:
    HttpServer(const char* host, int port) : TcpServer(host, port) { }

    void RegisterRoute(const HttpRoute& route, const std::function<Response(const Request&)> & handler) {
      auto route_it = handlers_.find(route);

      if (route_it != std::end(handlers_)) {
        throw std::invalid_argument("[Error]: HttpRoute already exists\n");
      }

      handlers_.insert({route, handler});
    }

   protected:

    void HandleClient(TcpStream& strm) override {
      auto req = Request::ParseFrom(strm);

      const auto route_handler_it = handlers_.find({req.Method(), req.URI()});

      if (route_handler_it == std::end(handlers_)) {
        throw std::invalid_argument("[Error]: HttpRoute does not exist\n");
      }

      const auto resp = route_handler_it->second(req);

      strm.Send(resp.Serialize());
    }

   private:

    std::unordered_map<HttpRoute, std::function<Response(const Request&)>> handlers_;
  };

}

#endif // HTTP_SERVER_H
