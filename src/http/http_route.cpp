#include <kws/http_route.h>

namespace KWS {

bool operator==(const HttpRoute& lhs, const HttpRoute& rhs)
{
    return lhs.Method() == rhs.Method() && lhs.Path() == rhs.Path();
}

}  // namespace KWS
