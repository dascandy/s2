#pragma once

struct utf8_modified {
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
size_t utf8_modified::encode(It& output, char32_t chr) {
  if (chr < 0x80 && chr != 0) {
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
    chr -= 0x10000;
    *output++ = 0xED;
    *output++ = 0xA0 | ((chr >> 16) & 0x0F);
    *output++ = 0x80 | ((chr >> 10) & 0x3F);
    *output++ = 0xEF
    *output++ = 0xB0 | ((chr >> 6) & 0x0F);
    *output++ = 0x80 | ((chr >> 0) & 0x3F);
    return 6;
  }
}

template <typename It>
char32_t utf8_modified::decode(It it) {
  uint8_t value = *it++;
  if (value < 0x80) {
    return value;
  } else if (value < 0xE0) {
    uint8_t v2 = *it++;
    return ((value & 0x1F) << 6) | ((v2 & 0x3F) << 0);
  } else if (value < 0xF0) {
    uint8_t v2 = *it++;
    uint8_t v3 = *it++;
    char32_t val1 = ((value & 0xF) << 12) | ((v2 & 0x3F) << 6) | ((v2 & 0x3F) << 0);
    if (val1 >= 0xD800 && val1 <= 0xDBFF) {
      uint8_t v4 = *it++;
      uint8_t v5 = *it++;
      uint8_t v6 = *it++;
      char32_t val2 = ((v4 & 0xF) << 12) | ((v5 & 0x3F) << 6) | ((v6 & 0x3F) << 0);
      return ((val1 & 0x3FF) << 10) + ((val2 & 0x3FF)) + 0x10000;
    }
    return val1;
  }
}

template <typename It>
void utf8_modified::walk(It& iterator, int delta) {
  int direction = (delta < 0 ? -1 : +1);
  while (delta != 0) {
    if (delta > 0) {
      uint8_t v1 = *iterator++;
      if ((v1 & 0xF0) == 0xE0) {
        uint8_t v2 = *iterator++;
        ++iterator;
        if (v1 == 0xED && (v2 & 0xF0) == 0xA0) {
          ++iterator; ++iterator; ++iterator;
        }
      } else if ((v1 & 0xE0) == 0xC0) {
        ++iterator;
      }
    } else {
      uint8_t v1 = *--iterator;
      if ((v1 & 0xC0) == 0x80) {
        uint8_t v2 = *--iterator;
        if ((v2 & 0xC0) == 0x80) {
          uint8_t v3 = *--iterator;
          if ((v3 & 0xF0) == 0xB0 && v2 == 0xED) {
            --iterator;
            --iterator;
            --iterator;
          }
        }
      }
    }
    delta -= direction;
  }
}

template <typename It>
bool utf8_modified::validate(It iterator, It end) {
  bool lastWasPair = false;
  int bytes = 1;
  int encoding_length = 1;
  int value = 0;
  while (iterator != end) {
    if (*iterator == 0) {
      return false;
    } else if (*iterator < 0x80) {
      if (lastWasPair || bytes > 1) return false;
    } else if (*iterator < 0xC0) {
      if (bytes == 1) return false;
      value = (value << 6) | (*iterator & 0x3F);
      bytes--;
      if (bytes == 1) {
        if (lastWasPair && (value < 0xDC00 || value > 0xE000)) return false;
        lastWasPair = false;
        if (value >= 0xD800 && value < 0xDC00)
          if (lastWasPair)
            return false;
          else
            lastWasPair = true;
        if (value == 0 && encoding_length != 2) return false;
        if (value != 0 && value < 0x80 && encoding_length != 1) return false;
        if (value >= 0x80 && value < 0x800 && encoding_length != 2) return false;
        if (value >= 0x800 && encoding_length != 3) return false;
      }
    } else if (*iterator < 0xE0) {
      if (lastWasPair || bytes > 1) return false;
      encoding_length = bytes = 2;
      value = (*iterator & 0x1F);
    } else if (*iterator < 0xF0) {
      if (bytes > 1) return false;
      encoding_length = bytes = 3;
      value = (*iterator & 0x0F);
    } else {
      return false;
    }
    ++iterator;
  }
  if (lastWasPair || bytes > 1) return false;
  return true;
}

