//
// Created by FLXR on 9/1/2019.
//

#ifndef XE_NONCOPYABLE_HPP
#define XE_NONCOPYABLE_HPP

namespace xe {

  struct NonCopyable {
    NonCopyable(NonCopyable &) = delete;
    NonCopyable(NonCopyable &&) = delete;
    NonCopyable &operator=(NonCopyable &) = delete;
    NonCopyable &operator=(NonCopyable &&) = delete;

  protected:
    NonCopyable() = default;
    virtual ~NonCopyable() = default;
  };

}

#endif //XE_NONCOPYABLE_HPP
