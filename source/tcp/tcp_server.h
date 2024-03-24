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

    TcpServer(const TcpServer&) = delete;
    TcpServer(TcpServer&&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;
    TcpServer& operator=(TcpServer&&) = delete;

    void Serve();

  protected:
    virtual void HandleClient(TcpStream& strm) = 0;

  private:
    std::string host_;
    int port_;

    TcpSocket servSock_;
};

}  // namespace KWS::TCP

#endif  // TCP_SERVER_H
