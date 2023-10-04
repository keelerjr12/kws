#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "http.h"
#include "tcp_stream.h"
#include "tcp_server.h"
#include <functional>

namespace KWS {

  template<class Request, class Response>
  class HttpServer : public TcpServer {

   public:
    HttpServer(const char* host, int port) : TcpServer(host, port) { }

    void RegisterHandler(HttpMethod method, const std::string& uri, const std::function<Response(const Request&)> & handler) {
      auto uri_it = handlers_.find(uri);

      if (uri_it != std::end(handlers_)) {
        uri_it->second.insert({method, handler});
      } else {
        std::unordered_map<HttpMethod, std::function<Response(const Request&)>> new_map;
        new_map.insert({method, handler});
        handlers_.insert({uri, new_map});
      }
    }

   protected:

    void HandleClient(TcpStream& strm) override {
      auto req = Request::ParseFrom(strm);

      const auto uri_map = handlers_.find(req.URI());
      const auto handler = uri_map->second.find(req.Method());
      const auto resp = handler->second(req);

      strm.Send(resp.Serialize());
    }

   private:

    using MethodHandlerMap = std::unordered_map<HttpMethod, std::function<Response(const Request&)>>;
    std::unordered_map<std::string, MethodHandlerMap> handlers_;
  };

}

#endif // HTTP_SERVER_H
