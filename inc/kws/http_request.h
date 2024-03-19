#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <kws/http_method.h>
#include <string>
#include <sstream>
#include <unordered_map>

namespace KWS::TCP {
class TcpStream;
}

namespace KWS::Http {

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

    template <class T>
    T RouteValue(const std::string& param) const;

    void AddRouteValue(const std::string& param, const std::string& value);

  private:
    static void ParseRequestLine(HttpRequest& req, TCP::TcpStream& stream);
    static void ParseHeaders(HttpRequest& req, TCP::TcpStream& stream);

    HttpMethod method_;
    std::string path_;
    std::string version_;

    std::unordered_map<std::string, std::string> headers_;
    std::unordered_map<std::string, std::string> params_;
};

template <class T>
T HttpRequest::RouteValue(const std::string& param) const {
    const auto val = params_.find(param)->second;

    std::stringstream ss{val};
    T ret;
    ss >> ret;

    return ret;
}

}  // namespace KWS::Http

#endif  // HTTP_REQUEST_H
