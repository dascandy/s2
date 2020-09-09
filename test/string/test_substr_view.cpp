#include <s2/string.h>
#include "catch.hpp"

TEST_CASE("create a substring of a string", "[string]") {
  s2::string s = "Hello Unicode World";
  auto x = substr(s, 6, 7);
  REQUIRE(x == decltype(x)("Unicode"));
  static_assert(std::is_same_v<decltype(x), s2::basic_string_view<s2::encoding::utf8>>, "Should be a string view return");
}

TEST_CASE("trim a string", "[string]") {
  s2::string s = "   \t Unicode \n\r ";
  auto x = trim(s);
  REQUIRE(x == decltype(x)("Unicode"));
  static_assert(std::is_same_v<decltype(x), s2::basic_string_view<s2::encoding::utf8>>, "Should be a string view return");
}

TEST_CASE("left-trim a string", "[string]") {
  s2::string s = "   \t Unicode \n\r ";
  auto x = trim_left(s);
  REQUIRE(x == decltype(x)("Unicode \n\r "));
  static_assert(std::is_same_v<decltype(x), s2::basic_string_view<s2::encoding::utf8>>, "Should be a string view return");
}

TEST_CASE("right-trim a string", "[string]") {
  s2::string s = "   \t Unicode \n\r ";
  auto x = trim_right(s);
  REQUIRE(x == decltype(x)("   \t Unicode"));
  static_assert(std::is_same_v<decltype(x), s2::basic_string_view<s2::encoding::utf8>>, "Should be a string view return");
}

TEST_CASE("only whitespace on an empty string","[string]") {
  s2::string s = "";
  REQUIRE(only_whitespace(s));
}

TEST_CASE("only whitespace on a string with whitespace","[string]") {
  s2::string s = " \t\n\r  \xA0";
  REQUIRE(only_whitespace(s));
}

TEST_CASE("not only whitespace on a nonempty string","[string]") {
  s2::string s = "              N";
  REQUIRE(!only_whitespace(s));
}


