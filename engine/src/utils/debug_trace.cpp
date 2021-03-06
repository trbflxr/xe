//
// Created by FLXR on 9/2/2019.
//

#include "xepch.hpp"
#include <xe/utils/debug_trace.hpp>

#define MTR_ENABLED 1
#include "external/minitrace/minitrace.h"

namespace xe::detail {

  void traceInit(const char *file) {
    mtr_init(file);
  }

  void traceShutdown() {
    mtr_flush();
    mtr_shutdown();
  }

  void traceBegin(const char *category, const char *name) {
    MTR_BEGIN(category, name);
  }

  void traceEnd(const char *category, const char *name) {
    MTR_END(category, name);
  }

  void traceMetaThreadName(const char *name) {
    MTR_META_THREAD_NAME(name);
  }

}
