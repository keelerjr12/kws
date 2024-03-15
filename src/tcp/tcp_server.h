#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "tcp_socket.h"
#include <string>

namespace KWS::TCP {

class TcpStream;

class TcpServer
{
  public:
    TcpServer(std::string host, int port);

    virtual ~TcpServer() = default;

    void Serve();

  protected:
    virtual void HandleClient(TcpStream& strm) = 0;

  private:
    std::string host_;
    int port_;

    TcpSocket servSock;
};

}  // namespace KWS::TCP

#endif  // TCP_SERVER_H
