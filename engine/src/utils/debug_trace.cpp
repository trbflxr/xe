//
// Created by FLXR on 9/2/2019.
//

#include "xepch.hpp"
#include <xe/utils/debug_trace.hpp>
#pragma warning(push, 0)
#include <minitrace/minitrace.h>
#pragma warning(pop)

namespace xe::detail {

  void traceInit(const char *file) {
    mtr_init(file);
  }

  void traceShutdown() {
    mtr_flush();
    mtr_shutdown();
  }

  void traceScope(const char *category, const char *name) {
    MTR_SCOPE(category, name);
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
