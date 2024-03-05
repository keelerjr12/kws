#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <string>

namespace KWS {

class TcpStream;

class TcpServer
{
  public:
    TcpServer(const std::string host, int port);

    virtual ~TcpServer();

    void Serve();

  protected:
    virtual void HandleClient(TcpStream& strm) = 0;

  private:
    void Bind();
    void Listen(int backlog) const;
    int Accept() const;

    std::string host_;
    int port_;
    int sockfd_;
};

}  // namespace KWS

#endif  // TCP_SERVER_H
