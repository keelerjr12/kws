#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include <arpa/inet.h>
#include <stdexcept>
#include <string>

namespace KWS::TCP {

/* TODO: Need to split this class into two separate socket types */
class TcpSocket
{
  public:
    explicit TcpSocket(int sockfd);

    ~TcpSocket();

    void Bind(const std::string host, int port);
    void Listen(int backlog) const;

    TcpSocket Accept() const;

    void Send(std::string_view data) const;

    template<std::size_t BUF_SZ>
    std::size_t Read(std::array<char, BUF_SZ>& buf);

  private:
    std::string host_;
    int port_;

    int sockfd_;
};

template<std::size_t BUF_SZ>
std::size_t TcpSocket::Read(std::array<char, BUF_SZ>& buf)
{
    const auto bytes_recv = recv(sockfd_, buf.data(), BUF_SZ, 0);

    if (bytes_recv < 0)
    {
        throw std::runtime_error("[ERROR]: Socket closed");
    }

    return static_cast<std::size_t>(bytes_recv);
}

TcpSocket CreateSocket();

}  // namespace KWS::TCP

#endif  // TCP_SOCKET_H
