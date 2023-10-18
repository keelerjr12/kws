#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <string>

namespace KWS {

  class HttpResponse {
   public:
     HttpResponse(std::string content);

     std::string Serialize() const;

   private:
     std::string content_;
  };

}

#endif // HTTP_RESPONSE_H
