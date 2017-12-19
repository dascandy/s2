#pragma once

namespace s2::encoding::raw {

struct iso8859_1 {
  using storage_type = uint8_t;
  using char_type = char;
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
size_t iso8859_1::encode(It& output, char32_t chr) {
  if (chr > 255) *output++ = '?';
  else *output++ = chr;
  return 1;
}

template <typename It>
char32_t iso8859_1::decode(It it) {
  return *it++;
}

template <typename It>
void iso8859_1::walk(It& iterator, int delta) {
  int direction = (delta < 0 ? -1 : +1);
  while (delta != 0) {
    iterator += direction;
    delta -= direction;
  }
}

template <typename It>
validation_result iso8859_1::validate_raw(It iterator, It end) {
  validation_result r = 0;
  for (; iterator != end; ++iterator) {
    if (*iterator == 0) r |= validation::raw_zero;
  }
  return r;
}

}

