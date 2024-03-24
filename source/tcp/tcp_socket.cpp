#include <bit>
#include <stdexcept>
#include <string>
#include <string_view>

#include "tcp_socket.h"

#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace KWS::TCP
{

TcpSocket::TcpSocket(int sockfd)
    : sockfd_(sockfd)
{
}

TcpSocket::~TcpSocket()
{
  close(sockfd_);
}

void TcpSocket::Bind(const std::string& host, int port)
{
  host_ = host;
  port_ = port;

  sockaddr_in sai = {};
  sai.sin_family = AF_INET;
  inet_pton(AF_INET, host_.c_str(), &sai.sin_addr.s_addr);
  sai.sin_port = htons(port_);

  int reuse = 1;
  setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

  const auto errCode =
      bind(sockfd_, std::bit_cast<sockaddr*>(&sai), sizeof(sai));
  if (errCode == -1) {
    throw std::runtime_error("[ERROR]: Unable to bind to socket");
  }
}

void TcpSocket::Listen(int backlog) const
{
  const auto errCode = listen(sockfd_, backlog);

  if (errCode == -1) {
    throw std::runtime_error("[ERROR]: Unable to listen on socket");
  }
}

TcpSocket TcpSocket::Accept() const
{
  struct sockaddr peerAddr = {};
  socklen_t peerAddrSz = sizeof(peerAddr);

  auto clientfd = accept4(sockfd_, &peerAddr, &peerAddrSz, SOCK_CLOEXEC);
  if (clientfd == -1) {
    throw std::runtime_error("[ERROR]: Unable to accept connection");
  }

  return TcpSocket {clientfd};
}

void TcpSocket::Send(std::string_view data) const
{
  send(sockfd_, data.data(), data.size(), 0);
}

TcpSocket CreateSocket()
{
  const auto sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    throw std::runtime_error("[ERROR]: Unable to open socket");
  }

  return TcpSocket {sockfd};
}

}  // namespace KWS::TCP
