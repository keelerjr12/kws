#include "router.h"
#include "http_request.h"
#include "http_response.h"
#include "http_route.h"
#include <stdexcept>

#include <iostream>

namespace KWS {

HttpResponse Router::Execute(const HttpRequest& req) const
{
    if (!HandlerExists({req.Method(), req.Path()}))
    {
        throw std::runtime_error("HTTP route does not exist");
    }

    const auto handler = GetHandler({req.Method(), req.Path()});

    return handler(req);
}

void Router::RegisterRoute(const HttpRoute& route, Handler handler)
{
    if (HandlerExists(route))
    {
        throw std::invalid_argument("[Error]: HttpRoute already exists\n");
    }

    handlers_.insert({route, std::move(handler)});
}

void Router::RegisterErrorHandler(HttpStatusCode code, Handler handler)
{
    error_handlers_.insert({code, std::move(handler)});
}

bool Router::HandlerExists(const HttpRoute& route) const
{
    return handlers_.find(route) != std::end(handlers_);
}

std::string LStrip(const std::string& str, char chr = ' ')
{
    const auto loc = str.find_first_not_of(chr);

    if (loc == std::string::npos)
    {
        return "";
    }

    return str.substr(loc);
}

std::string RStrip(const std::string& str, char chr = ' ')
{
    const auto loc = str.find_last_not_of(chr);

    if (loc == std::string::npos)
    {
        return "";
    }

    return str.substr(0, loc + 1);
}

std::string Strip(const std::string& str, char chr = ' ')
{
    auto stripped_str = LStrip(str, chr);
    stripped_str = RStrip(stripped_str, chr);

    return stripped_str;
}

// TODO: move this!
std::vector<std::string> Tokenize(std::string url)
{
    std::vector<std::string> segments;

    url = Strip(url, '/');

    auto loc = url.find('/');

    while (loc != std::string::npos)
    {
        const auto segment = url.substr(0, loc);
        segments.push_back(segment);

        url = url.substr(loc + 1);
        loc = url.find('/');
    }

    segments.push_back(url);

    return segments;
}

bool IsParameter(const std::string& segment)
{
    return !segment.empty() && segment.front() == '{' && segment.back() == '}';
}

bool AreEqual(const std::vector<std::string>& lhs,
              const std::vector<std::string>& rhs)
{
    if (lhs.size() != rhs.size())
    {
        return false;
    }

    for (auto i = 0; i < lhs.size(); ++i)
    {
        if (IsParameter(rhs[i]))
        {
            continue;
        }

        if (lhs[i] != rhs[i])
        {
            return false;
        }
    }

    return true;
}

Router::Handler Router::GetHandler(const HttpRoute& route) const
{
    /*const auto route_handler_it = handlers_.find(route);
    return route_handler_it->second;*/

    const auto route_segments = Tokenize(route.Path());

    /*for (const auto& segment : route_segments) {
      std::cout << segment << "--";
    }

    std::cout << std::endl;*/

    for (const auto& handler : handlers_)
    {
        const auto handler_segments = Tokenize(handler.first.Path());

        /*for (const auto& segment : handler_segments) {
        std::cout << segment << "--";
      }

      std::cout << std::endl;*/

        if (AreEqual(route_segments, handler_segments))
        {
            return handler.second;
        }
    }

    throw std::runtime_error("[ERROR]: No handler found");
}

Router::Handler Router::GetErrorHandler(HttpStatusCode code) const
{
    const auto err_handler_it = error_handlers_.find(code);
    return err_handler_it->second;
}

}  // namespace KWS
