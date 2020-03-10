//
// Created by FLXR on 9/2/2019.
//

#ifndef XE_LOGGER_HPP
#define XE_LOGGER_HPP

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <xe/common.hpp>
#include <xe/utils/noncopyable.hpp>

namespace xe {

  enum class LogLevel {
    Trace = 0,
    Info,
    Warn,
    Error,
    Critical,
    Off
  };

  class XE_API Logger : NonCopyable {
  public:
    static void setLogLevel(LogLevel engineLevel, LogLevel clientLevel);

    static inline spdlog::logger &coreRef() { return *ref().coreLogger_; }
    static inline spdlog::logger &clientRef() { return *ref().clientLogger_; }

  private:
    Logger();

    static Logger &ref();

  private:
    std::shared_ptr<spdlog::logger> coreLogger_;
    std::shared_ptr<spdlog::logger> clientLogger_;
  };

}

//core
#define XE_CORE_TRACE(...)    ::xe::Logger::coreRef().trace(__VA_ARGS__)
#define XE_CORE_INFO(...)     ::xe::Logger::coreRef().info(__VA_ARGS__)
#define XE_CORE_WARN(...)     ::xe::Logger::coreRef().warn(__VA_ARGS__)
#define XE_CORE_ERROR(...)    ::xe::Logger::coreRef().error(__VA_ARGS__)
#define XE_CORE_CRITICAL(...) ::xe::Logger::coreRef().critical(__VA_ARGS__)

//client
#define XE_TRACE(...)         ::xe::Logger::clientRef().trace(__VA_ARGS__)
#define XE_INFO(...)          ::xe::Logger::clientRef().info(__VA_ARGS__)
#define XE_WARN(...)          ::xe::Logger::clientRef().warn(__VA_ARGS__)
#define XE_ERROR(...)         ::xe::Logger::clientRef().error(__VA_ARGS__)
#define XE_CRITICAL(...)      ::xe::Logger::clientRef().critical(__VA_ARGS__)

#endif //XE_LOGGER_HPP
