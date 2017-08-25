#pragma once

struct utf32 {
  using storage_type = uint32_t;
  using char_type = char32_t;
  template <typename It>
  static size_t encode(It& output, char32_t chr);
  template <typename It>
  static char32_t decode(It iterator);
  template <typename It>
  static void walk(It& iterator, int delta);
  template <typename It>
  static bool validate(It iterator, It end);
};

template <typename It>
size_t utf32::encode(It& output, char32_t chr) {
  *output++ = chr;
  return 1;
}

template <typename It>
char32_t utf32::decode(It it) {
  return *it++;
}

template <typename It>
void utf32::walk(It& iterator, int delta) {
  int direction = (delta < 0 ? -1 : +1);
  while (delta != 0) {
    iterator += direction;
    delta -= direction;
  }
}

template <typename It>
bool utf32::validate(It iterator, It end) {
  for (; iterator != end; ++iterator) {
    if (*iterator >= 0xD800 && *iterator < 0xE000) return false;
    if (*iterator >= 0x110000) return false;
  }
  return true;
}

