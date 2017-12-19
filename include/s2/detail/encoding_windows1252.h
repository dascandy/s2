#pragma once

#include <s2/detail/encoding.h>
#include <s2/detail/raw_encoding_windows1252.h>

namespace s2::encoding {

struct windows1252 : raw::windows1252 {
  static constexpr validation_result allowed = validation::raw_zero;
  template <typename It>
  static bool validate(It iterator, It end) {
    return (validate_raw(iterator, end) & (~allowed)) == 0;
  }
};

}

