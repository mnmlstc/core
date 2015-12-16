#include <core/iterator.hpp>
#include <algorithm>
#include <ostream>
#include <sstream>
#include <vector>

#include <cstdint>

#include "catch.hpp"

template <class T>
std::ostream& operator << (std::ostream& os, std::vector<T> const& vec) {
  os << "[";
  std::copy(
    core::cbegin(vec),
    core::cend(vec),
    core::make_ostream_joiner(os, ",")
  );
  return os << "]";
}

TEST_CASE("iterator") {
  std::vector<std::uint64_t> value = { 1, 2, 3, 4, 5 };

  SECTION("size") {
    int array[5];
    CHECK(core::size(value) == value.size());
    static_assert(core::size(array) == 5, "core::size");
  }

  SECTION("empty") {
    int array[5];
    CHECK_FALSE(core::empty(value));
    static_assert(not core::empty(array), "core::empty");
  }

  SECTION("front") {
    constexpr int array[5] { 0, 1, 2, 3, 4 };
    CHECK(core::front(value) == value.front());
    static_assert(core::front(array) == 0, "core::front");
  }

  SECTION("back") {
    constexpr int array[5] { 0, 1, 2, 3, 4 };
    CHECK(core::back(value) == value.back());
    static_assert(core::back(array) == 4, "core::back");
  }

  SECTION("data") {
    int array[5] { 1, 2, 3, 4, 5 };
    CHECK(core::data(value) == value.data());
    CHECK(core::data(array) == std::addressof(array[0]));
  }

  SECTION("cbegin") { CHECK(core::cbegin(value) == value.begin()); }
  SECTION("cend") { CHECK(core::cend(value) == value.end()); }

  SECTION("infix-ostream-iterator") {
    std::ostringstream stream;
    stream << value;
    CHECK(stream.str() == "[1,2,3,4,5]");
  }
}
