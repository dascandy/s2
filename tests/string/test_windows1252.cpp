#include <s2/string>
#include "catch.hpp"
#include <s2/detail/encoding_windows1252.h>

TEST_CASE("Test windows1252 string parsing", "[string/windows1252]") {
  s2::basic_string_view<s2::encoding::windows1252> winstring("Victor jagt zw\xF6lf Boxk\xE4mpfer quer \xFC" "ber den gro\xDF" "en Sylter Deich");
  s2::string_view ustring("Victor jagt zwölf Boxkämpfer quer über den großen Sylter Deich");
  REQUIRE(winstring == ustring);
}

TEST_CASE("Windows 1252 converts unknowns to question marks") {
  s2::string_view ustring2("Příliš žluťoučký kůň úpěl ďábelské ódy.");
  s2::basic_string<s2::encoding::windows1252> winstring2 = ustring2;

  s2::string_view ustring3("P?íliš žlu?ou?ký k?? úp?l ?ábelské ódy.");
  REQUIRE(winstring2.view() == ustring3);
}


