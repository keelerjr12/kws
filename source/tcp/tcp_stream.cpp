#include <algorithm>
#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "tcp_stream.h"

#include "../infrastructure/logger.h"
#include "tcp_socket.h"

namespace KWS::TCP
{

TcpStream::TcpStream(TcpSocket&& socket)
    : socket_(std::move(socket))
{
}

std::string TcpStream::ReceiveLine()
{
  if (auto line = FindExistingLine()) {
    return *line;
  }

  const auto line = ReadDataUntilLineFound();
  return line;
}

void TcpStream::Send(std::string_view data) const
{
  socket_.Send(data);
}

std::optional<std::string> TcpStream::FindExistingLine()
{
  std::string line;

  const auto endLine = msg_.find(messageDelim, lastPos_);

  if (endLine != std::string::npos) {
    line = msg_.substr(lastPos_, endLine + messageDelim.size() - lastPos_);
    lastPos_ = endLine + messageDelim.size();

    KWS::Infrastructure::Log(
        KWS::Infrastructure::LogLevel::Debug, "Existing line found: {0}", line);
    return line;
  }

  return {};
}

std::string TcpStream::ReadDataUntilLineFound()
{
  KWS::Infrastructure::Log(KWS::Infrastructure::LogLevel::Debug,
                           "TCP stream being read...");

  // TODO: make this a member variable
  std::array<char, bufSz> buf = {0};

  while (true) {
    const auto bytesRecv = socket_.Read(buf);
    msg_.append(buf.data(), bytesRecv);

    if (auto line = FindExistingLine()) {
      return *line;
    }

    std::fill(buf.begin(), buf.end(), 0);
  }

  return {};
}

}  // namespace KWS::TCP
