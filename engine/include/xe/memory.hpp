//
// Created by FLXR on 9/1/2019.
//

#ifndef XE_MEMORY_HPP
#define XE_MEMORY_HPP

#include <memory>

namespace xe {

  template<typename T>
  using scoped_ptr = std::unique_ptr<T>;

  template<typename T, typename... Args>
  scoped_ptr<T> make_scoped(Args &&... args) {
    return scoped_ptr<T>(new T(std::forward<Args>(args)...));
  }

  template<typename T>
  using ref_ptr = std::shared_ptr<T>;

  template<typename T, typename... Args>
  ref_ptr<T> make_ref(Args &&... args) {
    return ref_ptr<T>(new T(std::forward<Args>(args)...));
  }

  template<class T>
  struct memory {
    scoped_ptr<T[]> data;
    size_t size = 0;

    T &operator[](size_t pos) {
      assert(pos < size && "Invalid position...");
      return data[pos];
    }

    const T &operator[](size_t pos) const {
      assert(pos < size && "Invalid position...");
      return data[pos];
    }

    memory() { }

    memory(memory &&m) {
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
  };

}

#endif //XE_MEMORY_HPP
