//
// Created by FLXR on 9/1/2019.
//

#ifndef XE_VECTOR_HPP
#define XE_VECTOR_HPP

#include <vector>
#include <tbb/cache_aligned_allocator.h>

namespace xe {

  template <typename T>
  using vector = std::vector<T, tbb::cache_aligned_allocator<T>>;

}

#endif //XE_VECTOR_HPP
