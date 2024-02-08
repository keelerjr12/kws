#include "http_response.h"
#include <kws/http_status_code.h>
#include <string>

namespace KWS {

HttpResponse::HttpResponse(HttpStatusCode statusCode, std::string body)
  : statusCode_(statusCode)
  , body_(std::move(body))
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

}  // namespace KWS
