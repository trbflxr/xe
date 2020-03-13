//
// Created by FLXR on 9/2/2019.
//

#ifndef XE_DEBUG_TRACE_HPP
#define XE_DEBUG_TRACE_HPP

#include <xe/common.hpp>

#ifdef XE_DEBUG_TRACING
  #define XE_TRACE_INIT(file)              ::xe::detail::traceInit(file)
  #define XE_TRACE_SHUTDOWN()              ::xe::detail::traceShutdown()
  #define XE_TRACE_BEGIN(category, name)   ::xe::detail::traceBegin(category, name)
  #define XE_TRACE_END(category, name)     ::xe::detail::traceEnd(category, name)
  #define XE_TRACE_META_THREAD_NAME(name)  ::xe::detail::traceMetaThreadName(name)
#else
  #define XE_TRACE_INIT(file)              ((void)0)
  #define XE_TRACE_SHUTDOWN()              ((void)0)
  #define XE_TRACE_BEGIN(category, name)   ((void)0)
  #define XE_TRACE_END(category, name)     ((void)0)
  #define XE_TRACE_META_THREAD_NAME(name)  ((void)0)
#endif

namespace xe::detail {

  XE_API void traceInit(const char *file);
  XE_API void traceShutdown();
  XE_API void traceBegin(const char *category, const char *name);
  XE_API void traceEnd(const char *category, const char *name);
  XE_API void traceMetaThreadName(const char *name);

}


#endif //XE_DEBUG_TRACE_HPP
