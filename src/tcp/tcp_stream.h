#ifndef TCP_STREAM
#define TCP_STREAM

#include <optional>
#include <string>
#include <string_view>
#include "tcp_socket.h"

namespace KWS::TCP {


class TcpStream
{
  public:
    TcpStream(const TcpSocket& socket);

    std::string ReceiveLine();

    void Send(std::string_view data) const;

  private:
    std::optional<std::string> FindExistingLine();
    std::string ReadDataUntilLineFound();

    TcpSocket socket_;
    std::string msg_;
    std::string::size_type last_pos_ = 0;

    constexpr static auto MESSAGE_DELIM = std::string("\r\n");
    constexpr static auto BUF_SIZE = 512;
};

}  // namespace KWS

#endif  // TCP_STREAM
