//
// Created by FLXR on 9/2/2019.
//

#ifndef XE_DEBUG_TRACE_HPP
#define XE_DEBUG_TRACE_HPP

#include <xe/common.hpp>

#ifdef XE_DEBUG_TRACING
  #define XE_TRACE_INIT(file)              xe::detail::traceInit(file)
  #define XE_TRACE_SHUTDOWN()              xe::detail::traceShutdown()
  #define XE_TRACE_BEGIN(category, name)   xe::detail::traceBegin(category, name)
  #define XE_TRACE_END(category, name)     xe::detail::traceEnd(category, name)
  #define XE_TRACE_META_THREAD_NAME(name)  xe::detail::traceMetaThreadName(name)
#else
  #define XE_TRACE_INIT(file)
  #define XE_TRACE_SHUTDOWN()
  #define XE_TRACE_BEGIN(category, name)
  #define XE_TRACE_END(category, name)
  #define XE_TRACE_META_THREAD_NAME(name)
#endif

namespace xe::detail {

  void XE_API traceInit(const char *file);
  void XE_API traceShutdown();
  void XE_API traceBegin(const char *category, const char *name);
  void XE_API traceEnd(const char *category, const char *name);
  void XE_API traceMetaThreadName(const char *name);

}


#endif //XE_DEBUG_TRACE_HPP
