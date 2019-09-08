//
// Created by FLXR on 9/2/2019.
//

#ifndef XE_STRING_HPP
#define XE_STRING_HPP

#include <iterator>
#include <string>
#include <xe/common.hpp>
#include <xe/vector.hpp>

#pragma warning(disable : 4251) //export std::string

namespace xe {

  class XE_API string {
  public:
    typedef std::string::iterator iterator;
    typedef std::string::const_iterator constIterator;

    static const size_t npos;

  public:
    string() = default;

    string(char ch);
    string(wchar_t ch);
    string(uint ch);

    string(const char *str);
    string(const std::string &str);

    string(const wchar_t *str);
    string(const std::wstring &str);

    string(const string &copy);

    inline void clear() { base_.clear(); }
    inline size_t size() const { return base_.size(); }
    inline bool empty() const { return base_.empty(); }

    inline void erase(size_t position, size_t count = 1) { base_.erase(position, count); }

    inline void insert(size_t position, const string &str) { base_.insert(position, str.base_); }

    inline size_t find(const string &str, size_t start = 0) const { return base_.find(str.base_, start); }

    void replace(size_t position, size_t length, const string &replaceWith);
    void replace(const string &searchFor, const string &replaceWith);

    inline string substr(size_t position, size_t length = npos) const { return base_.substr(position, length); }

    inline const char *data() const { return base_.data(); }
    inline const char *c_str() const { return base_.c_str(); }
    inline char *data() { return base_.data(); }

    inline iterator begin() { return base_.begin(); }
    inline constIterator begin() const { return base_.begin(); }

    inline iterator end() { return base_.end(); }
    inline constIterator end() const { return base_.end(); }

    std::wstring toWide() const;

    inline string &operator=(const string &right);
    inline string &operator+=(const string &right);

    inline char operator[](size_t index) const;
    inline char &operator[](size_t index);

    template<typename OStream>
    inline friend OStream &operator<<(OStream &os, const string &s) { return os << s.c_str(); }

    static uint wcharToUTF8(uint wchar);

    vector<string> split(char delimiter) const;
    vector<string> split(const string &delimiters) const;

    static const char *findToken(const char *str, const string &token);
    static const char *findChar(const char *str, char ch);
    static string getWord(const char *str, const char **outPosition);
    static string getStatement(const char *str, const char **outPosition = nullptr);

    static string getFileName(const string &str, bool includeExt = true, const char *delims = "\\/");
    static string getFileExt(const string &str);

    static vector<string> tokenize(const string &str);

  private:
    friend bool operator==(const string &left, const string &right);
    friend bool operator<(const string &left, const string &right);

  private:
    std::string base_;
  };

  inline string &string::operator=(const string &right) {
    base_ = right.base_;
    return *this;
  }

  inline string &string::operator+=(const string &right) {
    base_ += right.base_;
    return *this;
  }

  inline char string::operator[](size_t index) const {
    return base_[index];
  }

  inline char &string::operator[](size_t index) {
    return base_[index];
  }

  inline bool operator==(const string &left, const string &right) {
    return left.base_ == right.base_;
  }

  inline bool operator!=(const string &left, const string &right) {
    return !(left == right);
  }

  inline bool operator<(const string &left, const string &right) {
    return left.base_ < right.base_;
  }

  inline bool operator>(const string &left, const string &right) {
    return right < left;
  }

  inline bool operator<=(const string &left, const string &right) {
    return !(right < left);
  }

  inline bool operator>=(const string &left, const string &right) {
    return !(left < right);
  }

  inline string operator+(const string &left, const string &right) {
    string string = left;
    string += right;

    return string;
  }

}

namespace std {

#if defined(__MINGW64__)
  template<>
  struct hash<xe::string> : public __hash_base<size_t, xe::string> {
    size_t operator()(const xe::string &s) const noexcept { return std::_Hash_impl::hash(s.data(), s.size()); }
  };
#elif defined(_MSC_VER)
  template<>
  struct hash<xe::string> {
    size_t operator()(const xe::string &s) const noexcept { return _Hash_array_representation(s.c_str(), s.size()); }
  };
#endif
}

#endif //XE_STRING_HPP
