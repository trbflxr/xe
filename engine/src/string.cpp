//
// Created by FLXR on 9/2/2019.
//

#include "xepch.hpp"
#include <cstring>
#include <xe/string.hpp>

namespace xe {

  const size_t string::npos = std::basic_string<uint>::npos;

  static void fromWideString(const wchar_t *str, std::string &dest) {
#ifdef XE_PLATFORM_WINDOWS
    const size_t size = WideCharToMultiByte(CP_UTF8, 0, str, -1, nullptr, 0, nullptr, nullptr);
    dest.reserve(size);
    WideCharToMultiByte(CP_UTF8, 0, str, -1, dest.data(), size, nullptr, nullptr);
#else
  #error only windows currently supported
#endif
  }

  static std::wstring toWideString(const std::string &src) {
#ifdef XE_PLATFORM_WINDOWS
    const size_t size = MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, nullptr, 0);
    std::wstring buffer(size, L'\0');

    MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, buffer.data(), size);

    return buffer;
#else
  #error only windows currently supported
#endif
  }

  string::string(char ch) {
    base_ += ch;
  }

  string::string(wchar_t ch) {
    const uint c = wcharToUTF8(ch);
    base_ += reinterpret_cast<const char *>(&c);
  }

  string::string(uint ch) {
    base_ += reinterpret_cast<const char *>(&ch);
  }

  string::string(const char *str) {
    if (str) {
      base_ = str;
    }
  }

  string::string(const std::string &str) {
    base_ = str;
  }

  string::string(const wchar_t *str) {
    if (str) {
      fromWideString(str, base_);
    }
  }

  string::string(const std::wstring &str) {
    fromWideString(str.c_str(), base_);
  }

  string::string(const string &copy) :
      base_(copy.base_) { }

  void string::replace(size_t position, size_t length, const string &replaceWith) {
    base_.replace(position, length, replaceWith.base_);
  }

  void string::replace(const string &searchFor, const string &replaceWith) {
    const size_t step = replaceWith.size();
    const size_t len = searchFor.size();
    size_t pos = find(searchFor);

    while (pos != npos) {
      replace(pos, len, replaceWith);
      pos = find(searchFor, pos + step);
    }
  }

  std::wstring string::toWide() const {
    return toWideString(base_);
  }

  string &string::operator=(const string &right) {
    base_ = right.base_;
    return *this;
  }

  string &string::operator+=(const string &right) {
    base_ += right.base_;
    return *this;
  }

  char string::operator[](size_t index) const {
    return base_[index];
  }

  char &string::operator[](size_t index) {
    return base_[index];
  }

  vector<string> string::split(char delimiter) const {
    return split(string(delimiter));
  }

  vector<string> string::split(const string &delimiters) const {
    size_t start = 0;
    size_t end = base_.find_first_of(delimiters.base_);

    vector<string> result;

    while (end <= string::npos) {
      string token = base_.substr(start, end - start);
      if (!token.empty())
        result.push_back(token);

      if (end == string::npos) break;

      start = end + 1;
      end = base_.find_first_of(delimiters.base_, start);
    }

    return result;
  }

  const char *string::findToken(const char *str, const string &token) {
    const size_t tokenSize = token.size();

    const char *t = str;
    while ((t = strstr(t, token.c_str()))) {
      bool left = str == t || isspace(t[-1]);
      bool right = !t[tokenSize] || isspace(t[tokenSize]);

      if (left && right) return t;

      t += tokenSize;
    }
    return nullptr;
  }

  const char *string::findChar(const char *str, char ch) {
    return strchr(str, ch);
  }

  string string::getWord(const char *str, const char **outPosition) {
    const size_t length = strlen(str);

    std::string res = str;
    for (size_t i = 0; i < length; ++i) {
      if (str[i] == 95) continue; //underscore
      if (str[i] < 48 || str[i] > 57) { //numbers
        if (str[i] < 65 || str[i] > 90) { //cap letters
          if (str[i] < 97 || str[i] > 122) { //letters
            res = std::string(str, i);

            if (outPosition) *outPosition = str + i;

            break;
          }
        }
      }
    }
    return res;
  }

  string string::getStatement(const char *str, const char **outPosition) {
    const char *end = strstr(str, ";");
    if (!end) return string(str);

    if (outPosition) *outPosition = end;

    const uint length = end - str + 1;
    return std::string(str, length);
  }

  vector<string> string::tokenize(const string &str) {
    return str.split(" \t\n");
  }

  bool operator==(const string &left, const string &right) {
    return left.base_ == right.base_;
  }

  bool operator!=(const string &left, const string &right) {
    return !(left == right);
  }

  bool operator<(const string &left, const string &right) {
    return left.base_ < right.base_;
  }

  bool operator>(const string &left, const string &right) {
    return right < left;
  }

  bool operator<=(const string &left, const string &right) {
    return !(right < left);
  }

  bool operator>=(const string &left, const string &right) {
    return !(left < right);
  }

  string operator+(const string &left, const string &right) {
    string string = left;
    string += right;

    return string;
  }

  uint wcharToUTF8(uint wchar) {
    uint utf8 = 0;
    char *c = (char *) &utf8;

    if (wchar < 0x80) {
      utf8 = wchar;
    } else if (wchar < 0x800) {
      c[0] = static_cast<char>((wchar >> 6) | 0xC0);
      c[1] = static_cast<char>((wchar & 0x3F) | 0x80);
    } else if (wchar < 0x10000) {
      c[0] = static_cast<char>((wchar >> 12) | 0xE0);
      c[1] = static_cast<char>(((wchar >> 6) & 0x3F) | 0x80);
      c[2] = static_cast<char>((wchar & 0x3F) | 0x80);
    } else if (wchar < 0x110000) {
      c[0] = static_cast<char>((wchar >> 18) | 0xF0);
      c[1] = static_cast<char>(((wchar >> 12) & 0x3F) | 0x80);
      c[2] = static_cast<char>(((wchar >> 6) & 0x3F) | 0x80);
      c[3] = static_cast<char>((wchar & 0x3F) | 0x80);
    }

    return utf8;
  }

}
