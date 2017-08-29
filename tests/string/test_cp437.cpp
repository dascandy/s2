#include <s2/string>
#include "catch.hpp"
#include <s2/detail/encoding_cp437.h>

TEST_CASE("Test cp437 string parsing", "[string/cp437]") {
  s2::basic_string_view<s2::encoding::cp437> winstring("Victor jagt zw\x94lf Boxk\x84mpfer quer \x81" "ber den gro\xE1" "en Sylter Deich");
  s2::string_view ustring("Victor jagt zwölf Boxkämpfer quer über den großen Sylter Deich");
  REQUIRE(winstring == ustring);
}

TEST_CASE("Cp437 converts unknowns to question marks") {
  s2::string_view ustring2("Příliš žluťoučký kůň úpěl ďábelské ódy.");
  s2::basic_string<s2::encoding::cp437> winstring2 = ustring2;

  s2::string_view ustring3("P?íli? ?lu?ou?k? k?? úp?l ?ábelské ódy.");
  REQUIRE(winstring2.view() == ustring3);
}


