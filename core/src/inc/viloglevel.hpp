#pragma once

#include "iviloglevel.hpp"
#include <string>
#include <set>

namespace vilogger {
    
  class ViLogLevel : public IViLogLevel {
    
    LogLevel m_llvl;  
    
  public:

    virtual void set(LogLevel) override;
    virtual LogLevel get() const override;

  };
}

