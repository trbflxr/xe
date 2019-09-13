//
// Created by FLXR on 9/2/2019.
//

#ifndef XE_XEPCH_HPP
#define XE_XEPCH_HPP

#include <cassert>
#include <cstring>

#include <atomic>
#include <thread>
#include <mutex>
#include <algorithm>
#include <utility>
#include <sstream>

#include <xe/memory.hpp>
#include <xe/string.hpp>
#include <xe/vector.hpp>
#include <xe/unordered_map.hpp>

#include <xe/math/math.hpp>

#include <xe/utils/debug_trace.hpp>
#include <xe/utils/logger.hpp>

#ifdef XE_PLATFORM_WINDOWS
  #include <Windows.h>
#endif

#endif //XE_XEPCH_HPP
