#include "catch.hpp"
#include <s2/future>

TEST_CASE("do nothing in the default executor", "[future]") {
  s2::default_executor def;
}

TEST_CASE("Can use a basic future", "[future]") {
  s2::future<int> i = s2::async([]{ return 42; });
  REQUIRE(i.get() == 42);
}
/*
TEST_CASE("Can use void future", "[future]") {
  s2::future<void> i = s2::async([]{});
  i.get();
}
*/

