#pragma once

namespace s2::encoding::raw {

struct windows1252 {
  using storage_type = uint8_t;
  using char_type = char;
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
size_t windows1252::encode(It& output, char32_t chr) {
  switch(chr) {
  case 0x20AC: *output++ = 0x80; break;
  case 0x201A: *output++ = 0x82; break;
  case 0x0192: *output++ = 0x83; break;
  case 0x201E: *output++ = 0x84; break;
  case 0x2026: *output++ = 0x85; break;
  case 0x2020: *output++ = 0x86; break;
  case 0x2021: *output++ = 0x87; break;
  case 0x02C6: *output++ = 0x88; break;
  case 0x2030: *output++ = 0x89; break;
  case 0x0160: *output++ = 0x8A; break;
  case 0x2039: *output++ = 0x8B; break;
  case 0x0152: *output++ = 0x8C; break;
  case 0x017D: *output++ = 0x8E; break;
  case 0x2018: *output++ = 0x91; break;
  case 0x2019: *output++ = 0x92; break;
  case 0x201C: *output++ = 0x93; break;
  case 0x201D: *output++ = 0x94; break;
  case 0x2022: *output++ = 0x95; break;
  case 0x2013: *output++ = 0x96; break;
  case 0x2014: *output++ = 0x97; break;
  case 0x02DC: *output++ = 0x98; break;
  case 0x2122: *output++ = 0x99; break;
  case 0x0161: *output++ = 0x9A; break;
  case 0x203A: *output++ = 0x9B; break;
  case 0x0153: *output++ = 0x9C; break;
  case 0x017E: *output++ = 0x9E; break;
  case 0x0178: *output++ = 0x9F; break;
  case 0x80:
  case 0x82:
  case 0x83:
  case 0x84:
  case 0x85:
  case 0x86:
  case 0x87:
  case 0x88:
  case 0x89:
  case 0x8A:
  case 0x8B:
  case 0x8C:
  case 0x8E:
  case 0x91:
  case 0x92:
  case 0x93:
  case 0x94:
  case 0x95:
  case 0x96:
  case 0x97:
  case 0x98:
  case 0x99:
  case 0x9A:
  case 0x9B:
  case 0x9C:
  case 0x9E:
  case 0x9F:
    *output++ = '?'; break;
  default: 
    if (chr > 255) *output++ = '?'; 
    else *output++ = chr; break;
  }
  return 1;
}

template <typename It>
char32_t windows1252::decode(It it) {
  char32_t value = *it++;
  switch (value) {
    case 0x80: value = 0x20AC; break;
    case 0x82: value = 0x201A; break;
    case 0x83: value = 0x0192; break;
    case 0x84: value = 0x201E; break;
    case 0x85: value = 0x2026; break;
    case 0x86: value = 0x2020; break;
    case 0x87: value = 0x2021; break;
    case 0x88: value = 0x02C6; break;
    case 0x89: value = 0x2030; break;
    case 0x8A: value = 0x0160; break;
    case 0x8B: value = 0x2039; break;
    case 0x8C: value = 0x0152; break;
    case 0x8E: value = 0x017D; break;
    case 0x91: value = 0x2018; break;
    case 0x92: value = 0x2019; break;
    case 0x93: value = 0x201C; break;
    case 0x94: value = 0x201D; break;
    case 0x95: value = 0x2022; break;
    case 0x96: value = 0x2013; break;
    case 0x97: value = 0x2014; break;
    case 0x98: value = 0x02DC; break;
    case 0x99: value = 0x2122; break;
    case 0x9A: value = 0x0161; break;
    case 0x9B: value = 0x203A; break;
    case 0x9C: value = 0x0153; break;
    case 0x9E: value = 0x017E; break;
    case 0x9F: value = 0x0178; break;
  }
  return value;
}

template <typename It>
void windows1252::walk(It& iterator, int delta) {
  int direction = (delta < 0 ? -1 : +1);
  while (delta != 0) {
    iterator += direction;
    delta -= direction;
  }
}

template <typename It>
validation_result windows1252::validate_raw(It iterator, It end) {
  validation_result r = 0;
  for (; iterator != end; ++iterator) {
    if (*iterator == 0) r |= validation::raw_zero;
  }
  return r;
}

}


