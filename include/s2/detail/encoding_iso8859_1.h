#pragma once

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
  static bool validate(It iterator, It end);
};

template <typename It>
size_t iso8859_1::encode(It& output, char32_t chr) {
  if (chr > 255) *output++ = '?';
  else *output++ = chr;
  return 1;
}

template <typename It>
char32_t iso8859_1::decode(It iterator) {
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
bool iso8859_1::validate(It iterator, It end) {
  return true;
}

