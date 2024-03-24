#ifndef KWS_HTTP_STATUS_CODE_H
#define KWS_HTTP_STATUS_CODE_H

namespace KWS::Http
{

enum class HttpStatusCode
{
  OK = 200,
  BadRequest = 400,
  NotFound = 404
};

}  // namespace KWS::Http

#endif  // KWS_HTTP_STATUS_CODE_H
