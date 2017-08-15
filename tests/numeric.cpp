#include <core/numeric.hpp>

#include <functional>
#include <vector>

#include "catch.hpp"

TEST_CASE("numeric") {
  SECTION("iota") {
    std::vector<int> values = { 0, 0, 0, 0, 0 };
    core::iota(values, 0);
    CHECK(values[0] == 0);
    CHECK(values[1] == 1);
    CHECK(values[2] == 2);
    CHECK(values[3] == 3);
    CHECK(values[4] == 4);
  }

  SECTION("accumulate") {
    std::vector<int> values = { 1, 2, 3, 4 };
    auto result = core::accumulate(values, 0);
    CHECK(result == 10);
    result = core::accumulate(values, 1, std::multiplies<int> { });
    CHECK(result == 24);
  }

  SECTION("inner-product") {
    std::vector<int> value_one { 0, 1, 2, 3, 4 };
    std::vector<int> value_two { 5, 4, 3, 2, 1 };
    auto result = core::inner_product(value_one, ::std::begin(value_two), 0);
    CHECK(result == 20);
    result = core::inner_product(
      value_one,
      std::begin(value_two),
      1,
      std::multiplies<int> { },
      std::plus<int> { }
    );
    CHECK(result == 3125);
  }

  SECTION("adjacent-difference") {
    std::vector<int> values { 2, 4, 6, 8, 10 };
    auto result = std::vector<int>(values.size());
    core::adjacent_difference(values, std::begin(result));
    CHECK(result[0] == 2);
    CHECK(result[1] == 2);
    CHECK(result[2] == 2);
    CHECK(result[3] == 2);
    CHECK(result[4] == 2);

    core::adjacent_difference(values, std::begin(result), std::plus<int> { });
    CHECK(result[0] == 2);
    CHECK(result[1] == 6);
    CHECK(result[2] == 10);
    CHECK(result[3] == 14);
    CHECK(result[4] == 18);
  }

  SECTION("partial-sum") {
    std::vector<int> values { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };
    auto result = std::vector<int>(values.size());

    core::partial_sum(values, std::begin(result));
    CHECK(result[0] == 2);
    CHECK(result[1] == 4);
    CHECK(result[2] == 6);
    CHECK(result[3] == 8);
    CHECK(result[4] == 10);
    CHECK(result[5] == 12);
    CHECK(result[6] == 14);
    CHECK(result[7] == 16);
    CHECK(result[8] == 18);
    CHECK(result[9] == 20);

    core::partial_sum(values, std::begin(result), std::multiplies<int> { });
    CHECK(result[0] == 2);
    CHECK(result[1] == 4);
    CHECK(result[2] == 8);
    CHECK(result[3] == 16);
    CHECK(result[4] == 32);
    CHECK(result[5] == 64);
    CHECK(result[6] == 128);
    CHECK(result[7] == 256);
    CHECK(result[8] == 512);
    CHECK(result[9] == 1024);


  }
}
