//
// Created by trbflxr on 3/26/2020.
//

#ifndef XE_BASE64_HPP
#define XE_BASE64_HPP

#include <xe/common.hpp>

namespace xe {

  class XE_API base64 {
  public:
    static std::string encode(const void *bytes, size_t size);
    static std::string decode(std::string_view encoded);

  private:
    static std::string_view base64Chars();
  };

}

#endif //XE_BASE64_HPP
