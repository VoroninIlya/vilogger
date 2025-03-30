#pragma once

#include "iviloglevel.hpp"
#include <gmock/gmock.h>

using namespace vilogger;

class ViLogLevelMock : public IViLogLevel {  
public:
  MOCK_METHOD(void, set, (LogLevel lvl), (override));
  MOCK_METHOD(LogLevel, get, (), (const, override));
};

