#include <s2/string>
#include "catch.hpp"
#include <s2/detail/encoding_utf32.h>

struct entry {
  const uint32_t* p;
  size_t len;
};

#define CASE(x, t) \
        TEST_CASE("UTF32 check " #x " is valid", "[string/utf32]") \
        { const uint32_t* v = (const uint32_t*)x; \
          REQUIRE(s2::encoding::utf32::validate(v, v + sizeof(x) / sizeof(x[0]) - 1) == t); \
        }

#define VALID(x) CASE(x, true)
#define INVALID(x) CASE(x, false)

VALID(U"\x0000");
VALID(U"\x0001");
VALID(U"\x0002");
VALID(U"\x007E");
VALID(U"\x007F");
VALID(U"\x0080");
VALID(U"\xD7FF");
VALID(U"\xE000");
VALID(U"\xFEFF");
VALID(U"\xFFFD");
VALID(U"\xFFFE");
VALID(U"\xFFFF");
VALID(U"\x10000");
VALID(U"\x1FFFF");
VALID(U"\x10FFFF");
INVALID(U"\xD800");
INVALID(U"\xD800\xDC00");
INVALID(U"\xD800\xDFFF");
INVALID(U"\xDBFF");
INVALID(U"\xDBFF\xDC00");
INVALID(U"\xDBFF\xDFFF");
INVALID(U"\xDC00");
INVALID(U"\xDFFF");
INVALID(U"\x110000");

#undef VALID
#undef INVALID

