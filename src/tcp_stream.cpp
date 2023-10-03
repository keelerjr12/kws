#include "tcp_stream.h"
#include <array>
#include <stdexcept>
#include <string>
#include <string_view>
#include <arpa/inet.h>
#include <unistd.h>

namespace Keeler {

  TcpStream::TcpStream(int sockfd) : sockfd_(sockfd) { }

  std::string TcpStream::ReceiveLine() {
    const auto MESSAGE_DELIM = std::string("\r\n");
    std::string line;

    const auto end_line = msg_.find(MESSAGE_DELIM, last_pos_);

    if (end_line != std::string::npos) {
      line = msg_.substr(last_pos_, end_line + MESSAGE_DELIM.size() - last_pos_);
      last_pos_ = end_line + MESSAGE_DELIM.size();
      return line;
    }

    constexpr auto BUF_SIZE = 512;
    std::array<char, BUF_SIZE> buf = {0};

    while (true) {
      const auto bytes_recv = recv(sockfd_, buf.data(), BUF_SIZE, 0);

      if (bytes_recv < 0) {
        throw std::runtime_error("[ERROR]: Socket closed");
      }

      msg_.append(buf.data(), bytes_recv);

      const auto end_line = msg_.find(MESSAGE_DELIM, last_pos_);

      if (end_line != std::string::npos) {
        line = msg_.substr(last_pos_, end_line + MESSAGE_DELIM.size());
        last_pos_ = end_line + MESSAGE_DELIM.size();
        break;
      }

      std::fill(buf.begin(), buf.end(), 0);
    }

    return line;
  }

  void TcpStream::Send(std::string_view data) const {
    send(sockfd_, data.data(), data.size(), 0);
  }

}
