#pragma once

#include <s2/detail/encoding.h>
#include <s2/detail/raw_encoding_utf32.h>

namespace s2::encoding {

struct utf32 : raw::utf32 {
  static constexpr validation_result allowed = validation::raw_zero;
  template <typename It>
  static bool validate(It iterator, It end) {
    return (raw::utf32::validate_raw(iterator, end) & (~allowed)) == 0;
  }
};

}

