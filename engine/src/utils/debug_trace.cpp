//
// Created by FLXR on 9/2/2019.
//

#include "xepch.hpp"
#define MTR_ENABLED
#include "external/minitrace/minitrace.hpp"
#include <xe/utils/debug_trace.hpp>

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
