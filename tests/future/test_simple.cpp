#include "catch.hpp"
#include <s2/future>

TEST_CASE( "Can use a basic future", "[future]" ) {
  s2::future<int> i = s2::async([]{ return 42; });
  REQUIRE(i.get() == 42);
}


