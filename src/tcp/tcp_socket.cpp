#include "tcp_socket.h"
#include <arpa/inet.h>
#include <stdexcept>
#include <unistd.h>

namespace KWS::TCP {

TcpSocket::TcpSocket(int sockfd) : sockfd_(sockfd) {}

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

    const auto err_code =
        bind(sockfd_, reinterpret_cast<sockaddr*>(&sai), sizeof(sai));
    if (err_code == -1)
    {
        throw std::runtime_error("[ERROR]: Unable to bind to socket");
    }
}

void TcpSocket::Listen(int backlog) const
{
    const auto err_code = listen(sockfd_, backlog);

    if (err_code == -1)
    {
        throw std::runtime_error("[ERROR]: Unable to listen on socket");
    }
}

TcpSocket TcpSocket::Accept() const
{
    struct sockaddr peer_addr = {};
    socklen_t peer_addr_sz = sizeof(peer_addr);

    auto clientfd = accept(sockfd_, &peer_addr, &peer_addr_sz);
    if (clientfd == -1)
    {
        throw std::runtime_error("[ERROR]: Unable to accept connection");
    }

    return TcpSocket{clientfd};
}

void TcpSocket::Send(std::string_view data) const
{
    send(sockfd_, data.data(), data.size(), 0);
}

TcpSocket CreateSocket()
{
    const auto sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        throw std::runtime_error("[ERROR]: Unable to open socket");
    }

    return TcpSocket{sockfd};
}

}  // namespace KWS::TCP
