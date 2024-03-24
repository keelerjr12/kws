#include <string>

#include "logger.h"

namespace KWS::Infrastructure
{
std::string ToString(LogLevel lvl)
{
  switch (lvl) {
    case LogLevel::Debug:
      return "DEBUG";
      break;
    case LogLevel::Info:
      return "INFO";
      break;
    case LogLevel::Error:
      return "ERROR";
      break;
  };

  return "UNDEFINED";
}
}  // namespace KWS::Infrastructure
