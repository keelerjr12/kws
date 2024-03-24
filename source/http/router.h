#ifndef ROUTER_H
#define ROUTER_H

#include "route_template.h"
#include <functional>
#include <kws/http_route.h>
#include <kws/http_status_code.h>
#include <optional>
#include <unordered_map>

namespace KWS::Http {

class HttpRequest;
class HttpResponse;
class HttpRoute;

class Router
{
  public:
    using Handler = std::function<HttpResponse(const HttpRequest&)>;

    HttpResponse Execute(HttpRequest& req) const;

    void RegisterRoute(const HttpRoute& route, const Handler& handler);
    void RegisterErrorHandler(HttpStatusCode code, const Handler& handler);

  private:
    std::optional<RouteTemplate>
    FindRouteTemplate(const HttpMethod& method, const std::string& path) const;

    bool ErrorHandlerExists(HttpStatusCode code) const;
    Handler GetErrorHandler(HttpStatusCode code) const;

    std::unordered_map<RouteTemplate, Handler> handlers_;
    std::unordered_map<HttpStatusCode, Handler> errorHandlers_;
};

}  // namespace KWS::Http

#endif  // ROUTER_H
