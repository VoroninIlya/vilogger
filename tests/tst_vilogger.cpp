#include "vilogger.hpp"
#include "viloglevelmock.hpp"
#include <memory>
#include <gmock/gmock.h>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace vilogger;

class ViLoggerTest : public ::testing::Test 
{
protected:
  std::shared_ptr<ViLogLevelMock> m_llvl;
  std::shared_ptr<ViLogger> logger;
  std::string m_name = "test1";
  std::string m_time = "01:02:03";
  std::string m_res;
  size_t maxLogNameLength;
  
public:

  void SetUp() override {

    m_llvl = std::make_shared<ViLogLevelMock>();

    logger = std::make_shared<ViLogger>(
      m_name, m_llvl, std::bind(&ViLoggerTest::print, this, std::placeholders::_1),
      std::bind(&ViLoggerTest::time, this)
    );

  }

  void TearDown() override {
    m_llvl.reset();
    logger.reset();
    m_res = "";
  }

  void print(const std::string& s) {
    m_res = s;
  }

  std::string time() {
    return m_time;
  }

  std::string generateAwaitedResult(
    const std::string& time,
    LogLevel lvl,
    const std::string& name,
    const std::string& msg,
    ViLogFormat form
  ) {
    std::stringstream ss;
    std::string awaitedLvlStr = IViLogLevel::levelStrings[static_cast<size_t>(lvl)];
    if(ViLogFormat::PlaintText == form) {

      maxLogNameLength = name.length();
      ss << time;
      ss << "|" << std::left << std::setw(IViLogger::kLevelStrLen) << 
        awaitedLvlStr.substr(0, std::min(static_cast<size_t>(IViLogger::kLevelStrLen), awaitedLvlStr.length())) << "|";
      ss << std::left << std::setw(maxLogNameLength) << 
        name.substr(0, std::min(static_cast<size_t>(maxLogNameLength), name.length())) << "|" << msg << std::endl;

    } else if (ViLogFormat::Json == form) {

      ss << "{";
      ss << "\"time\":\"" << time << "\",";
      ss << "\"level\":\"" << awaitedLvlStr << "\",";
      ss << "\"name\":\"" << name << "\",";
      ss << "\"message\":\"" << msg << "\"";
      ss << "}\n";

    }

    return ss.str();
  }

};

TEST_F(ViLoggerTest, CopyAssignment) {
  ViLogger log {m_name, m_llvl, nullptr, nullptr};
  IViLogger* logPtr = (IViLogger*)&log;
  logPtr = (IViLogger*)&(*logger);

  EXPECT_CALL(*m_llvl, get())
    .Times(1)
    .WillOnce(testing::Return(LogLevel::Info));

  ASSERT_EQ(LogLevel::Info, logPtr->getAllowedLogLevel());
}

TEST_F(ViLoggerTest, TestIfAllowedAndCurrentLvlMatching) {
  LogLevel currentLvl = LogLevel::Info;
  LogLevel allowedLvl = LogLevel::Info;

  ViLogger log {std::move(*logger)};
  logger.reset();

  EXPECT_CALL(*m_llvl, get())
    .Times(1)
    .WillOnce(testing::Return(allowedLvl));

  ASSERT_EQ(ViLogFormat::PlaintText, log.getLogFormat());

  std::string msg = "Test message";
  log.level(currentLvl).message(msg);

  ASSERT_EQ(generateAwaitedResult(m_time, currentLvl, m_name, msg, 
    ViLogFormat::PlaintText), m_res);
}

TEST_F(ViLoggerTest, TestIfCurrentLvlGreaterAsAllowed) {
  LogLevel currentLvl = LogLevel::Debug;
  LogLevel allowedLvl = LogLevel::Info;

  ViLogger log {m_name, m_llvl, nullptr, nullptr};
  log = std::move(*logger);
  logger.reset();
  
  EXPECT_CALL(*m_llvl, get())
    .Times(1)
    .WillOnce(testing::Return(allowedLvl));

  ASSERT_EQ(ViLogFormat::PlaintText, log.getLogFormat());

  std::string msg = "Test message";
  log.level(currentLvl).message(msg);

  ASSERT_EQ("", m_res);
}

TEST_F(ViLoggerTest, TestGetAllowedLogLevel) {
  EXPECT_CALL(*m_llvl, get())
    .Times(1)
    .WillOnce(testing::Return(LogLevel::Trace));

  ASSERT_EQ(LogLevel::Trace, logger->getAllowedLogLevel());
}

TEST_F(ViLoggerTest, TestSetAllowedLogLevel) {

  EXPECT_CALL(*m_llvl, set(LogLevel::Trace))
    .Times(1);

  logger->setAllowedLogLevel(LogLevel::Trace);
}

TEST_F(ViLoggerTest, TestSubLoggers) {
  std::string sub1 = "sub1";
  std::string subSub1 = "subSub1";
  LogLevel currentLvl = LogLevel::Info;
  LogLevel allowedLvl = LogLevel::Debug;

  logger->addSubLogger(sub1);
  logger->getSubLogger(sub1).addSubLogger(subSub1);

  EXPECT_CALL(*m_llvl, set(allowedLvl))
    .Times(3);

  logger->setAllowedLogLevel(allowedLvl);

  EXPECT_CALL(*m_llvl, get())
    .Times(1)
    .WillOnce(testing::Return(allowedLvl));

  std::string msg = "Test message";
  logger->getSubLogger(sub1).getSubLogger(subSub1)
    .level(currentLvl).message(msg);

  std::string awaitedName = m_name + "." + sub1 + "." + subSub1;

  ASSERT_EQ(generateAwaitedResult(m_time, currentLvl, awaitedName, 
    msg, ViLogFormat::PlaintText), m_res);

  m_res = "";
  currentLvl = LogLevel::Info;

  EXPECT_CALL(*m_llvl, get())
    .Times(1)
    .WillOnce(testing::Return(allowedLvl));

  logger->setLogFormat(ViLogFormat::Json); 
  msg = "Test message";
  logger->getSubLogger(sub1)
    .level(currentLvl).message(msg);

  awaitedName = m_name + "." + sub1;

  ASSERT_EQ(generateAwaitedResult(m_time, currentLvl, awaitedName, msg, 
    ViLogFormat::Json), m_res);

  m_res = "";
  currentLvl = LogLevel::Trace;

  EXPECT_CALL(*m_llvl, get())
    .Times(1)
    .WillOnce(testing::Return(allowedLvl));

  logger->getSubLogger(sub1).getSubLogger(subSub1)
    .level(currentLvl).message(msg);

  ASSERT_EQ("", m_res);

}
