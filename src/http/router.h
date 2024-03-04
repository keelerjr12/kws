#ifndef ROUTER_H
#define ROUTER_H

#include <functional>
#include <kws/http_route.h>
#include <kws/http_status_code.h>
#include <unordered_map>

namespace KWS {

class HttpRequest;
class HttpResponse;
class HttpRoute;

class Router
{
  public:
    using Handler = std::function<HttpResponse(const HttpRequest&)>;

    HttpResponse Execute(const HttpRequest& req) const;

    void RegisterRoute(const HttpRoute& route, Handler handler);
    void RegisterErrorHandler(HttpStatusCode code, Handler handler);

  private:
    bool HandlerExists(const HttpRoute& route) const;
    bool ErrorHandlerExists(HttpStatusCode code) const;

    Handler GetHandler(const HttpRoute& route) const;
    Handler GetErrorHandler(HttpStatusCode code) const;

    std::unordered_map<HttpRoute, Handler> handlers_;
    std::unordered_map<HttpStatusCode, Handler> error_handlers_;
};

}  // namespace KWS

#endif  // ROUTER_H
