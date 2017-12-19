#pragma once

namespace s2::encoding {

using validation_result = uint32_t;
namespace validation {
  // Some encoding variants do these things
  static constexpr validation_result surrogate_in_non_surrogate_encoding  = 0x01;
  static constexpr validation_result overlong_zero                        = 0x02;
  static constexpr validation_result overlong_encoding                    = 0x04;
  static constexpr validation_result raw_zero                             = 0x08;
  static constexpr validation_result long_encoding                        = 0x10;

  // two unmatched surrogates may match and form a different character; causing the source text to be mangled
  // while some encodings do this; it is not always safe or reliable to do so
  static constexpr validation_result unmatched_surrogate                  = 0x20; 

  // This may be seen and should always be invalid
  static constexpr validation_result beyond_range                         = 0x40;
  static constexpr validation_result invalid_bytes                        = 0x80;
}

}


