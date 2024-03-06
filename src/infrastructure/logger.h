#ifndef LOGGER_H
#define LOGGER_H

#include <chrono>
#include <format>
#include <iostream>
#include <string_view>

template<typename... Args>
void logConsole(std::string_view msg,
                std::string_view lvl,
                std::string_view file,
                int line,
                Args&&... args)
{
    const auto tm = std::chrono::utc_clock::now();

    const auto dt =
        std::format("{:%F %T}", std::chrono::floor<std::chrono::seconds>(tm));
    const auto meta = std::format("[{0}] {1}: ({2}:{3})", dt, lvl, file, line);
    const auto formMsg =
        std::vformat(msg, std::make_format_args(std::forward<Args>(args)...));

    std::cout << std::format("{0} {1}\n", meta, formMsg);
}

#ifdef NDEBUG
    #define LOG_DEBUG(str, ...)
#else
    #define LOG_DEBUG(msg, ...)                                                \
        logConsole(msg, "DEBUG", __FILE__, __LINE__, __VA_ARGS__)
#endif  // NDEBUG

#define LOG_INFO(msg, ...)                                                     \
    logConsole(msg, "INFO", __FILE__, __LINE__, __VA_ARGS__)

#define LOG_ERROR(msg, ...)                                                    \
    logConsole(msg, "ERROR", __FILE__, __LINE__, __VA_ARGS__)

#endif  //LOGGER_H
