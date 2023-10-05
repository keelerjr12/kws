#include "tcp_server.h"
#include "tcp_stream.h"
#include <stdexcept>
#include <arpa/inet.h>
#include <unistd.h>

namespace KWS {

  const auto LISTEN_BACKLOG = 50;

  static int CreateSocket();

  TcpServer::TcpServer(const char* host, int port) : host_(host), port_(port), sockfd_(CreateSocket()) { }

  TcpServer::~TcpServer() {
    close(sockfd_);
  }

  void TcpServer::Serve() {
    Bind();
    Listen(LISTEN_BACKLOG);

    while (true) {
      const int clientfd = Accept();

      TcpStream tcp_strm {clientfd};
      HandleClient(tcp_strm);
    }
  }

  void TcpServer::Bind() {
    sockaddr_in sai = {};
    sai.sin_family = AF_INET;
    inet_pton(AF_INET, host_, &sai.sin_addr.s_addr);
    sai.sin_port = htons(port_);

    int reuse = 1;
    setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    const auto err_code = bind(sockfd_, reinterpret_cast<sockaddr*>(&sai), sizeof(sai));
    if (err_code == -1) {
      throw std::runtime_error("[ERROR]: Unable to bind to socket");
    }
  }

  void TcpServer::Listen(int backlog) const {
    const auto err_code = listen(sockfd_, backlog);

    if (err_code == -1) {
      throw std::runtime_error("[ERROR]: Unable to listen on socket");
    }
  }

  int TcpServer::Accept() const {
    struct sockaddr peer_addr = {};
    socklen_t peer_addr_sz = sizeof(peer_addr);

    auto clientfd = accept(sockfd_, &peer_addr, &peer_addr_sz);
    if (clientfd == -1) {
      throw std::runtime_error("[ERROR]: Unable to accept connection");
    }

    return clientfd;
  }

  static int CreateSocket() {
    const auto sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) { 
      throw std::runtime_error("[ERROR]: Unable to open socket");
    }

    return sockfd;
  }

}
