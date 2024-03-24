#ifndef TCP_STREAM
#define TCP_STREAM

#include "tcp_socket.h"
#include <optional>
#include <string>
#include <string_view>

namespace KWS::TCP {

class TcpStream
{
  public:
    explicit TcpStream(TcpSocket&& socket);

    std::string ReceiveLine();

    void Send(std::string_view data) const;

  private:
    std::optional<std::string> FindExistingLine();
    std::string ReadDataUntilLineFound();

    TcpSocket socket_;
    std::string msg_;
    std::string::size_type lastPos_ = 0;

    constexpr static auto messageDelim = std::string("\r\n");
    constexpr static auto bufSz = 512;
};

}  // namespace KWS::TCP

#endif  // TCP_STREAM
