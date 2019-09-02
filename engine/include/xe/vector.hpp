//
// Created by FLXR on 9/1/2019.
//

#ifndef XE_VECTOR_HPP
#define XE_VECTOR_HPP

#include <vector>
#pragma warning(push, 0)
#include <tbb/cache_aligned_allocator.h>
#pragma warning(pop)

namespace xe {

  template <typename T>
  using vector = std::vector<T, tbb::cache_aligned_allocator<T>>;

}

#endif //XE_VECTOR_HPP
