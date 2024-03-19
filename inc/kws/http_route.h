#ifndef HTTP_ROUTE_H
#define HTTP_ROUTE_H

#include <kws/http_method.h>
#include <string>

namespace KWS::Http {

class HttpRoute
{
  public:
    HttpRoute(HttpMethod method, const std::string& path) :
      method_(method), path_(path)
    {}

    HttpMethod Method() const
    {
        return method_;
    }

    std::string Path() const
    {
        return path_;
    }

  private:
    HttpMethod method_;
    std::string path_;
};

}  // namespace KWS::Http

#endif  // HTTP_ROUTE_H
