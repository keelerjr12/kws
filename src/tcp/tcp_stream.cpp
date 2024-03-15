#include "tcp_stream.h"
#include "../infrastructure/logger.h"
#include <arpa/inet.h>
#include <array>
#include <string>
#include <string_view>
#include <unistd.h>

namespace KWS::TCP {

TcpStream::TcpStream(const TcpSocket& socket)
  : socket_(socket)
{}

std::string TcpStream::ReceiveLine()
{
    if (auto line = FindExistingLine())
    {
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

    const auto end_line = msg_.find(MESSAGE_DELIM, last_pos_);

    if (end_line != std::string::npos)
    {
        line =
            msg_.substr(last_pos_, end_line + MESSAGE_DELIM.size() - last_pos_);
        last_pos_ = end_line + MESSAGE_DELIM.size();

        LOG_DEBUG("Existing line found: {0}", line);
        return line;
    }

    return {};
}

std::string TcpStream::ReadDataUntilLineFound()
{
    LOG_DEBUG("TCP stream being read...");

    // TODO: make this a member variable
    std::array<char, BUF_SIZE> buf = {0};

    while (true)
    {
        const auto bytes_recv = socket_.Read(buf);
        msg_.append(buf.data(), bytes_recv);

        if (auto line = FindExistingLine())
        {
            return *line;
        }

        std::fill(buf.begin(), buf.end(), 0);
    }

    return {};
}

}  // namespace KWS::TCP
