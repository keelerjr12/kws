#include "tcp_server.h"
#include "tcp_socket.h"
#include "tcp_stream.h"
#include <string>
#include <utility>

namespace KWS::TCP {

const auto listenBacklog = 50;

TcpServer::TcpServer(std::string host, int port) :
  host_(std::move(host)), port_(port), servSock_(CreateSocket())
{}

void TcpServer::Serve()
{
    servSock_.Bind(host_, port_);
    servSock_.Listen(listenBacklog);

    while (true)
    {
        auto clientSock = servSock_.Accept();

        TcpStream tcpStrm{std::move(clientSock)};
        HandleClient(tcpStrm);
    }
}

}  // namespace KWS::TCP
