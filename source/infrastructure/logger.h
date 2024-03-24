#ifndef LOGGER_H
#define LOGGER_H

#include <chrono>
#include <format>
#include <iostream>
#include <source_location>
#include <string_view>

namespace KWS::Infrastructure
{

#ifdef NDEBUG
constexpr bool debugEnabled = false;
#else
constexpr bool debugEnabled = true;
#endif

enum class LogLevel
{
  Debug,
  Info,
  Error
};

std::string ToString(LogLevel lvl);

template<typename... Args>
struct Log
{
  Log(LogLevel lvl,
      std::string_view msg,
      Args&&... args,
      std::source_location loc = std::source_location::current())
  {
    if (!debugEnabled && lvl == LogLevel::Debug) {
      return;
    }

    const auto currTm = std::chrono::utc_clock::now();

    const auto dateTime = std::format(
        "{:%F %T}", std::chrono::floor<std::chrono::seconds>(currTm));

    const auto meta = std::format("[{0}] {1}: ({2}:{3})",
                                  dateTime,
                                  ToString(lvl),
                                  loc.file_name(),
                                  loc.line());

    const auto formMsg =
        std::vformat(msg, std::make_format_args(std::forward<Args>(args)...));

    std::cout << std::format("{0} {1}\n", meta, formMsg);
  }
};

template<typename... Args>
Log(LogLevel, std::string_view, Args&&...) -> Log<Args...>;

}  // namespace KWS::Infrastructure

#endif  // LOGGER_H
