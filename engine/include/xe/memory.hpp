//
// Created by FLXR on 9/1/2019.
//

#ifndef XE_MEMORY_HPP
#define XE_MEMORY_HPP

#include <memory>
#include <xe/assert.hpp>

namespace xe {

  template<class T>
  struct memory {
    std::unique_ptr<T[]> data;
    size_t size = 0;

    memory() = default;

    memory(memory &&m) noexcept {
      data = std::move(m.data);
      size = m.size;
      m.size = 0;
    }

    memory(const memory &m) = delete;
    memory &operator=(memory &&) = delete;
    memory &operator=(const memory &) = delete;

    void copy(const T *ptr, size_t c) {
      data = std::make_unique<T[]>(c);
      for (size_t i = 0; i < c; ++i) {
        data[i] = ptr[i];
      }
      size = c;
    }

    void copy(const memory &m) { copy(m.data.get(), m.size); }

    void alloc(size_t c) {
      data = std::make_unique<T[]>(c);
      size = c;
    }

    T &operator[](size_t pos) {
      XE_ASSERT(pos < size, "Invalid position: {}", pos);
      return data[pos];
    }

    const T &operator[](size_t pos) const {
      XE_ASSERT(pos < size, "Invalid position: {}", pos);
      return data[pos];
    }
  };

}

#endif //XE_MEMORY_HPP
