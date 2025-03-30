#pragma once
#include "iviloglevel.hpp"
#include <memory>
#include <string>
#include <sstream>
#include <cstdint>
#include <map>

namespace vilogger {

  enum class ViLogFormat {
    PlaintText = 0,
    Json
  };

  struct IViLogger {

    inline static const uint8_t kLevelStrLen = 5;

    virtual void setAllowedLogLevel(LogLevel) = 0;
    virtual LogLevel getAllowedLogLevel(void) const = 0;

    virtual void setLogFormat(ViLogFormat) = 0;
    virtual ViLogFormat getLogFormat(void) const = 0;

    virtual std::map<std::string, std::shared_ptr<IViLogger>> getSubLoggers() const = 0;
    virtual IViLogger& getSubLogger(const std::string&) = 0;

    virtual void addSubLogger(const std::string&) = 0;
    virtual void removeSubLogger(const std::string&) = 0;

    virtual IViLogger& level(LogLevel) = 0;
    virtual IViLogger& message(const std::string&) = 0;

    virtual ~IViLogger() = default;
  
  };

}

