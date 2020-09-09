#include <s2/string.h>
#include "catch.hpp"
#include <s2/detail/encoding_utf16.h>

struct entry {
  const uint16_t* p;
  size_t len;
};

#define CASE(x, t) \
        TEST_CASE("UTF16 check " #x " is valid", "[string/utf16]") \
        { const uint16_t* v = (const uint16_t*)x; \
          REQUIRE(s2::encoding::utf16::validate(v, v + sizeof(x) / sizeof(x[0]) - 1) == t); \
        }

#define VALID(x) CASE(x, true)
#define INVALID(x) CASE(x, false)

VALID(u"\x0000");
VALID(u"\x0001");
VALID(u"\x0002");
VALID(u"\x007E");
VALID(u"\x007F");
VALID(u"\x0080");
VALID(u"\xD7FF");
VALID(u"\xD800\xDC00");
VALID(u"\xD800\xDFFF");
VALID(u"\xDBFF\xDC00");
VALID(u"\xDBFF\xDFFF");
VALID(u"\xE000");
VALID(u"\xFEFF");
VALID(u"\xFFFD");
VALID(u"\xFFFE");
VALID(u"\xFFFF");
INVALID(u"\xD800\x0000");
INVALID(u"\xD800\xDA00");
INVALID(u"\xD800");
INVALID(u"\xDBFF");
INVALID(u"\xDC00");
INVALID(u"\xDFFF");

#undef VALID
#undef INVALID

