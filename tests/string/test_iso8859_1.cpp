#include <s2/string>
#include "catch.hpp"
#include <s2/detail/encoding_iso8859_1.h>

TEST_CASE("Test iso8859_1 string parsing", "[string/iso8859_1]") {
  s2::basic_string_view<s2::encoding::iso8859_1> winstring("Victor jagt zw\xF6lf Boxk\xE4mpfer quer \xFC" "ber den gro\xDF" "en Sylter Deich");
  s2::string_view ustring("Victor jagt zwölf Boxkämpfer quer über den großen Sylter Deich");
  REQUIRE(winstring == ustring);
}

TEST_CASE("iso8859_1 converts unknowns to question marks") {
  s2::string_view ustring2("Příliš žluťoučký kůň úpěl ďábelské ódy.");
  s2::basic_string<s2::encoding::iso8859_1> winstring2 = ustring2;

  s2::string_view ustring3("P?íli? ?lu?ou?ký k?? úp?l ?ábelské ódy.");
  REQUIRE(winstring2.view() == ustring3);
}

TEST_CASE("iso8859_1 differs from _15") {
  s2::basic_string_view<s2::encoding::iso8859_1> v("\xA4 \xA8");
  s2::string_view v2("¤ ¨");
  REQUIRE(v == v2);
}


