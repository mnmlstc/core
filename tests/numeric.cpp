#include <core/numeric.hpp>
#include <vector>

#include <unittest/unittest.hpp>

int main () {
  using namespace unittest;
  test("numeric") = {
    task("iota") = [] { assert::fail(); },
    task("accumulate") = [] { assert::fail(); },
    task("inner-product") = [] { assert::fail(); },
    task("adjacent-difference") = [] { assert::fail(); },
    task("partial-sum") = [] { assert::fail(); }
  };

  monitor::run();
}
