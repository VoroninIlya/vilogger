#pragma once

#include <ostream>

namespace vilogger {
  
  enum class LogLevel  {
    Disabled = 0,
    Info,
    Debug,
    Trace,
    Error
  };

  struct IViLogLevel {

    inline static const std::string levelStrings[] {"", "INFO", "DEBUG", "TRACE", "ERROR"};

    virtual void set(LogLevel) = 0;
    virtual LogLevel get() const = 0;

    virtual ~IViLogLevel() = default;
  };
  
}
