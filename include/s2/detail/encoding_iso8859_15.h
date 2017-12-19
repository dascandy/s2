#pragma once

#include <s2/detail/encoding.h>
#include <s2/detail/raw_encoding_iso8859_15.h>

namespace s2::encoding {

struct iso8859_15 : raw::iso8859_15 {
  static constexpr validation_result allowed = validation::raw_zero;
  template <typename It>
  static bool validate(It iterator, It end) {
    return (validate_raw(iterator, end) & (~allowed)) == 0;
  }
};

}

