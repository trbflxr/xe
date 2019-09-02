//
// Created by FLXR on 9/2/2019.
//

#include "xepch.hpp"
#include <xe/utils/logger.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace xe {

  spdlog::level::level_enum xeToSpdLevel(LogLevel level) {
    switch (level) {
      case LogLevel::Info: return spdlog::level::info;
      case LogLevel::Warn: return spdlog::level::warn;
      case LogLevel::Error: return spdlog::level::err;
      case LogLevel::Critical: return spdlog::level::critical;
      case LogLevel::Off: return spdlog::level::off;
      default: return spdlog::level::trace;
    }
  }

  Logger::Logger() {
    spdlog::set_pattern("[%H:%M:%S:%e] [%n] %^[%L]%$ [thread %t]: %v");

    coreLogger_ = spdlog::stdout_color_mt("XE");
    coreLogger_->set_level(xeToSpdLevel(LogLevel::Trace));

    clientLogger_ = spdlog::stdout_color_mt("CLIENT");
    clientLogger_->set_level(xeToSpdLevel(LogLevel::Trace));

#ifdef XE_PLATFORM_WINDOWS
    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IONBF, 2048);
#endif
  }

  Logger &Logger::ref() {
    static ref_ptr<Logger> logger;
    if (!logger) {
      logger.reset(new Logger());
    }
    return *logger;
  }

  void Logger::setLogLevel(LogLevel engineLevel, LogLevel clientLevel) {
    ref().coreLogger_->set_level(xeToSpdLevel(engineLevel));
    ref().clientLogger_->set_level(xeToSpdLevel(clientLevel));
  }

}
