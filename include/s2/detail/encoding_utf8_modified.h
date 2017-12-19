#pragma once

#include <s2/detail/encoding.h>
#include <s2/detail/raw_encoding_utf8.h>

namespace s2::encoding {

struct utf8_modified : raw::utf8 {
  static constexpr validation_result allowed = validation::overlong_zero | validation::surrogate_in_non_surrogate_encoding;
  template <typename It>
  size_t encode(It& output, char32_t chr) {
    if (chr < 0x10000) {
      return raw::utf8::encode(output, chr);
    } else {
      chr -= 0x10000;
      size_t s = raw::utf8::encode(output, 0xDC00 + (chr & 0x3FF));
      return s + raw::utf8::encode(output, 0xD800 + ((chr >> 10) & 0x3FF));
    }
  }
  template <typename It>
  static bool validate(It iterator, It end) {
    return (validate_raw(iterator, end) & (~allowed)) == 0;
  }
};

}

