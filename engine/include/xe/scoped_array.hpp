//
// Created by FLXR on 9/1/2019.
//

#ifndef XE_SCOPED_ARRAY_HPP
#define XE_SCOPED_ARRAY_HPP

#include <cassert>
#include <cstring>
#include <algorithm>
#include <xe/xeint.hpp>
#include <xe/utils/noncopyable.hpp>

namespace xe {

  template<class T, class T_index = uint>
  class scoped_array : NonCopyable {
  public:
    inline scoped_array() : px_(nullptr), size_(0) { }
    inline  ~scoped_array() override { delete[] px_; }

    T *alloc(size_t size) {
      internalReset(new T[size], size);
      return px_;
    }

    void release() {
      internalReset(nullptr, 0);
    }

    void copy(const T *begin, const T *end) {
      const size_t elements = ((size_t) end - (size_t) begin) / sizeof(T);
      internalReset(reinterpret_cast<T *>(new T[elements]), elements);
      for (size_t i = 0; i < elements; ++i) {
        new(px_ + i) T(begin[i]);
      }
    }

    void copy(const scoped_array<T> &other) {
      if (other.px_) {
        copy(&other[0], &other[0] + other.size());
      } else {
        internalReset(nullptr, 0);
      }
    }

    T &operator[](const T_index i) const {
      assert(px_ != 0 && "Invalid array, not allocated yet");
      assert(i >= 0 && "Invalid index (underflow)");
      assert(i < size_ && "Invalid index (overflow)");
      return px_[i];
    }

    T *get() const {
      return px_;
    }

    inline size_t size() const { return size_; }
    inline size_t sizeInBytes() const { return size() * sizeof(T); }

    inline bool valid() const { return px_ != 0; }

    void swap(scoped_array &b) {
      T *tmp = b.px_;
      b.px_ = px_;
      px_ = tmp;
      std::swap(b.size_, size_);
    }

    void memClear() {
      memset(px_, 0, sizeInBytes());
    }

    void memSet(const T &value) {
      T_index total = size();
      for (T_index i = 0; i < total; ++i) {
        (*this)[i] = value;
      }
    }

    void memCopy(const T_index indexOffset, const T *from, const T_index elementsToCopy) {
      assert(elementsToCopy + indexOffset <= size() && "Too many elements to copy");
      for (T_index i = 0; i < elementsToCopy; ++i) {
        (*this)[i + indexOffset] = from[i];
      }
    }

    inline T *begin() { return &px_[0]; }
    inline T *end() { return &px_[size_]; }

    inline const T *begin() const { return &px_[0]; }
    inline const T *end() const { return &px_[size_]; }

  private:
    inline scoped_array(T *p, const T_index size) : px_(p), size_(size) { }

    void internalReset(T *p, const T_index size) {
      assert(p == 0 || p != px_);
      scoped_array<T, T_index>(p, size).swap(*this);
    }

  private:
    T *px_;
    T_index size_;
  };

  template<class T>
  inline void swap(scoped_array<T> &a, scoped_array<T> &b) {
    a.swap(b);
  }

}

#endif //XE_SCOPED_ARRAY_HPP
