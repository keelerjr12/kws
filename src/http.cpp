#include "http.h"
#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_map>

namespace KWS {

  static const std::unordered_map<std::string, HttpMethod> method_map = {
    {"GET", HttpMethod::GET},
    {"POST", HttpMethod::POST},
    {"PUT", HttpMethod::PUT},
    {"DELETE", HttpMethod::DELETE}
  };

  // TODO: move this to a utility class
  std::string toUpper(std::string str) {
    std::transform(std::begin(str), std::end(str), std::begin(str), [](unsigned char chr) {
        return std::toupper(chr);
    });

    return str;
  }

  HttpMethod ToHttpMethod(const std::string& str) {
    auto method_it = method_map.find(toUpper(str));
    if (method_it == std::end(method_map)) {
      throw std::invalid_argument(str + " is in invalid HTTP method");
    }

    return method_it->second;
  }

  std::string ToString(HttpMethod method) {
    for (const auto& pair : method_map) {
      if (pair.second == method) {
        return pair.first;
      }
    }

    throw std::invalid_argument("Method cannot be converted to a string");
  }

  HttpRequest::HttpRequest(HttpMethod method, const std::string& uri) : 
      method_(method), uri_(uri) { }

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

  HttpResponse::HttpResponse(const std::string& content) : content_(content) { }

  std::string HttpResponse::Serialize() const {
    std::string resp;
    resp += "HTTP/1.1 200 OK\r\n";
    resp += "Content-Type: text/html\r\n";
    resp += "Content-Length: " + std::to_string(content_.size()) + "\r\n\r\n";
    resp += content_;

    return resp;
  }

}
