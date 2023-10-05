#include "http_server.h"

namespace KWS {

  bool operator==(const HttpRoute& lhs, const HttpRoute& rhs) {
    return lhs.Method() == rhs.Method() && lhs.URI() == rhs.URI();
  }

}
