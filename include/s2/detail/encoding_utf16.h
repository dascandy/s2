#pragma once

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
char32_t utf16::decode(It& &it) {
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


