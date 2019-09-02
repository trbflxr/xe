//
// Created by FLXR on 9/1/2019.
//

#ifndef XE_REFERENCE_COUNTER_HPP
#define XE_REFERENCE_COUNTER_HPP

#include <cassert>
#include <xe/xeint.hpp>
#include <xe/utils/noncopyable.hpp>

namespace xe {

  class ReferenceCounter : NonCopyable {
  public:
    inline uint count() const noexcept { return count_; }

    inline void ref() noexcept { ++count_; }

    inline void unref() {
      assert(count_ > 0);
      --count_;
      if (count_ == 0) {
        delete this;
      }
    }

    inline void unrefNoDelete() noexcept {
      assert(count_ > 0);
      --count_;
    }

  protected:
    inline ReferenceCounter() noexcept : count_(0) { }

  private:
    uint count_;
  };

};

#endif //XE_REFERENCE_COUNTER_HPP
