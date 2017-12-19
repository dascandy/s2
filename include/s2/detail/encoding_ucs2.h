#pragma once

#include <s2/detail/encoding.h>
#include <s2/detail/raw_encoding_ucs2.h>

namespace s2::encoding {

struct ucs2 : raw::ucs2 {
  static constexpr validation_result allowed = validation::raw_zero; // | validation::unmatched_surrogate;
  template <typename It>
  static bool validate(It iterator, It end) {
    return (validate_raw(iterator, end) & (~allowed)) == 0;
  }
};

}

