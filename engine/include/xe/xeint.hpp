//
// Created by FLXR on 9/1/2019.
//

#ifndef XE_XEINT_HPP
#define XE_XEINT_HPP

typedef signed char int8;
typedef short int16;
typedef int int32;
typedef long long int64;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned uint32;
typedef unsigned long long uint64;

typedef uint8 byte;
typedef uint32 uint;

#ifdef _WIN32
  #ifdef _WIN64
typedef uint64 size_t;
  #else
typedef uint32 size_t;
  #endif
#endif

#endif //XE_XEINT_HPP
