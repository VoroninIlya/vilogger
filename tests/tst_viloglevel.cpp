#include "viloglevel.hpp"
#include <memory>
#include <gmock/gmock.h>

using namespace vilogger;

class ViLogLevelTest : public ::testing::Test {};

TEST_F(ViLogLevelTest, FullTest) {
  ViLogLevel logLvl{};

  std::stringstream ss;

  ASSERT_EQ(LogLevel::Disabled, logLvl.get());

  logLvl.set(LogLevel::Info);
  ASSERT_EQ(LogLevel::Info, logLvl.get());

  logLvl.set(LogLevel::Debug);
  ASSERT_EQ(LogLevel::Debug, logLvl.get());
  
  logLvl.set(LogLevel::Trace);
  ASSERT_EQ(LogLevel::Trace, logLvl.get());
  
}