//
// Created by FLXR on 9/2/2019.
//

#ifndef XE_STRING_HPP
#define XE_STRING_HPP

#include <vector>
#include <string>
#include <xe/common.hpp>

namespace xe::string {

  bool isWhitespace(char c) noexcept;
  bool isNumber(std::string_view str) noexcept;

  std::string fixEscapedChars(std::string str);
  std::string unfixEscapedChars(std::string str);

  std::string wideToUtf8(std::wstring_view wide);
  std::wstring utf8ToWide(std::string_view utf8);

  std::vector<std::string> split(std::string_view str, char delimiter);
  std::vector<std::string> split(std::string_view str, std::string_view delimiters);

  const char *findToken(const char *str, std::string_view token);
  const char *findChar(const char *str, char ch);
  std::string getWord(const char *str, const char **outPosition);
  std::string getStatement(const char *str, const char **outPosition = nullptr);

  std::string replaceFirst(std::string str, std::string_view token, std::string_view to);

  std::string getDirectoryName(std::string_view str, const char *delims = "\\/");
  std::string getFileName(std::string_view str, bool includeExt = true, const char *delims = "\\/");
  std::string getFileExt(std::string_view str);

  std::vector<std::string> tokenize(std::string_view str);

  template<typename T>
  static std::string to(T val) {
    if constexpr (std::is_same_v<std::string, T> || std::is_same_v<const char *, T>) {
      return val;
    } else if constexpr (std::is_enum_v<T>) {
      typedef typename std::underlying_type<T>::type safe_type;
      return std::to_string(static_cast<safe_type>(val));
    } else if constexpr (std::is_same_v<bool, T>) {
      return val ? "true" : "false";
    } else if constexpr (std::is_same_v<std::nullptr_t, T>) {
      return "null";
    } else if constexpr (is_optional_v<T>) {
      if (!val.has_value()) {
        return "null";
      }
      return to(*val);
    } else if constexpr (std::is_same_v<char, T>) {
      return std::string(1, val);
    } else {
      return std::to_string(val);
    }
  }

  template<typename T>
  static T from(const std::string &str) {
    if constexpr (std::is_same_v<std::string, T>) {
      return str;
    } else if constexpr (std::is_enum_v<T>) {
      typedef typename std::underlying_type<T>::type safe_type;
      return static_cast<T>(from<safe_type>(str));
    } else if constexpr (std::is_same_v<bool, T>) {
      return str == "true" || from<std::optional<int32_t>>(str) == 1;
    } else if constexpr (is_optional_v<T>) {
      typedef typename T::value_type base_type;
      base_type temp;
      std::istringstream iss(str);

      if ((iss >> temp).fail()) {
        return std::nullopt;
      }
      return temp;
    } else {
      long double temp = 0.0;
      std::istringstream iss(str);
      iss >> temp;
      return static_cast<T>(temp);
    }
  }

}

#endif //XE_STRING_HPP
