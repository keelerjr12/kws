#include "http_server.h"
#include "tcp_stream.h"
#include <array>
#include <functional>
#include <sstream>
#include <string>
#include <stdexcept>
#include <arpa/inet.h>
#include <unistd.h>
#include <unordered_map>

namespace Keeler {

  const auto LISTEN_BACKLOG = 50;

  static int CreateSocket();

  std::string toUpper(std::string str) {
    std::transform(std::begin(str), std::end(str), std::begin(str), [](unsigned char c) {
        return std::toupper(c);
    });

    return str;
  }

  static std::unordered_map<std::string, HttpMethod> method_map = {
    {"GET", HttpMethod::GET},
    {"POST", HttpMethod::POST},
    {"PUT", HttpMethod::PUT},
    {"DELETE", HttpMethod::DELETE}
  };

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

  HttpServer::HttpServer(const char* host, int port) : host_(host), port_(port), sockfd_(CreateSocket()) { }

  HttpServer::~HttpServer()  {
    close(sockfd_);
  }

  void HttpServer::RegisterHandler(HttpMethod method, const std::string& uri, const std::function<HttpResponse(const HttpRequest&)> & handler) {
    auto uri_it = handlers_.find(uri);

    if (uri_it != std::end(handlers_)) {
      uri_it->second.insert({method, handler});
    } else {
      std::unordered_map<HttpMethod, std::function<HttpResponse(const HttpRequest&)>> new_map;
      new_map.insert({method, handler});
      handlers_.insert({uri, new_map});
    }
  }

  void HttpServer::Serve() {
    Bind();
    Listen( LISTEN_BACKLOG);

    const int clientfd = Accept();

    TcpStream tcp_strm {clientfd};
    auto http_req = HttpRequest::ParseFrom(tcp_strm);

    const auto uri_map = handlers_.find(http_req.URI());
    const auto handler = uri_map->second.find(http_req.Method());
    const auto http_resp = handler->second(http_req);

    tcp_strm.Send(http_resp.Serialize());
  }

  void HttpServer::Bind() {
    sockaddr_in sai = { 0 };
    sai.sin_family = AF_INET;
    inet_pton(AF_INET, host_, &sai.sin_addr.s_addr);
    sai.sin_port = htons(port_);

    int reuse = 1;
    setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    auto err_code = bind(sockfd_, reinterpret_cast<sockaddr*>(&sai), sizeof(sai));
    if (err_code == -1) {
      throw std::runtime_error("[ERROR]: Unable to bind to socket");
    }
  }

  void HttpServer::Listen(int backlog) const {
    auto err_code = listen(sockfd_, backlog);

    if (err_code == -1) {
      throw std::runtime_error("[ERROR]: Unable to listen on socket");
    }
  }

  int HttpServer::Accept() const {
    struct sockaddr peer_addr = {0};
    socklen_t peer_addr_sz = sizeof(peer_addr);

    auto clientfd = accept(sockfd_, &peer_addr, &peer_addr_sz);
    if (clientfd == -1) {
      throw std::runtime_error("[ERROR]: Unable to accept connection");
    }

    return clientfd;
  }

  static int CreateSocket() {
    auto sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) { 
      throw std::runtime_error("[ERROR]: Unable to open socket");
    }

    return sockfd;
  }

}
