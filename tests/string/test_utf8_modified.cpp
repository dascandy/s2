#include <s2/string>
#include <s2/detail/encoding_utf8_modified.h>
#include "catch.hpp"

struct entry {
  const uint8_t* p;
  size_t len;
};

#define VALID(x) do { const uint8_t* v = (const uint8_t*)x; REQUIRE(s2::encoding::utf8_modified::validate(v, v + sizeof(x)-1) == true); } while(0)
#define INVALID(x) do { const uint8_t* v = (const uint8_t*)x; REQUIRE(s2::encoding::utf8_modified::validate(v, v + sizeof(x)-1) == false); } while(0)

TEST_CASE("Test modified utf8 validation", "[string/utf8_modified]") {
  VALID("\xC0\x80"); // 0x00 in overlong per spec
  VALID("\x01");
  VALID("\x02");
  VALID("\x7E");
  VALID("\x7F"); // 0x7F
  VALID("\xC2\x80"); // 0x80
  VALID("\xDF\xBF"); // 0x7FF
  VALID("\xE0\xA0\x80"); // 0x800
  VALID("\xED\x9F\xBF"); // 0xD7FF
  VALID("\xEE\x80\x80"); // 0xE000
  VALID("\xEF\xBF\xBF"); // 0xFFFF
  VALID("\xED\xA0\x80\xED\xB0\x80"); // 0x10000
  VALID("\xED\xAF\xBF\xED\xBF\xBF"); // 0x10FFFF

  INVALID("\x00");    // No raw 0 bytes in modified utf8
  INVALID("\xDE");    // Missing first continuation byte
  INVALID("\xDF");    // Missing first continuation byte
  INVALID("\xE0");    // Missing first continuation byte
  INVALID("\xEF");    // Missing first continuation byte
  INVALID("\xF0");    // Missing first continuation byte
  INVALID("\xF4");    // Missing first continuation byte

  INVALID("\xE0\x80"); // Missing second continuation byte
  INVALID("\xE0\xBF"); // Missing second continuation byte
  INVALID("\xE1\x80"); // Missing second continuation byte
  
  INVALID("\xC2\x00"); // Not followed by continuation byte
  INVALID("\xDF\x00"); // Not followed by continuation byte
  INVALID("\xDF\xC0"); // Not followed by continuation byte

  INVALID("\xE0\x80\x00"); // Not followed by continuation byte
  INVALID("\xE0\xBF\x00"); // Not followed by continuation byte
  INVALID("\xE1\x80\x00"); // Not followed by continuation byte

  INVALID("\xC2\x80\x80"); // Excess continuation byte
  INVALID("\xDF\x80\x80"); // Excess continuation byte
  INVALID("\xDF\xBF\x80"); // Excess continuation byte

  INVALID("\xE0\x80\x80\x80"); // Excess continuation byte
  INVALID("\xE0\xBF\x80\x80"); // Excess continuation byte
  INVALID("\xE1\x80\x80\x80"); // Excess continuation byte


  INVALID("\x80");    // Invalid start byte
  INVALID("\x81");    // Invalid start byte
  INVALID("\xBC");    // Invalid start byte
  INVALID("\xBF");    // Invalid start byte

  INVALID("\xC0\x81"); // overlong encoding of 0x01
  INVALID("\xE0\x80\x80"); // overlong encoding of 0x00
  INVALID("\xC1\xBF"); // overlong encoding of 0x7F
  INVALID("\xE0\x9F\xBF"); // overlong encoding of 0x7FF

  INVALID("\xED\xA0\x80"); // Invalid loose surrogate half 0xD800
  INVALID("\xED\xAF\xBF"); // Invalid loose surrogate half 0xDBFF
  INVALID("\xED\xB0\x80"); // Invalid loose surrogate half 0xDC00
  INVALID("\xED\xBF\xBF"); // Invalid loose surrogate half 0xDFFF

  // Modified UTF8 disallows 4-byte encodings
  INVALID("\xF0\x80"); // Missing second continuation byte
  INVALID("\xF0\x80\x00"); // Not followed by continuation byte
  INVALID("\xF0\x80\x80"); // Missing third continuation byte
  INVALID("\xF0\x80\x80\x00"); // Not followed by continuation byte
  INVALID("\xF0\x80\x80\x80"); // overlong encoding of 0x00
  INVALID("\xF0\x80\x80\x80\x80"); // Excess continuation byte
  INVALID("\xF0\x8F\xBF\xBF"); // overlong encoding of 0xFFFF
  INVALID("\xF0\x90\x80\x80"); // 0x10000
  INVALID("\xF4\x80"); // Missing second continuation byte
  INVALID("\xF4\x80\x00"); // Not followed by continuation byte
  INVALID("\xF4\x80\x80"); // Missing third continuation byte
  INVALID("\xF4\x80\x80\x00"); // Not followed by continuation byte
  INVALID("\xF4\x80\x80\x80\x80"); // Excess continuation byte
  INVALID("\xF4\x8F\xBF\xBF"); // 0x10FFFF
  INVALID("\xF4\x90\x80\x80"); // 0x110000, outside of unicode range
  INVALID("\xF5\x80\x80\x80"); // Invalid start byte
  INVALID("\xF6\x80\x80\x80"); // Invalid start byte
  INVALID("\xF7\x80\x80\x80"); // Invalid start byte
  INVALID("\xF8\x80\x80\x80\x80"); // Invalid start byte
  INVALID("\xF9\x80\x80\x80\x80"); // Invalid start byte
  INVALID("\xFA\x80\x80\x80\x80"); // Invalid start byte
  INVALID("\xFB\x80\x80\x80\x80"); // Invalid start byte
  INVALID("\xFC\x80\x80\x80\x80\x80"); // Invalid start byte
  INVALID("\xFD\x80\x80\x80\x80\x80"); // Invalid start byte
  INVALID("\xFE\x80\x80\x80\x80\x80\x80"); // Invalid start byte
  INVALID("\xFF\x80\x80\x80\x80\x80\x80\x80"); // Invalid start byte
}
#undef VALID
#undef INVALID

