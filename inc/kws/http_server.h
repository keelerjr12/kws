#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <functional>
#include <kws/http_status_code.h>
#include <memory>

namespace KWS::Http {

class HttpRequest;
class HttpResponse;
class HttpRoute;
class HttpServerImpl;

class HttpServer
{
  public:
    using Handler = std::function<HttpResponse(const HttpRequest&)>;

    HttpServer(const std::string& host, int port);
    virtual ~HttpServer();

    void Serve();

    void RegisterRoute(const HttpRoute& route, const Handler& handler);
    void RegisterErrorHandler(HttpStatusCode code, const Handler& handler);

  private:
    std::unique_ptr<Http::HttpServerImpl> impl_;
};

}  // namespace KWS::Http

#endif  // HTTP_SERVER_H
