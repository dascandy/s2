#pragma once

template <typename It>
size_t utf8::encode(It& output, char32_t chr) {
  if (chr < 0x80) {
    *output++ = chr;
    return 1;
  } else if (chr < 0x800) {
    *output++ = 0xC0 | ((chr >> 6) & 0x1F);
    *output++ = 0x80 | ((chr >> 0) & 0x3F);
    return 2;
  } else if (chr < 0x10000) {
    *output++ = 0xE0 | ((chr >> 12) & 0x0F);
    *output++ = 0x80 | ((chr >> 6) & 0x3F);
    *output++ = 0x80 | ((chr >> 0) & 0x3F);
    return 3;
  } else {
    *output++ = 0xF0 | ((chr >> 18) & 0x07);
    *output++ = 0x80 | ((chr >> 12) & 0x3F);
    *output++ = 0x80 | ((chr >> 6) & 0x3F);
    *output++ = 0x80 | ((chr >> 0) & 0x3F);
    return 4;
  }
}

template <typename It>
char32_t utf8::decode(It& &it) {
  uint8_t value = *it++;
  if (value < 0x80) {
    return value;
  } else if (value < 0xE0) {
    uint8_t v2 = *it++;
    return ((value & 0x1F) << 6) | ((v2 & 0x3F) << 0);
  } else if (value < 0xF0) {
    uint8_t v2 = *it++;
    uint8_t v3 = *it++;
    return ((value & 0xF) << 12) | ((v2 & 0x3F) << 6) | ((v2 & 0x3F) << 0);
  } else {
    uint8_t v2 = *it++;
    uint8_t v3 = *it++;
    uint8_t v4 = *it++;
    return ((value & 0x7) << 18) | ((v2 & 0x3F) << 12) | ((v3 & 0x3F) << 6) | ((v4 & 0x3F) << 0);
  }
}

template <typename It>
void utf8::walk(It& iterator, int delta) {
  int direction = (delta < 0 ? -1 : +1);
  while (delta != 0) {
    iterator += direction;
    while ((*iterator & 0xC0) == 0x80) iterator += direction;
    delta -= direction;
  }
}


