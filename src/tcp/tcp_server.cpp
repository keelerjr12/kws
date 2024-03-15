#include "tcp_server.h"
#include "tcp_socket.h"
#include "tcp_stream.h"
#include <string>

namespace KWS::TCP {

const auto LISTEN_BACKLOG = 50;

TcpServer::TcpServer(std::string host, int port)
    : host_(std::move(host)), port_(port), servSock(CreateSocket()) {}

void TcpServer::Serve() {
  servSock.Bind(host_, port_);
  servSock.Listen(LISTEN_BACKLOG);

  while (true) {
    const auto clientSock = servSock.Accept();

    TcpStream tcp_strm{clientSock};
    HandleClient(tcp_strm);
  }
}

} // namespace KWS::TCP
