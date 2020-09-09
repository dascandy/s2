#include <s2/string.h>
#include "catch.hpp"

TEST_CASE("create a string and destruct it", "[string]") {
  s2::string s;
}

TEST_CASE("Assemble a string with a rope over string views", "[string]") {
  s2::string s = "Hello World!";
  s2::string sv = s.view() + 123 + s.view();
  s2::string_view c = "Hello World!123Hello World!";
  REQUIRE(sv.view() == c);
}


