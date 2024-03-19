#ifndef KWS_HTTP_STATUS_CODE_H
#define KWS_HTTP_STATUS_CODE_H

namespace KWS::Http {

enum class HttpStatusCode
{
    OK = 200,
    BAD_REQUEST = 400,
    NOT_FOUND = 404
};

}

#endif  // KWS_HTTP_STATUS_CODE_H
