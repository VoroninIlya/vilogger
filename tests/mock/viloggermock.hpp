#pragma once

#include "iviloglevel.hpp"
#include <gmock/gmock.h>

using namespace vilogger;

class ViLoggerMock : public IViLogger {  
public:
  MOCK_METHOD(void, setAllowedLogLevel, (LogLevel), (override));
  MOCK_METHOD(LogLevel, getAllowedLogLevel, (), (const, override));
  MOCK_METHOD(std::map<std::string, std::shared_ptr<IViLogger>>, getSubLoggers, (), (const, override));
  MOCK_METHOD(IViLogger&, getSubLogger, (const std::string&), (override));
  MOCK_METHOD(void, addSubLogger, (const std::string&), (override));
  MOCK_METHOD(void, removeSubLogger, (const std::string&), (override));
  MOCK_METHOD(IViLogger&, level, (LogLevel), (override));
  MOCK_METHOD(IViLogger&, message, (const std::string&), (override));
};
