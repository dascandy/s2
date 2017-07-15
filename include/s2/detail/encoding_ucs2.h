#pragma once

template <typename It>
size_t ucs2::encode(It& output, char32_t chr) {
  *output++ = (chr < 0x10000) ? chr : 0xFFFD;
  return 1;
}

template <typename It>
char32_t ucs2::decode(It& &it) {
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


