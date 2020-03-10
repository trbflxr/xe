//
// Created by FLXR on 9/2/2019.
//

#include "xepch.hpp"
#include <xe/string.hpp>

#include <xe/assert.hpp>
#include "external/utfcpp/utf8.h"

namespace xe::detail {

  template<size_t S>
  std::string wideToUtf8(std::wstring_view wide) {
    try {
      std::string utf8;
      static_assert((S == 2) || (S == 4), "Only utf16 / utf32 supports");
      if constexpr (S == 2) {
        utf8::utf16to8(wide.begin(), wide.end(), std::back_inserter(utf8));
      } else {
        utf8::utf32to8(wide.begin(), wide.end(), std::back_inserter(utf8));
      }
      return utf8;
    } catch ([[maybe_unused]]utf8::exception &e) {
      XE_ASSERT(false, "[xe::string] Bad wide string, exception: '{}'", e.what());
      return { };
    }
  }

  template<size_t S>
  std::wstring utf8ToWide(std::string_view utf8) {
    try {
      std::wstring wide;
      static_assert((S == 2) || (S == 4), "Only utf16 / utf32 supports");
      if constexpr (S == 2) {
        utf8::utf8to16(utf8.begin(), utf8.end(), std::back_inserter(wide));
      } else {
        utf8::utf8to32(utf8.begin(), utf8.end(), std::back_inserter(wide));
      }
      return wide;
    } catch (utf8::exception &e) {
      XE_ASSERT(false, "[xe::string] Bad utf8 string, exception: '{}'", e.what());
      return { };
    }
  }

  std::wstring winToWide(std::string_view utf8, UINT codePage) {
    if (utf8.empty()) {
      return { };
    }

    const auto size = ::MultiByteToWideChar(codePage, 0, utf8.data(),
                                            static_cast<int32_t>(utf8.size()), nullptr, 0);
    if (!size) {
      return { };
    }

    std::wstring wide;
    wide.resize(static_cast<size_t>(size), '\0');

    const auto written = ::MultiByteToWideChar(codePage, 0, utf8.data(),
                                               static_cast<int32_t>(utf8.size()), wide.data(), size);
    if (!written) {
      return { };
    }

    wide.resize(static_cast<size_t>(written));
    return wide;
  }

}

namespace xe::string {

  bool isWhitespace(char c) noexcept {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
  }

  bool isNumber(std::string_view str) noexcept {
    return std::all_of(str.cbegin(), str.cend(), [](auto c) {
      return (c >= '0' && c <= '9') || c == '.' || c == '-';
    });
  }

  std::string fixEscapedChars(std::string str) {
    static const std::vector<std::pair<char, std::string_view>> replaces = {{'\\', "\\\\"},
                                                                            {'\n', "\\n"},
                                                                            {'\r', "\\r"},
                                                                            {'\t', "\\t"},
                                                                            {'\"', "\\\""}};

    for (const auto &[from, to] : replaces) {
      auto pos = str.find(from);
      while (pos != std::string::npos) {
        str.replace(pos, 1, to);
        pos = str.find(from, pos + 2);
      }
    }
    return str;
  }

  std::string unfixEscapedChars(std::string str) {
    static const std::vector<std::pair<std::string_view, char>> replaces = {{"\\n",  '\n'},
                                                                            {"\\r",  '\r'},
                                                                            {"\\t",  '\t'},
                                                                            {"\\\"", '\"'},
                                                                            {"\\\\", '\\'}};

    for (const auto &[from, to] : replaces) {
      auto pos = str.find(from);
      while (pos != std::string::npos) {
        if (pos != 0 && str[pos - 1] == '\\')
          str.erase(str.begin() + --pos);
        else
          str.replace(pos, from.size(), 1, to);
        pos = str.find(from, pos + 1 + from.size());
      }
    }
    return str;
  }

  std::string wideToUtf8(std::wstring_view wide) {
    return detail::wideToUtf8<sizeof(std::wstring::value_type)>(wide);
  }

  std::wstring utf8ToWide(std::string_view utf8) {
#ifdef XE_PLATFORM_WINDOWS
    return detail::winToWide(utf8, CP_UTF8);
#else
    return detail::utf8ToWide<sizeof(std::wstring::value_type)>(utf8);
#endif
  }

  std::vector<std::string> split(std::string_view str, char delimiter) {
    std::vector<std::string> result;

    size_t current = 0;
    size_t previous = 0;
    current = str.find(delimiter);
    while (current != std::string::npos) {
      result.push_back(std::string(str.substr(previous, current - previous)));
      previous = current + 1;
      current = str.find(delimiter, previous);
    }
    result.push_back(std::string(str.substr(previous, current - previous)));

    return result;
  }

  std::vector<std::string> split(std::string_view str, std::string_view delimiters) {
    size_t start = 0;
    size_t end = str.find_first_of(delimiters);

    std::vector<std::string> result;

    while (end <= std::string::npos) {
      std::string token = std::string(str.substr(start, end - start));
      if (!token.empty()) {
        result.push_back(token);
      }

      if (end == std::string::npos) {
        break;
      }

      start = end + 1;
      end = str.find_first_of(delimiters, start);
    }

    return result;
  }

  const char *findToken(const char *str, std::string_view token) {
    const size_t tokenSize = token.size();

    const char *t = str;
    t = strstr(t, token.data());
    while (t) {
      bool left = str == t || isspace(t[-1]);
      bool right = !t[tokenSize] || isspace(t[tokenSize]);

      if (left && right) {
        return t;
      }
      t += tokenSize;
      t = strstr(t, token.data());
    }
    return nullptr;
  }

  const char *findChar(const char *str, char ch) {
    return strchr(str, ch);
  }

  std::string getWord(const char *str, const char **outPosition) {
    const size_t length = strlen(str);

    std::string res = str;
    for (size_t i = 0; i < length; ++i) {
      if (str[i] == 95) continue; //underscore
      if (str[i] < 48 || str[i] > 57) { //numbers
        if (str[i] < 65 || str[i] > 90) { //cap letters
          if (str[i] < 97 || str[i] > 122) { //letters
            res = std::string(str, i);
            if (outPosition) {
              *outPosition = str + i;
            }
            break;
          }
        }
      }
    }
    return res;
  }

  std::string getStatement(const char *str, const char **outPosition) {
    const char *end = strstr(str, ";");
    if (!end) {
      return std::string(str);
    }

    if (outPosition) {
      *outPosition = end;
    }

    const size_t length = end - str + 1;
    return std::string(str, length);
  }

  std::string replaceFirst(std::string str, std::string_view token, std::string_view to) {
    const auto startPos = str.find(token);

    if (startPos == std::string::npos) {
      return str;
    }

    str.replace(startPos, token.length(), to);
    return str;
  }

  std::string getDirectoryName(std::string_view str, const char *delims) {
    const size_t last_slash_idx = str.find_last_of(delims);
    if (last_slash_idx != std::string::npos) {
      return std::string(str.substr(0, last_slash_idx));
    }
    XE_CORE_ERROR("[String] Failed to parse directory path of '{}'", str);
    return "";
  }

  std::string getFileName(std::string_view str, bool includeExt, const char *delims) {
    std::string fileName(str);
    const size_t lastSlash = fileName.find_last_of(delims);
    if (std::string::npos != lastSlash) {
      fileName.erase(0, lastSlash + 1);
    }

    if (!includeExt) {
      const size_t period = fileName.rfind('.');
      if (std::string::npos != period) {
        fileName.erase(period, fileName.size() - period);
      }
    }
    return fileName;
  }

  std::string getFileExt(std::string_view str) {
    const char *dot = strrchr(str.data(), '.');
    if (!dot || dot == str) {
      return { };
    }
    return std::string(dot + 1);
  }

  std::vector<std::string> tokenize(std::string_view str) {
    return split(str, " \t\n");
  }

}
