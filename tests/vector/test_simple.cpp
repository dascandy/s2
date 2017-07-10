#include "catch.hpp"
#include <s2/vector>

TEST_CASE("create a vector and destruct it", "[vector]") {
  s2::vector<int> v;
}

TEST_CASE("push a single number into a vector & retrieve it", "[vector]") {
  static const int testval = 1123581321;
  s2::vector<int> i;
  REQUIRE(i.capacity() == 31);
  REQUIRE(i.size() == 0);
  i.push_back(testval);
  REQUIRE(i.size() == 1);
  REQUIRE(i.capacity() == 31);
  REQUIRE(i[0] == testval);
  auto v = i.pop_back();
  REQUIRE(i.size() == 0);
  REQUIRE(i.capacity() == 31);
  REQUIRE(v == testval);
}

TEST_CASE("can externalize", "[vector]") {
  s2::vector<int> v;
  for (size_t n = 0; n < 32; n++) { v.push_back(n); }
  REQUIRE(v.capacity() > 40);
  REQUIRE(v.capacity() < 60);
  for (size_t n = 0; n < 32; n++) { REQUIRE(v[n] == n); }
}

struct nc {
  nc() = default;
  nc(nc&&) = default;
  nc& operator=(nc&&) = default;
  nc(const nc&) = delete;
  nc& operator=(const nc&) = delete;
};

TEST_CASE("can store noncopyables", "[vector]") {
  s2::vector<nc> ncs;
  ncs.push_back(nc());
  ncs.emplace_back();
  ncs.pop_back();
  ncs.resize(0);
}


