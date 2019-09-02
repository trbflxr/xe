//
// Created by FLXR on 9/1/2019.
//

#ifndef XE_ARRAY_HPP
#define XE_ARRAY_HPP

#include <vector>
#include <tbb/cache_aligned_allocator.h>

namespace xe {

  template <typename T>
  using array = std::vector<T, tbb::cache_aligned_allocator<T>>;

}

#endif //XE_ARRAY_HPP
