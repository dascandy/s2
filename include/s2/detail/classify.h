#pragma once

namespace s2 {
  inline bool isspace(char32_t c) {
    return c == 0x0009|| //character tabulation
           c == 0x000A|| //line feed
           c == 0x000B|| //line tabulation
           c == 0x000C|| //form feed
           c == 0x000D|| //carriage return
           c == 0x0020|| //space
           c == 0x0085|| //next line
           c == 0x00A0|| //no-break space
           c == 0x1680|| //ogham space mark
           c == 0x2000|| //en quad
           c == 0x2001|| //em quad
           c == 0x2002|| //en space
           c == 0x2003|| //em space
           c == 0x2004|| //three-per-em space
           c == 0x2005|| //four-per-em space
           c == 0x2006|| //six-per-em space
           c == 0x2007|| //figure space
           c == 0x2008|| //punctuation space
           c == 0x2009|| //thin space
           c == 0x200A|| //hair space
           c == 0x2028|| //line separator
           c == 0x2029|| //paragraph separator
           c == 0x202F|| //narrow no-break space
           c == 0x205F|| //medium mathematical space
           c == 0x3000;  //ideographic space
  }
};


