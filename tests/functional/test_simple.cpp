#include "catch.hpp"
#include <s2/functional>

TEST_CASE("create a function and destruct it", "[functional]") {
  s2::function<void()> v;
}

TEST_CASE("Create a simple function and call it", "[functional]") {
  s2::function<int(int)> f{ [](int i){ return i*5 +2; }};
  REQUIRE(f(8) == 42);
  REQUIRE(f(6) == 32);
}

TEST_CASE("Can move a function", "[functional]") {
  s2::function<int(int)> f1 = [](int i) { return i; };
  REQUIRE(f1);
  s2::function<int(int)> f2 = std::move(f1);
  REQUIRE(!f1);
  REQUIRE(f2);
}


