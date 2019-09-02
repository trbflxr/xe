//
// Created by FLXR on 9/2/2019.
//

#include <xe/common.hpp>
#include <xe/utils/debug_trace.hpp>

#ifdef XE_PLATFORM_WINDOWS
  #define WIN32_LEAN_AND_MEAN
  #define WIN32_EXTRA_LEAN
  #include <windows.h>
#endif

namespace xe {

  void init() {
    XE_TRACE_INIT("xetrace.json");
    XE_TRACE_META_THREAD_NAME("main thread");

    XE_TRACE_BEGIN("xe", "dummy");
    _sleep(100);
    XE_TRACE_END("xe", "dummy");

    XE_TRACE_BEGIN("xe", "dummy1");
    _sleep(1000);
    XE_TRACE_END("xe", "dummy1");

    XE_TRACE_SHUTDOWN();
  }

  void shutdown() {

  }

}
