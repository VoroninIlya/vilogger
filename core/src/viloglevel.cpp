#include "viloglevel.hpp"

namespace vilogger {

  void ViLogLevel::set(LogLevel lvl) {
    m_llvl = lvl;
  }
  
  LogLevel ViLogLevel::get() const {
    return m_llvl;
  }

}
