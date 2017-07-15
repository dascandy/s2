#pragma once

template <typename It>
size_t utf32::encode(It& output, char32_t chr) {
  *output++ = chr;
  return 1;
}

template <typename It>
char32_t utf32::decode(It& &it) {
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


