#pragma once

#include <s2/detail/encoding.h>

namespace s2::encoding::raw {

struct cp437 {
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
size_t cp437::encode(It& output, char32_t chr) {
  switch(chr) {
  case 0x263A: *output++ = 0x01; break;
  case 0x263B: *output++ = 0x02; break;
  case 0x2665: *output++ = 0x03; break;
  case 0x2666: *output++ = 0x04; break;
  case 0x2663: *output++ = 0x05; break;
  case 0x2660: *output++ = 0x06; break;
  case 0x2022: *output++ = 0x07; break;
  case 0x25D8: *output++ = 0x08; break;
  case 0x25CB: *output++ = 0x09; break;
  case 0x25D9: *output++ = 0x0A; break;
  case 0x2642: *output++ = 0x0B; break;
  case 0x2640: *output++ = 0x0C; break;
  case 0x266A: *output++ = 0x0D; break;
  case 0x266B: *output++ = 0x0E; break;
  case 0x263C: *output++ = 0x0F; break;
  case 0x25BA: *output++ = 0x10; break;
  case 0x25C4: *output++ = 0x11; break;
  case 0x2195: *output++ = 0x12; break;
  case 0x203C: *output++ = 0x13; break;
  case 0x00B6: *output++ = 0x14; break;
  case 0x00A7: *output++ = 0x15; break;
  case 0x25AC: *output++ = 0x16; break;
  case 0x21A8: *output++ = 0x17; break;
  case 0x2191: *output++ = 0x18; break;
  case 0x2193: *output++ = 0x19; break;
  case 0x2192: *output++ = 0x1A; break;
  case 0x2190: *output++ = 0x1B; break;
  case 0x221F: *output++ = 0x1C; break;
  case 0x2194: *output++ = 0x1D; break;
  case 0x25B2: *output++ = 0x1E; break;
  case 0x25BC: *output++ = 0x1F; break;
  case 0x2302: *output++ = 0x7F; break;
  case 0x00C7: *output++ = 0x80; break;
  case 0x00FC: *output++ = 0x81; break;
  case 0x00E9: *output++ = 0x82; break;
  case 0x00E2: *output++ = 0x83; break;
  case 0x00E4: *output++ = 0x84; break;
  case 0x00E0: *output++ = 0x85; break;
  case 0x00E5: *output++ = 0x86; break;
  case 0x00E7: *output++ = 0x87; break;
  case 0x00EA: *output++ = 0x88; break;
  case 0x00EB: *output++ = 0x89; break;
  case 0x00E8: *output++ = 0x8A; break;
  case 0x00EF: *output++ = 0x8B; break;
  case 0x00EE: *output++ = 0x8C; break;
  case 0x00EC: *output++ = 0x8D; break;
  case 0x00C4: *output++ = 0x8E; break;
  case 0x00C5: *output++ = 0x8F; break;
  case 0x00C9: *output++ = 0x90; break;
  case 0x00E6: *output++ = 0x91; break;
  case 0x00C6: *output++ = 0x92; break;
  case 0x00F4: *output++ = 0x93; break;
  case 0x00F6: *output++ = 0x94; break;
  case 0x00F2: *output++ = 0x95; break;
  case 0x00FB: *output++ = 0x96; break;
  case 0x00F9: *output++ = 0x97; break;
  case 0x00FF: *output++ = 0x98; break;
  case 0x00D6: *output++ = 0x99; break;
  case 0x00DC: *output++ = 0x9A; break;
  case 0x00A2: *output++ = 0x9B; break;
  case 0x00A3: *output++ = 0x9C; break;
  case 0x00A5: *output++ = 0x9D; break;
  case 0x20A7: *output++ = 0x9E; break;
  case 0x0192: *output++ = 0x9F; break;
  case 0x00E1: *output++ = 0xA0; break;
  case 0x00ED: *output++ = 0xA1; break;
  case 0x00F3: *output++ = 0xA2; break;
  case 0x00FA: *output++ = 0xA3; break;
  case 0x00F1: *output++ = 0xA4; break;
  case 0x00D1: *output++ = 0xA5; break;
  case 0x00AA: *output++ = 0xA6; break;
  case 0x00BA: *output++ = 0xA7; break;
  case 0x00BF: *output++ = 0xA8; break;
  case 0x2310: *output++ = 0xA9; break;
  case 0x00AC: *output++ = 0xAA; break;
  case 0x00BD: *output++ = 0xAB; break;
  case 0x00BC: *output++ = 0xAC; break;
  case 0x00A1: *output++ = 0xAD; break;
  case 0x00AB: *output++ = 0xAE; break;
  case 0x00BB: *output++ = 0xAF; break;
  case 0x2591: *output++ = 0xB0; break;
  case 0x2592: *output++ = 0xB1; break;
  case 0x2593: *output++ = 0xB2; break;
  case 0x2502: *output++ = 0xB3; break;
  case 0x2524: *output++ = 0xB4; break;
  case 0x2561: *output++ = 0xB5; break;
  case 0x2562: *output++ = 0xB6; break;
  case 0x2556: *output++ = 0xB7; break;
  case 0x2555: *output++ = 0xB8; break;
  case 0x2563: *output++ = 0xB9; break;
  case 0x2551: *output++ = 0xBA; break;
  case 0x2557: *output++ = 0xBB; break;
  case 0x255D: *output++ = 0xBC; break;
  case 0x255C: *output++ = 0xBD; break;
  case 0x255B: *output++ = 0xBE; break;
  case 0x2510: *output++ = 0xBF; break;
  case 0x2514: *output++ = 0xC0; break;
  case 0x2534: *output++ = 0xC1; break;
  case 0x252C: *output++ = 0xC2; break;
  case 0x251C: *output++ = 0xC3; break;
  case 0x2500: *output++ = 0xC4; break;
  case 0x253C: *output++ = 0xC5; break;
  case 0x255E: *output++ = 0xC6; break;
  case 0x255F: *output++ = 0xC7; break;
  case 0x255A: *output++ = 0xC8; break;
  case 0x2554: *output++ = 0xC9; break;
  case 0x2569: *output++ = 0xCA; break;
  case 0x2566: *output++ = 0xCB; break;
  case 0x2560: *output++ = 0xCC; break;
  case 0x2550: *output++ = 0xCD; break;
  case 0x256C: *output++ = 0xCE; break;
  case 0x2567: *output++ = 0xCF; break;
  case 0x2568: *output++ = 0xD0; break;
  case 0x2564: *output++ = 0xD1; break;
  case 0x2565: *output++ = 0xD2; break;
  case 0x2559: *output++ = 0xD3; break;
  case 0x2558: *output++ = 0xD4; break;
  case 0x2552: *output++ = 0xD5; break;
  case 0x2553: *output++ = 0xD6; break;
  case 0x256B: *output++ = 0xD7; break;
  case 0x256A: *output++ = 0xD8; break;
  case 0x2518: *output++ = 0xD9; break;
  case 0x250C: *output++ = 0xDA; break;
  case 0x2588: *output++ = 0xDB; break;
  case 0x2584: *output++ = 0xDC; break;
  case 0x258C: *output++ = 0xDD; break;
  case 0x2590: *output++ = 0xDE; break;
  case 0x2580: *output++ = 0xDF; break;
  case 0x03B1: *output++ = 0xE0; break;
  case 0x00DF: *output++ = 0xE1; break;
  case 0x0393: *output++ = 0xE2; break;
  case 0x03C0: *output++ = 0xE3; break;
  case 0x03A3: *output++ = 0xE4; break;
  case 0x03C3: *output++ = 0xE5; break;
  case 0x00B5: *output++ = 0xE6; break;
  case 0x03C4: *output++ = 0xE7; break;
  case 0x03A6: *output++ = 0xE8; break;
  case 0x0398: *output++ = 0xE9; break;
  case 0x03A9: *output++ = 0xEA; break;
  case 0x03B4: *output++ = 0xEB; break;
  case 0x221E: *output++ = 0xEC; break;
  case 0x03C6: *output++ = 0xED; break;
  case 0x03B5: *output++ = 0xEE; break;
  case 0x2229: *output++ = 0xEF; break;
  case 0x2261: *output++ = 0xF0; break;
  case 0x00B1: *output++ = 0xF1; break;
  case 0x2265: *output++ = 0xF2; break;
  case 0x2264: *output++ = 0xF3; break;
  case 0x2320: *output++ = 0xF4; break;
  case 0x2321: *output++ = 0xF5; break;
  case 0x00F7: *output++ = 0xF6; break;
  case 0x2248: *output++ = 0xF7; break;
  case 0x00B0: *output++ = 0xF8; break;
  case 0x2219: *output++ = 0xF9; break;
  case 0x00B7: *output++ = 0xFA; break;
  case 0x221A: *output++ = 0xFB; break;
  case 0x207F: *output++ = 0xFC; break;
  case 0x00B2: *output++ = 0xFD; break;
  case 0x25A0: *output++ = 0xFE; break;
  case 0x00A0: *output++ = 0xFF; break;

  case 0x7F:
  case 0x80:
  case 0x81:
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
  case 0x8D:
  case 0x8E:
  case 0x8F:
  case 0x90:
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
  case 0x9D:
  case 0x9E:
  case 0x9F:
  case 0xA4:
  case 0xA6:
  case 0xA8:
  case 0xA9:
  case 0xAD:
  case 0xAE:
  case 0xAF:
  case 0xB3:
  case 0xB4:
  case 0xB8:
  case 0xB9:
  case 0xBE:
  case 0xC0:
  case 0xC1:
  case 0xC2:
  case 0xC3:
  case 0xC8:
  case 0xCA:
  case 0xCB:
  case 0xCC:
  case 0xCD:
  case 0xCE:
  case 0xCF:
  case 0xD0:
  case 0xD2:
  case 0xD3:
  case 0xD4:
  case 0xD5:
  case 0xD7:
  case 0xD8:
  case 0xD9:
  case 0xDA:
  case 0xDB:
  case 0xDD:
  case 0xDE:
  case 0xE3:
  case 0xF0:
  case 0xF5:
  case 0xF8:
  case 0xFD:
  case 0xFE:
    *output++ = '?'; break;
  default: 
    if (chr > 255) *output++ = '?'; 
    else *output++ = chr; break;
  }
  return 1;
}

template <typename It>
char32_t cp437::decode(It it) {
  char32_t value = *it++;
  switch (value) {
  case 0x01: value = 0x263A; break;
  case 0x02: value = 0x263B; break;
  case 0x03: value = 0x2665; break;
  case 0x04: value = 0x2666; break;
  case 0x05: value = 0x2663; break;
  case 0x06: value = 0x2660; break;
  case 0x07: value = 0x2022; break;
  case 0x08: value = 0x25D8; break;
  case 0x09: value = 0x25CB; break;
  case 0x0A: value = 0x25D9; break;
  case 0x0B: value = 0x2642; break;
  case 0x0C: value = 0x2640; break;
  case 0x0D: value = 0x266A; break;
  case 0x0E: value = 0x266B; break;
  case 0x0F: value = 0x263C; break;
  case 0x10: value = 0x25BA; break;
  case 0x11: value = 0x25C4; break;
  case 0x12: value = 0x2195; break;
  case 0x13: value = 0x203C; break;
  case 0x14: value = 0x00B6; break;
  case 0x15: value = 0x00A7; break;
  case 0x16: value = 0x25AC; break;
  case 0x17: value = 0x21A8; break;
  case 0x18: value = 0x2191; break;
  case 0x19: value = 0x2193; break;
  case 0x1A: value = 0x2192; break;
  case 0x1B: value = 0x2190; break;
  case 0x1C: value = 0x221F; break;
  case 0x1D: value = 0x2194; break;
  case 0x1E: value = 0x25B2; break;
  case 0x1F: value = 0x25BC; break;
  case 0x7F: value = 0x2302; break;
  case 0x80: value = 0x00C7; break;
  case 0x81: value = 0x00FC; break;
  case 0x82: value = 0x00E9; break;
  case 0x83: value = 0x00E2; break;
  case 0x84: value = 0x00E4; break;
  case 0x85: value = 0x00E0; break;
  case 0x86: value = 0x00E5; break;
  case 0x87: value = 0x00E7; break;
  case 0x88: value = 0x00EA; break;
  case 0x89: value = 0x00EB; break;
  case 0x8A: value = 0x00E8; break;
  case 0x8B: value = 0x00EF; break;
  case 0x8C: value = 0x00EE; break;
  case 0x8D: value = 0x00EC; break;
  case 0x8E: value = 0x00C4; break;
  case 0x8F: value = 0x00C5; break;
  case 0x90: value = 0x00C9; break;
  case 0x91: value = 0x00E6; break;
  case 0x92: value = 0x00C6; break;
  case 0x93: value = 0x00F4; break;
  case 0x94: value = 0x00F6; break;
  case 0x95: value = 0x00F2; break;
  case 0x96: value = 0x00FB; break;
  case 0x97: value = 0x00F9; break;
  case 0x98: value = 0x00FF; break;
  case 0x99: value = 0x00D6; break;
  case 0x9A: value = 0x00DC; break;
  case 0x9B: value = 0x00A2; break;
  case 0x9C: value = 0x00A3; break;
  case 0x9D: value = 0x00A5; break;
  case 0x9E: value = 0x20A7; break;
  case 0x9F: value = 0x0192; break;
  case 0xA0: value = 0x00E1; break;
  case 0xA1: value = 0x00ED; break;
  case 0xA2: value = 0x00F3; break;
  case 0xA3: value = 0x00FA; break;
  case 0xA4: value = 0x00F1; break;
  case 0xA5: value = 0x00D1; break;
  case 0xA6: value = 0x00AA; break;
  case 0xA7: value = 0x00BA; break;
  case 0xA8: value = 0x00BF; break;
  case 0xA9: value = 0x2310; break;
  case 0xAA: value = 0x00AC; break;
  case 0xAB: value = 0x00BD; break;
  case 0xAC: value = 0x00BC; break;
  case 0xAD: value = 0x00A1; break;
  case 0xAE: value = 0x00AB; break;
  case 0xAF: value = 0x00BB; break;
  case 0xB0: value = 0x2591; break;
  case 0xB1: value = 0x2592; break;
  case 0xB2: value = 0x2593; break;
  case 0xB3: value = 0x2502; break;
  case 0xB4: value = 0x2524; break;
  case 0xB5: value = 0x2561; break;
  case 0xB6: value = 0x2562; break;
  case 0xB7: value = 0x2556; break;
  case 0xB8: value = 0x2555; break;
  case 0xB9: value = 0x2563; break;
  case 0xBA: value = 0x2551; break;
  case 0xBB: value = 0x2557; break;
  case 0xBC: value = 0x255D; break;
  case 0xBD: value = 0x255C; break;
  case 0xBE: value = 0x255B; break;
  case 0xBF: value = 0x2510; break;
  case 0xC0: value = 0x2514; break;
  case 0xC1: value = 0x2534; break;
  case 0xC2: value = 0x252C; break;
  case 0xC3: value = 0x251C; break;
  case 0xC4: value = 0x2500; break;
  case 0xC5: value = 0x253C; break;
  case 0xC6: value = 0x255E; break;
  case 0xC7: value = 0x255F; break;
  case 0xC8: value = 0x255A; break;
  case 0xC9: value = 0x2554; break;
  case 0xCA: value = 0x2569; break;
  case 0xCB: value = 0x2566; break;
  case 0xCC: value = 0x2560; break;
  case 0xCD: value = 0x2550; break;
  case 0xCE: value = 0x256C; break;
  case 0xCF: value = 0x2567; break;
  case 0xD0: value = 0x2568; break;
  case 0xD1: value = 0x2564; break;
  case 0xD2: value = 0x2565; break;
  case 0xD3: value = 0x2559; break;
  case 0xD4: value = 0x2558; break;
  case 0xD5: value = 0x2552; break;
  case 0xD6: value = 0x2553; break;
  case 0xD7: value = 0x256B; break;
  case 0xD8: value = 0x256A; break;
  case 0xD9: value = 0x2518; break;
  case 0xDA: value = 0x250C; break;
  case 0xDB: value = 0x2588; break;
  case 0xDC: value = 0x2584; break;
  case 0xDD: value = 0x258C; break;
  case 0xDE: value = 0x2590; break;
  case 0xDF: value = 0x2580; break;
  case 0xE0: value = 0x03B1; break;
  case 0xE1: value = 0x00DF; break;
  case 0xE2: value = 0x0393; break;
  case 0xE3: value = 0x03C0; break;
  case 0xE4: value = 0x03A3; break;
  case 0xE5: value = 0x03C3; break;
  case 0xE6: value = 0x00B5; break;
  case 0xE7: value = 0x03C4; break;
  case 0xE8: value = 0x03A6; break;
  case 0xE9: value = 0x0398; break;
  case 0xEA: value = 0x03A9; break;
  case 0xEB: value = 0x03B4; break;
  case 0xEC: value = 0x221E; break;
  case 0xED: value = 0x03C6; break;
  case 0xEE: value = 0x03B5; break;
  case 0xEF: value = 0x2229; break;
  case 0xF0: value = 0x2261; break;
  case 0xF1: value = 0x00B1; break;
  case 0xF2: value = 0x2265; break;
  case 0xF3: value = 0x2264; break;
  case 0xF4: value = 0x2320; break;
  case 0xF5: value = 0x2321; break;
  case 0xF6: value = 0x00F7; break;
  case 0xF7: value = 0x2248; break;
  case 0xF8: value = 0x00B0; break;
  case 0xF9: value = 0x2219; break;
  case 0xFA: value = 0x00B7; break;
  case 0xFB: value = 0x221A; break;
  case 0xFC: value = 0x207F; break;
  case 0xFD: value = 0x00B2; break;
  case 0xFE: value = 0x25A0; break;
  case 0xFF: value = 0x00A0; break;
  }
  return value;
}

template <typename It>
void cp437::walk(It& iterator, int delta) {
  int direction = (delta < 0 ? -1 : +1);
  while (delta != 0) {
    iterator += direction;
    delta -= direction;
  }
}

template <typename It>
validation_result cp437::validate_raw(It iterator, It end) {
  validation_result r = 0;
  for (; iterator != end; ++iterator) {
    if (*iterator == 0) r |= validation::raw_zero;
  }
  return r;
}

}


