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
  scoped_ptr<T> make_scope(Args &&... args) {
    return scoped_ptr<T>(new T(std::forward<Args>(args)...));
  }

  template<typename T>
  using ref_ptr = std::shared_ptr<T>;

  template<typename T, typename... Args>
  ref_ptr<T> make_ref(Args &&... args) {
    return ref_ptr<T>(new T(std::forward<Args>(args)...));
  }
}

#endif //XE_MEMORY_HPP
