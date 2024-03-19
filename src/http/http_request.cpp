#include "../tcp/tcp_stream.h"
#include <kws/http_method.h>
#include <kws/http_request.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

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
    const auto header_it = headers_.find(header);

    if (header_it == std::end(headers_))
    {
        throw std::invalid_argument(header + " does not exist.");
    }

    return header_it->second;
}

void HttpRequest::AddRouteValue(const std::string& param,
                                const std::string& value)
{
    params_.insert({param, value});
}

void HttpRequest::ParseRequestLine(HttpRequest& req, TCP::TcpStream& stream)
{
    auto request_line = stream.ReceiveLine();
    std::istringstream iss{request_line};
    std::string method;

    iss >> method >> req.path_ >> req.version_;

    req.method_ = ToHttpMethod(method);
}

void HttpRequest::ParseHeaders(HttpRequest& req, TCP::TcpStream& stream)
{
    const std::string DELIM = ": ";

    auto header_line = stream.ReceiveLine();

    while (header_line != "\r\n")
    {
        const auto header_delim_it = header_line.find(DELIM);

        const auto header = header_line.substr(0, header_delim_it);
        const auto value = header_line.substr(header_delim_it + DELIM.size());

        req.headers_.insert({header, value});
        header_line = stream.ReceiveLine();
    }
}

}  // namespace KWS::Http
