//
// Created by trbflxr on 2/29/2020.
//

#ifndef XE_ASSERT_HPP
#define XE_ASSERT_HPP

#include <xe/utils/logger.hpp>

#ifdef _MSC_VER
  #define XE_DEBUG_BREAK __debugbreak
#else
  #add SIGTRAP??
#endif

#ifdef XE_DEBUG
  #define XE_ASSERT(_expression, _format_str, ...)                                \
  {                                                                               \
    if(!(_expression)){                                                           \
      auto _format_result = fmt::format(FMT_STRING((_format_str)), __VA_ARGS__);  \
      XE_CORE_CRITICAL("Assertion failed\n"                                       \
                       "\t{} : {}\n"                                              \
                       "\tExpression: ({})\n"                                     \
                       "\t{}",                                                    \
                       __FILE__, __LINE__, #_expression, _format_result);         \
      XE_DEBUG_BREAK();                                                           \
    }                                                                             \
  }
#else
  #define XE_ASSERT(expression, ...) ((void)0)
#endif

#endif //XE_ASSERT_HPP
