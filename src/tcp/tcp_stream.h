#ifndef TCP_STREAM
#define TCP_STREAM

#include <string>
#include <string_view>

namespace KWS {

  class TcpStream {
   public:

    TcpStream(int sockfd);

    std::string ReceiveLine();

    void Send(std::string_view data) const;

   private:

    int sockfd_;
    std::string msg_;
    std::string::size_type last_pos_ = 0;
  };

}

#endif // TCP_STREAM
