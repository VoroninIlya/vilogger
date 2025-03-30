#pragma once

#include "ivilogger.hpp"
#include "iviloglevel.hpp"
#include <functional>
#include <string>
#include <memory>
#include <mutex>

namespace vilogger {

  class ViLogger : public IViLogger {

    std::string m_name{};
    std::shared_ptr<IViLogLevel> m_allowedLevel{};
    LogLevel m_currentLevel{};

    std::function<std::string()> m_getTime = nullptr;
    std::function<void(const std::string&)> m_print = nullptr;

    std::mutex m_printLock{};

    std::vector<std::string> m_parents{};
    std::map<std::string, std::shared_ptr<IViLogger>> m_subLoggers{};

    ViLogFormat m_format = ViLogFormat::PlaintText;

  protected:
    static size_t maxLogNameLength;

  public:
    ViLogger() = delete;
    ViLogger(const std::string&,
      std::shared_ptr<IViLogLevel>, 
      std::function<void(const std::string&)>,
      std::function<std::string()>
    );

    ViLogger(const ViLogger&) = delete;
    ViLogger(ViLogger&&) noexcept;

    virtual void setAllowedLogLevel(LogLevel) override;
    virtual LogLevel getAllowedLogLevel(void) const override;

    virtual void setLogFormat(ViLogFormat) override;
    virtual ViLogFormat getLogFormat(void) const override;

    virtual std::map<std::string, std::shared_ptr<IViLogger>> getSubLoggers() const override;
    virtual IViLogger& getSubLogger(const std::string&) override;

    virtual void addSubLogger(const std::string&) override;
    virtual void removeSubLogger(const std::string&) override;

    virtual IViLogger& level(LogLevel lvl = LogLevel::Info) override;
    virtual IViLogger& message(const std::string&) override;

    ViLogger& operator=(const ViLogger&) = delete;
    virtual ViLogger& operator=(ViLogger&&) noexcept;

    virtual ~ViLogger() = default;

  };

}

