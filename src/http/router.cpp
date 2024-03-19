#include "router.h"
#include "../infrastructure/logger.h"
#include <kws/http_request.h>
#include <kws/http_response.h>
#include <kws/http_route.h>
#include <optional>
#include <stdexcept>

namespace KWS::Http {

HttpResponse Router::Execute(HttpRequest& req) const
{
    // Get Route Template
    const auto routeTemplate = FindRouteTemplate(req.Method(), req.Path());

    if (!routeTemplate)
    {
        LOG_DEBUG("{0} {1} not found", ToString(req.Method()), req.Path());
        return HttpResponse{HttpStatusCode::NOT_FOUND,
                            "404 -- Resource Not Found"};
    }
    // Parameterize request
    const auto params = routeTemplate->ExtractParams(req.Path());
    for (const auto& [key, value] : params)
    {
        LOG_ERROR("{0} {1}", key, value);
        req.AddRouteValue(key, value);
    }

    // Get Handler
    const auto handler = handlers_.find(*routeTemplate)->second;
    // Call Handler w/request
    return handler(req);
}

void Router::RegisterRoute(const HttpRoute& route, const Handler& handler)
{
    const auto routeTemplate = FindRouteTemplate(route.Method(), route.Path());

    if (routeTemplate)
    {
        throw std::invalid_argument("[Error]: HttpRoute already exists\n");
    }

    handlers_.insert({{route.Method(), route.Path()}, handler});
}

void Router::RegisterErrorHandler(HttpStatusCode code, const Handler& handler)
{
    error_handlers_.insert({code, handler});
}

std::optional<RouteTemplate>
Router::FindRouteTemplate(const HttpMethod& method,
                          const std::string& path) const
{
    for (const auto& [routeTemplate, handler] : handlers_)
    {
        if (routeTemplate.Matches(path, method))
        {
            return routeTemplate;
        }
    }

    return {};
}

Router::Handler Router::GetErrorHandler(HttpStatusCode code) const
{
    const auto err_handler_it = error_handlers_.find(code);
    return err_handler_it->second;
}

}  // namespace KWS::Http
