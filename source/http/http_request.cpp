#include "../tcp/tcp_stream.h"
#include <kws/http_method.h>
#include <kws/http_request.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace KWS::Http {

HttpRequest::HttpRequest(HttpMethod method, std::string path) :
  method_(method), path_(std::move(path))
{}

HttpRequest HttpRequest::ParseFrom(TCP::TcpStream& stream)
{
    HttpRequest req;

    ParseRequestLine(req, stream);
    ParseHeaders(req, stream);

    return req;
}

HttpMethod HttpRequest::Method() const
{
    return method_;
}

std::string HttpRequest::Path() const
{
    return path_;
}

std::string HttpRequest::Version() const
{
    return version_;
}

std::string HttpRequest::Header(const std::string& header) const
{
    const auto headerIt = headers_.find(header);

    if (headerIt == std::end(headers_))
    {
        throw std::invalid_argument(header + " does not exist.");
    }

    return headerIt->second;
}

void HttpRequest::AddRouteValue(const std::string& param,
                                const std::string& value)
{
    params_.insert({param, value});
}

void HttpRequest::ParseRequestLine(HttpRequest& req, TCP::TcpStream& stream)
{
    auto requestLine = stream.ReceiveLine();
    std::istringstream iss{requestLine};
    std::string method;

    iss >> method >> req.path_ >> req.version_;

    req.method_ = ToHttpMethod(method);
}

void HttpRequest::ParseHeaders(HttpRequest& req, TCP::TcpStream& stream)
{
    const std::string delim = ": ";

    auto headerLine = stream.ReceiveLine();

    while (headerLine != "\r\n")
    {
        const auto headerDelimIt = headerLine.find(delim);

        const auto header = headerLine.substr(0, headerDelimIt);
        const auto value = headerLine.substr(headerDelimIt + delim.size());

        req.headers_.insert({header, value});
        headerLine = stream.ReceiveLine();
    }
}

}  // namespace KWS::Http
