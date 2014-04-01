#include <core/numeric.hpp>

#include <functional>
#include <vector>

#include <unittest/unittest.hpp>

int main () {
  using namespace unittest;
  test("numeric") = {
    task("iota") = [] {
      std::vector<int> values = { 0, 0, 0, 0, 0 };
      core::iota(values, 0);
      assert::equal(values[0], 0);
      assert::equal(values[1], 1);
      assert::equal(values[2], 2);
      assert::equal(values[3], 3);
      assert::equal(values[4], 4);
    },

    task("accumulate") = [] {
      std::vector<int> values = { 1, 2, 3, 4 };
      auto result = core::accumulate(values, 0);
      assert::equal(result, 10);
      result = core::accumulate(values, 1, std::multiplies<int> { });
      assert::equal(result, 24);
    },

    task("inner-product") = [] {
      std::vector<int> value_one { 0, 1, 2, 3, 4 };
      std::vector<int> value_two { 5, 4, 3, 2, 1 };
      auto result = core::inner_product(value_one, ::std::begin(value_two), 0);
      assert::equal(result, 20);
      result = core::inner_product(
        value_one,
        std::begin(value_two),
        1,
        std::multiplies<int> { },
        std::plus<int> { }
      );
      assert::equal(result, 3125);
    },

    task("adjacent-difference") = [] {
      std::vector<int> values { 2, 4, 6, 8, 10 };
      auto result = std::vector<int>(values.size());
      core::adjacent_difference(values, std::begin(result));
      assert::equal(result[0], 2);
      assert::equal(result[1], 2);
      assert::equal(result[2], 2);
      assert::equal(result[3], 2);
      assert::equal(result[4], 2);

      core::adjacent_difference(values, std::begin(result), std::plus<int> { });
      assert::equal(result[0], 2);
      assert::equal(result[1], 6);
      assert::equal(result[2], 10);
      assert::equal(result[3], 14);
      assert::equal(result[4], 18);
    },

    task("partial-sum") = [] {
      std::vector<int> values { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };
      auto result = std::vector<int>(values.size());

      core::partial_sum(values, std::begin(result));
      assert::equal(result[0], 2);
      assert::equal(result[1], 4);
      assert::equal(result[2], 6);
      assert::equal(result[3], 8);
      assert::equal(result[4], 10);
      assert::equal(result[5], 12);
      assert::equal(result[6], 14);
      assert::equal(result[7], 16);
      assert::equal(result[8], 18);
      assert::equal(result[9], 20);

      core::partial_sum(values, std::begin(result), std::multiplies<int> { });
      assert::equal(result[0], 2);
      assert::equal(result[1], 4);
      assert::equal(result[2], 8);
      assert::equal(result[3], 16);
      assert::equal(result[4], 32);
      assert::equal(result[5], 64);
      assert::equal(result[6], 128);
      assert::equal(result[7], 256);
      assert::equal(result[8], 512);
      assert::equal(result[9], 1024);


    }
  };

  monitor::run();
}
