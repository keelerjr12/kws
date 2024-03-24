#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <kws/http_status_code.h>
#include <string>

namespace KWS::Http {

class HttpResponse
{
  public:
    HttpResponse(HttpStatusCode statusCode, std::string body);

    HttpStatusCode Status() const;

    std::string Serialize() const;

  private:
    std::string body_;
    HttpStatusCode statusCode_;
};

}  // namespace KWS::Http

#endif  // HTTP_RESPONSE_H
