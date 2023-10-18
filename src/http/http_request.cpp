#include "http_request.h"
#include "http_method.h"
#include "../tcp/tcp_stream.h"
#include <stdexcept>
#include <sstream>

namespace KWS {

  HttpRequest::HttpRequest(HttpMethod method, std::string uri) : 
      method_(method), uri_(std::move(uri)) { }

  HttpRequest HttpRequest::ParseFrom(TcpStream& stream) {
    HttpRequest req;

    ParseRequestLine(req, stream);
    ParseHeaders(req, stream);

    return req;
  }

  HttpMethod HttpRequest::Method() const {
    return method_;
  }

  std::string HttpRequest::URI() const {
    return uri_;
  }

  std::string HttpRequest::Version() const {
    return version_;
  }

  std::string HttpRequest::Header(const std::string& header) const {
    const auto header_it = headers_.find(header);

    if (header_it == std::end(headers_)) {
      throw std::invalid_argument(header + " does not exist.");
    }

    return header_it->second;
  }

  void HttpRequest::ParseRequestLine(HttpRequest& req, TcpStream& stream) {
    auto request_line = stream.ReceiveLine();
    std::istringstream iss {request_line};
    std::string method;

    iss >> method >> req.uri_ >> req.version_;

    req.method_ = ToHttpMethod(method);
  }

  void HttpRequest::ParseHeaders(HttpRequest& req, TcpStream& stream) {
    const std::string DELIM = ": ";

    auto header_line = stream.ReceiveLine();

    while (header_line != "\r\n") {
      const auto header_delim_it = header_line.find(DELIM);

      const auto header = header_line.substr(0, header_delim_it);
      const auto value = header_line.substr(header_delim_it + DELIM.size());

      req.headers_.insert({header, value});
      header_line = stream.ReceiveLine();
    }
  }

}
