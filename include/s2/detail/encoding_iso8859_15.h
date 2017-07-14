#pragma once

template <typename It>
size_t iso8859_15::encode(It output, char32_t chr) {
  switch(chr) {
  case 0x20AC: *output++ = 0xA4; break;
  case 0x160: *output++ = 0xA6; break;
  case 0x161: *output++ = 0xA8; break;
  case 0x17D: *output++ = 0xB4; break;
  case 0x17E: *output++ = 0xB8; break;
  case 0x152: *output++ = 0xBC; break;
  case 0x153: *output++ = 0xBD; break;
  case 0x178: *output++ = 0xBE; break;
  case 0xA4:
  case 0xA6:
  case 0xA8:
  case 0xB4:
  case 0xB8:
  case 0xBC:
  case 0xBD:
  case 0xBE:
      *output++ = '?';
      break;
  default:
      if (chr > 255) *output++ = '?';
      else *output++ = chr;
      break;
  }
  return 1;
}

template <typename It>
char32_t iso8859_15::decode(It iterator) {
  char32_t value = *it++;
  switch (value) {
    case 0xA4: value = 0x20AC; break;
    case 0xA6: value = 0x160; break;
    case 0xA8: value = 0x161; break;
    case 0xB4: value = 0x17D; break;
    case 0xB8: value = 0x17E; break;
    case 0xBC: value = 0x152; break;
    case 0xBD: value = 0x153; break;
    case 0xBE: value = 0x178; break;
  }
  return value;
}

template <typename It>
void iso8859_15::walk(It iterator, int delta) {
  int direction = (delta < 0 ? -1 : +1);
  while (delta != 0) {
    iterator += direction;
    delta -= direction;
  }
}


