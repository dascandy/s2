#pragma once

template <typename It>
size_t iso8859_1::encode(It output, char32_t chr) {
  if (chr > 255) *output++ = '?';
  else *output++ = chr;
  return 1;
}

template <typename It>
char32_t iso8859_1::decode(It iterator) {
  return *it++;
}

template <typename It>
void iso8859_1::walk(It iterator, int delta) {
  int direction = (delta < 0 ? -1 : +1);
  while (delta != 0) {
    iterator += direction;
    delta -= direction;
  }
}


