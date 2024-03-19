#include <kws/http_response.h>
#include <kws/http_status_code.h>
#include <string>

namespace KWS::Http {

HttpResponse::HttpResponse(HttpStatusCode statusCode, std::string body) :
  body_(std::move(body)), statusCode_(statusCode)
{}

HttpStatusCode HttpResponse::Status() const
{
    return statusCode_;
}

std::string HttpResponse::Serialize() const
{
    std::string resp;
    resp += "HTTP/1.1 200 OK\r\n";
    resp += "Content-Type: text/html\r\n";
    resp += "Content-Length: " + std::to_string(body_.size()) + "\r\n\r\n";
    resp += body_;

    return resp;
}

}  // namespace KWS::Http
