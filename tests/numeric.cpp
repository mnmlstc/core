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

    task("inner-product") = [] { assert::fail(); },
    task("adjacent-difference") = [] { assert::fail(); },
    task("partial-sum") = [] { assert::fail(); }
  };

  monitor::run();
}
