//
// Created by FLXR on 9/2/2019.
//

#include "xepch.hpp"
#include <xe/common.hpp>

//todo: remove
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace xe {

  void init() {

    int a = 1;

    spdlog::info("Welcome to spdlog!");
    spdlog::error("Some error message with arg: {}", 1);

    spdlog::warn("Easy padding in numbers like {:08d}", 12);
    spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    spdlog::info("Support for floats {:03.2f}", 1.23456);
    spdlog::info("Positional args are {1} {0}..", "too", "supported");
    spdlog::info("{:<30}", "left aligned");
    spdlog::info("{:<30}", "zaloopa");

    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    spdlog::debug("This message should be displayed..");

    // change log pattern
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

    // Set the default logger to file logger
//    auto file_logger = spdlog::basic_logger_mt("basic_logger", "logs/basic.txt");
//    spdlog::set_default_logger(file_logger);
  }

  void shutdown() {

  }

}
