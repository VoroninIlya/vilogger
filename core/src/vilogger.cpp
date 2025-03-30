#include "vilogger.hpp"
#include "viloglevel.hpp"
#include <typeinfo>
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace vilogger {

  size_t ViLogger::maxLogNameLength = 0;

  ViLogger::ViLogger(const std::string& name,
    std::shared_ptr<IViLogLevel> lvl, 
    std::function<void(const std::string&)> printCb,
    std::function<std::string()> getTimeCb) : m_name(name),
    m_allowedLevel(lvl), m_getTime(getTimeCb), m_print(printCb) {
      maxLogNameLength = std::max(maxLogNameLength, m_name.length());
  }
  
  ViLogger::ViLogger(ViLogger&& obj) noexcept {
    *this = std::move(obj);
  }
  
  void ViLogger::setAllowedLogLevel(LogLevel lvl) {
    std::lock_guard lock(m_printLock);
    m_allowedLevel->set(lvl);
    for(auto& l : m_subLoggers) {
      l.second->setAllowedLogLevel(lvl);
    }
  }
  
  LogLevel ViLogger::getAllowedLogLevel(void) const {
    return m_allowedLevel->get();
  }

  void ViLogger::setLogFormat(ViLogFormat format) {
    std::lock_guard lock(m_printLock);
    m_format = format;
    for(auto& l : m_subLoggers) {
      l.second->setLogFormat(m_format);
    }
  }

  ViLogFormat ViLogger::getLogFormat(void) const {
    return m_format;
  }

  std::map<std::string, std::shared_ptr<IViLogger>> ViLogger::getSubLoggers() const {
    return m_subLoggers;
  }

  IViLogger& ViLogger::getSubLogger(const std::string& name) {
    return *m_subLoggers.at(name);
  }

  void ViLogger::addSubLogger(const std::string& name) {
    std::lock_guard lock(m_printLock);
    m_subLoggers[name] = std::make_shared<ViLogger>(name, m_allowedLevel, m_print, m_getTime);
    auto& parents = (dynamic_cast<ViLogger*>(m_subLoggers[name].get()))->m_parents;
    parents.assign(m_parents.begin(), m_parents.end());
    parents.push_back(m_name);
    std::string newLoggerName{};

    for(auto& n : m_parents) 
      newLoggerName += n + ".";

    newLoggerName += name;
    maxLogNameLength = std::max(maxLogNameLength, newLoggerName.length());
  }

  void ViLogger::removeSubLogger(const std::string& name) {
    std::lock_guard lock(m_printLock);
    m_subLoggers.erase(name);
  }

  IViLogger& ViLogger::level(LogLevel lvl) {
    m_currentLevel = lvl;
    return *this;
  }

  IViLogger& ViLogger::message(const std::string& msg) {

    std::lock_guard lock(m_printLock);

    if (LogLevel::Disabled == m_currentLevel || !m_print || m_currentLevel > m_allowedLevel->get())
      return *this;

    std::string lvlStr = ViLogLevel::levelStrings[static_cast<size_t>(m_currentLevel)];
    std::stringstream ss;

    std::string loggerName{};
    for(auto& n : m_parents) {
      loggerName += n + ".";
    }
    loggerName += m_name;

    ViLogger::maxLogNameLength = std::max(ViLogger::maxLogNameLength, loggerName.length());
    
    if(ViLogFormat::PlaintText == m_format) {

      if (m_getTime)
        ss << m_getTime();
    
      ss << "|" << std::left << std::setw(kLevelStrLen) << 
        lvlStr.substr(0, std::min(static_cast<size_t>(kLevelStrLen), lvlStr.length())) << "|";

      ss << std::left << std::setw(ViLogger::maxLogNameLength) << 
      loggerName.substr(0, std::min(static_cast<size_t>(ViLogger::maxLogNameLength), loggerName.length())) << "|";
      ss << msg << std::endl;

    } else if (ViLogFormat::Json == m_format) {

      ss << "{";
      if (m_getTime) {
        ss << "\"time\":\"" << m_getTime() << "\",";
      }
      ss << "\"level\":\"" << lvlStr << "\",";
      ss << "\"name\":\"" << loggerName << "\",";
      ss << "\"message\":\"" << msg << "\"";
      ss << "}\n";

    }

    m_print(ss.str());

    return *this;
  }

  ViLogger& ViLogger::operator=(ViLogger&& obj) noexcept {
    std::lock_guard lock(obj.m_printLock);
    m_name = obj.m_name; obj.m_name = "";
    m_getTime = obj.m_getTime; obj.m_getTime = nullptr;
    m_allowedLevel = obj.m_allowedLevel; obj.m_allowedLevel.reset();
    m_print = obj.m_print; obj.m_print = nullptr;
    m_format = obj.m_format; 
    return *this;
  }

}

