#ifndef HTTP_METHOD_H
#define HTTP_METHOD_H

#include <string>

namespace KWS::Http {

enum class HttpMethod
{
    GET,
    POST,
    PUT,
    DELETE
};

// TODO: move this to a utility class
std::string toUpper(std::string str);

HttpMethod ToHttpMethod(const std::string& str);
std::string ToString(HttpMethod method);

}  // namespace KWS::Http

#endif  // HTTP_METHOD_H
