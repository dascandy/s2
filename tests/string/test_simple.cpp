#include <s2/string>
#include "catch.hpp"

TEST_CASE("create a string and destruct it", "[string]") {
  s2::string<s2::encoding::utf8> s;
}

TEST_CASE("Assemble a string with a rope over string views", "[string]") {
  s2::string s = "Hello World!";
  s2::string sv = s + 123 + s;
  REQUIRE(sv == "Hello World!123Hello World!");
}


