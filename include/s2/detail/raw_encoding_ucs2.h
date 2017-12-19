#pragma once

namespace s2::encoding::raw {

struct ucs2 {
  using storage_type = uint16_t;
  using char_type = char16_t;
  template <typename It>
  static size_t encode(It& output, char32_t chr);
  template <typename It>
  static char32_t decode(It iterator);
  template <typename It>
  static void walk(It& iterator, int delta);
  template <typename It>
  static validation_result validate_raw(It iterator, It end);
};

template <typename It>
size_t ucs2::encode(It& output, char32_t chr) {
  *output++ = (chr < 0x10000) ? chr : 0xFFFD;
  return 1;
}

template <typename It>
char32_t ucs2::decode(It it) {
  return *it++;
}

template <typename It>
void ucs2::walk(It& iterator, int delta) {
  int direction = (delta < 0 ? -1 : +1);
  while (delta != 0) {
    iterator += direction;
    delta -= direction;
  }
}

template <typename It>
validation_result ucs2::validate_raw(It iterator, It end) {
  validation_result r = 0;
  for (; iterator != end; ++iterator) {
    if (*iterator >= 0xD800 && *iterator < 0xE000) r |= validation::unmatched_surrogate;
    if (*iterator == 0) r |= validation::raw_zero;
  }
  return r;
}

}


