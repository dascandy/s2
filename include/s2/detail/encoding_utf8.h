#pragma once

template <typename It>
size_t utf8::encode(It output, char32_t chr) {
  
}

template <typename It>
char32_t utf8::decode(It iterator) {
  
}

template <typename It>
void utf8::walk(It iterator, int delta) {
  int direction = (delta < 0 ? -1 : +1);
  while (delta != 0) {
    iterator += direction;
    while ((*iterator & 0xC0) == 0x80) iterator += direction;
    delta -= direction;
  }
}


