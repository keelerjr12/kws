#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <functional>
#include <kws/http_status_code.h>
#include <memory>

namespace KWS {

class HttpRequest;
class HttpResponse;
class HttpRoute;
class HttpServerImpl;

class HttpServer
{
  public:
    using Handler = std::function<HttpResponse(const HttpRequest&)>;

    HttpServer(const char* host, int port);
    virtual ~HttpServer();

    void Serve();

    void RegisterRoute(const HttpRoute& route, Handler handler);
    void RegisterErrorHandler(HttpStatusCode code, Handler handler);

  private:
    std::unique_ptr<HttpServerImpl> impl_;
};

}  // namespace KWS

#endif  // HTTP_SERVER_H
