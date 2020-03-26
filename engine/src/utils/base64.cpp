//
// Created by trbflxr on 3/26/2020.
//

#include "xepch.hpp"
#include <xe/utils/base64.hpp>

namespace xe {

  static inline bool isBase64(uint8_t c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
  }

  std::string base64::encode(const void *bytes, size_t size) {
    std::string ret;
    const uint8_t *data = static_cast<const uint8_t *>(bytes);
    int32_t i = 0;
    int32_t j = 0;
    uint8_t charArray3[3];
    uint8_t charArray4[4];

    while (size--) {
      charArray3[i++] = *(data++);
      if (i == 3) {
        charArray4[0] = (charArray3[0] & 0xfc) >> 2;
        charArray4[1] = ((charArray3[0] & 0x03) << 4) + ((charArray3[1] & 0xf0) >> 4);
        charArray4[2] = ((charArray3[1] & 0x0f) << 2) + ((charArray3[2] & 0xc0) >> 6);
        charArray4[3] = charArray3[2] & 0x3f;

        for (i = 0; (i < 4); i++) {
          ret += base64Chars()[charArray4[i]];
        }
        i = 0;
      }
    }

    if (i) {
      for (j = i; j < 3; j++) {
        charArray3[j] = '\0';
      }

      charArray4[0] = (charArray3[0] & 0xfc) >> 2;
      charArray4[1] = ((charArray3[0] & 0x03) << 4) + ((charArray3[1] & 0xf0) >> 4);
      charArray4[2] = ((charArray3[1] & 0x0f) << 2) + ((charArray3[2] & 0xc0) >> 6);
      charArray4[3] = charArray3[2] & 0x3f;

      for (j = 0; (j < i + 1); j++) {
        ret += base64Chars()[charArray4[j]];
      }

      while ((i++ < 3)) {
        ret += '=';
      }

    }

    return ret;
  }

  std::string base64::decode(std::string_view encoded) {
    size_t size = encoded.size();
    int32_t i = 0;
    int32_t j = 0;
    int32_t in = 0;
    uint8_t charArray4[4];
    uint8_t charArray3[3];
    std::string ret;

    while (size-- && (encoded[in] != '=') && isBase64(encoded[in])) {
      charArray4[i++] = encoded[in];
      in++;
      if (i == 4) {
        for (i = 0; i < 4; i++) {
          charArray4[i] = static_cast<uint8_t>(base64Chars().find(charArray4[i]));
        }

        charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
        charArray3[1] = ((charArray4[1] & 0xf) << 4) + ((charArray4[2] & 0x3c) >> 2);
        charArray3[2] = ((charArray4[2] & 0x3) << 6) + charArray4[3];

        for (i = 0; (i < 3); i++) {
          ret += charArray3[i];
        }
        i = 0;
      }
    }

    if (i) {
      for (j = i; j < 4; j++) {
        charArray4[j] = 0;
      }

      for (j = 0; j < 4; j++) {
        charArray4[j] = static_cast<uint8_t>(base64Chars().find(charArray4[j]));
      }

      charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
      charArray3[1] = ((charArray4[1] & 0xf) << 4) + ((charArray4[2] & 0x3c) >> 2);
      charArray3[2] = ((charArray4[2] & 0x3) << 6) + charArray4[3];

      for (j = 0; (j < i - 1); j++) {
        ret += charArray3[j];
      }
    }

    return ret;
  }

  std::string_view base64::base64Chars() {
    static const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    return chars;
  }

}
