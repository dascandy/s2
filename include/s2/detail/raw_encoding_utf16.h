#pragma once

namespace s2::encoding::raw {

struct utf16 {
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
size_t utf16::encode(It& output, char32_t chr) {
  if (chr < 0x10000) {
    *output++ = chr;
    return 1;
  } else {
    chr -= 0x10000;
    *output++ = 0xD800 | ((chr >> 10) & 0x3FF);
    *output++ = 0xDC00 | ((chr >> 0) & 0x3FF);
    return 2;
  }
}

template <typename It>
char32_t utf16::decode(It it) {
  uint16_t value = *it++;
  if (value >= 0xD800 && value <= 0xE000) {
    uint16_t value2 = *it++;
    return 0x10000 + ((value & 0x3FF) << 10) + (value2 & 0x3FF);
  } else {
    return value;
  }
}

template <typename It>
void utf16::walk(It& iterator, int delta) {
  int direction = (delta < 0 ? -1 : +1);
  while (delta != 0) {
    iterator += direction;
    while ((*iterator & 0xFC00) == 0xDC00) iterator += direction;
    delta -= direction;
  }
}

template <typename It>
validation_result utf16::validate_raw(It iterator, It end) {
  validation_result r = 0;
  bool inPair = false;
  for (; iterator != end; ++iterator) {
    if (*iterator == 0) r |= validation::raw_zero;
    if (*iterator >= 0xD800 && *iterator < 0xDC00) {
      if (inPair)
        r |= validation::unmatched_surrogate;
      inPair = true;
    } else if (*iterator >= 0xDC00 && *iterator < 0xE000) {
      if (!inPair)
        r |= validation::unmatched_surrogate;
      inPair = false;
    } else {
      if (inPair) 
        r |= validation::unmatched_surrogate;
      inPair = false;
    }
  }
  if (inPair)
    r |= validation::unmatched_surrogate;

  return r;
}

}


