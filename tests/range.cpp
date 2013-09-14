#include <core/range.hpp>

#include <unittest/unittest.hpp>

int main () {
  using namespace unittest;

  test("range") = {
    task("default-constructor") = []{ assert::fail(); }
  };

  monitor::run();
}
