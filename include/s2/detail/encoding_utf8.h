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

template <typename It>
bool utf8::validate(It iterator, It end) {
  int bytes = 1;
  int encoding_length = 1;
  int value = 0;
  while (iterator != end) {
    if (*iterator < 0x80) {
      if (bytes > 1) return false;
    } else if (*iterator < 0xC0) {
      if (bytes == 1) return false;
      value = (value << 6) | (*iterator & 0x3F);
      bytes--;
      if (bytes == 1) {
        if (value >= 0xD800 && value < 0xE000) return false;
        if (value > 0x110000) return false;
        if (value < 0x80 && encoding_length != 1) return false;
        if (value >= 0x80 && value < 0x800 && encoding_length != 2) return false;
        if (value >= 0x800 && value < 0x10000 && encoding_length != 3) return false;
        if (value >= 0x10000 && value < 0x110000 && encoding_length != 4) return false;
      }
    } else if (*iterator < 0xE0) {
      if (bytes > 1) return false;
      encoding_length = bytes = 2;
      value = (*iterator & 0x1F);
    } else if (*iterator < 0xF0) {
      if (bytes > 1) return false;
      encoding_length = bytes = 3;
      value = (*iterator & 0x0F);
    } else if (*iterator < 0xF8) {
      if (bytes > 1) return false;
      encoding_length = bytes = 4;
      value = (*iterator & 0x07);
    } else {
      return false;
    }
    ++iterator;
  }
  if (bytes > 0) return false;
  return true;
}


