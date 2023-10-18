#include "http_response.h"
#include <string>

namespace KWS {

  HttpResponse::HttpResponse(std::string content) : content_(std::move(content)) { }

  std::string HttpResponse::Serialize() const {
    std::string resp;
    resp += "HTTP/1.1 200 OK\r\n";
    resp += "Content-Type: text/html\r\n";
    resp += "Content-Length: " + std::to_string(content_.size()) + "\r\n\r\n";
    resp += content_;

    return resp;
  }

}
