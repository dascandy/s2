#include <s2/string>
#include "catch.hpp"

struct entry {
  const uint8_t* p;
  size_t len;
};

#define VALID(x) REQUIRE(s2::encoding::utf8::validate((const uint8_t*)x, (const uint8_t*)x + sizeof(x)) == true)
#define INVALID(x) REQUIRE(s2::encoding::utf8::validate((const uint8_t*)x, (const uint8_t*)x + sizeof(x)) == false)

TEST_CASE("Test utf8 validation", "[string/utf8]") {
  VALID("\x00");
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
  VALID("\xF0\x90\x80\x80"); // 0x10000
  VALID("\xF4\x8F\xBF\xBF"); // 0x10FFFF

  INVALID("\xDE");    // Missing first continuation byte
  INVALID("\xDF");    // Missing first continuation byte
  INVALID("\xE0");    // Missing first continuation byte
  INVALID("\xEF");    // Missing first continuation byte
  INVALID("\xF0");    // Missing first continuation byte
  INVALID("\xF4");    // Missing first continuation byte

  INVALID("\xE0\x80"); // Missing second continuation byte
  INVALID("\xE0\xBF"); // Missing second continuation byte
  INVALID("\xE1\x80"); // Missing second continuation byte
  INVALID("\xF0\x80"); // Missing second continuation byte
  INVALID("\xF4\x80"); // Missing second continuation byte

  INVALID("\xF0\x80\x80"); // Missing third continuation byte
  INVALID("\xF4\x80\x80"); // Missing third continuation byte
  
  INVALID("\xC2\x00"); // Not followed by continuation byte
  INVALID("\xDF\x00"); // Not followed by continuation byte
  INVALID("\xDF\xC0"); // Not followed by continuation byte

  INVALID("\xE0\x80\x00"); // Not followed by continuation byte
  INVALID("\xE0\xBF\x00"); // Not followed by continuation byte
  INVALID("\xE1\x80\x00"); // Not followed by continuation byte
  INVALID("\xF0\x80\x00"); // Not followed by continuation byte
  INVALID("\xF4\x80\x00"); // Not followed by continuation byte

  INVALID("\xF0\x80\x80\x00"); // Not followed by continuation byte
  INVALID("\xF4\x80\x80\x00"); // Not followed by continuation byte

  INVALID("\xC2\x80\x80"); // Excess continuation byte
  INVALID("\xDF\x80\x80"); // Excess continuation byte
  INVALID("\xDF\xBF\x80"); // Excess continuation byte

  INVALID("\xE0\x80\x80\x80"); // Excess continuation byte
  INVALID("\xE0\xBF\x80\x80"); // Excess continuation byte
  INVALID("\xE1\x80\x80\x80"); // Excess continuation byte

  INVALID("\xF0\x80\x80\x80\x80"); // Excess continuation byte
  INVALID("\xF4\x80\x80\x80\x80"); // Excess continuation byte

  INVALID("\x80");    // Invalid start byte
  INVALID("\x81");    // Invalid start byte
  INVALID("\xBC");    // Invalid start byte
  INVALID("\xBF");    // Invalid start byte
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

  INVALID("\xC0\x80"); // overlong encoding of 0x00
  INVALID("\xE0\x80\x80"); // overlong encoding of 0x00
  INVALID("\xF0\x80\x80\x80"); // overlong encoding of 0x00
  INVALID("\xC1\xBF"); // overlong encoding of 0x7F
  INVALID("\xE0\x9F\xBF"); // overlong encoding of 0x7FF
  INVALID("\xF0\x8F\xBF\xBF"); // overlong encoding of 0xFFFF

  INVALID("\xED\xA0\x80"); // Invalid surrogate half 0xD800
  INVALID("\xED\xAF\xBF"); // Invalid surrogate half 0xDBFF
  INVALID("\xED\xB0\x80"); // Invalid surrogate half 0xDC00
  INVALID("\xED\xBF\xBF"); // Invalid surrogate half 0xDFFF

  INVALID("\xF4\x90\x80\x80"); // 0x110000, outside of unicode range
}
#undef VALID
#undef INVALID

