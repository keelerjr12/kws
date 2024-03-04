#ifndef TCP_SERVER_H
#define TCP_SERVER_H

namespace KWS {

class TcpStream;

class TcpServer
{
  public:
    TcpServer(const char* host, int port);

    virtual ~TcpServer();

    void Serve();

  protected:
    virtual void HandleClient(TcpStream& strm) = 0;

  private:
    void Bind();
    void Listen(int backlog) const;
    int Accept() const;

    const char* host_;
    int port_;
    int sockfd_;
};

}  // namespace KWS

#endif  // TCP_SERVER_H
