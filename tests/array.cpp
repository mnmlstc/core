#include <core/array.hpp>

#include "catch.hpp"

TEST_CASE("array-make-array") {
  auto const number = 420l;
  auto character = 's';
  auto make_array = core::make_array(number, character, 0);
  CHECK((std::is_same<decltype(make_array)::value_type, long>::value));
  CHECK(make_array.size() == 3);

  auto make_array_2 = core::make_array<long>(1337, 100U);
  CHECK((std::is_same<decltype(make_array_2)::value_type, long>::value));
  CHECK(make_array_2.size() == 2);
}

TEST_CASE("array-to-array") {
  auto to_array = core::to_array("deadbeef");
  CHECK((std::is_same<decltype(to_array)::value_type, char>::value));
  CHECK(to_array.size() == 9);
}
