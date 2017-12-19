#pragma once

#include <s2/detail/encoding.h>
#include <s2/detail/raw_encoding_utf8.h>

namespace s2::encoding {

struct utf8 : raw::utf8 {
  static constexpr validation_result allowed = validation::raw_zero | validation::long_encoding;
  template <typename It>
  static bool validate(It iterator, It end) {
    return (validate_raw(iterator, end) & (~allowed)) == 0;
  }
};

}

