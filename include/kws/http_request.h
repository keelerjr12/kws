#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <sstream>
#include <string>
#include <unordered_map>

#include <kws/http_method.h>

namespace KWS::TCP
{
class TcpStream;
}  // namespace KWS::TCP

namespace KWS::Http
{

class HttpRequest
{
public:
  HttpRequest() = default;
  HttpRequest(HttpMethod method, std::string path);

  static HttpRequest ParseFrom(TCP::TcpStream& stream);

  HttpMethod Method() const;
  std::string Path() const;
  std::string Version() const;
  std::string Header(const std::string& header) const;

  template<class T>
  T RouteValue(const std::string& param) const;

  void AddRouteValue(const std::string& param, const std::string& value);

private:
  static void ParseRequestLine(HttpRequest& req, TCP::TcpStream& stream);
  static void ParseHeaders(HttpRequest& req, TCP::TcpStream& stream);

  HttpMethod method_ = HttpMethod::GET;
  std::string path_;
  std::string version_;

  std::unordered_map<std::string, std::string> headers_;
  std::unordered_map<std::string, std::string> params_;
};

template<class T>
T HttpRequest::RouteValue(const std::string& param) const
{
  const auto val = params_.find(param)->second;

  std::istringstream strStm {val}; //NOLINT
  T ret;
  strStm >> ret;

  return ret;
}

}  // namespace KWS::Http

#endif  // HTTP_REQUEST_H
