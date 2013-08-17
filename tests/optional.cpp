#include <core/optional.hpp>
#include <cstdint>

#include <unittest/unittest.hpp>

int main () {
  using namespace unittest;

  test("optional") = {
    task("swap") = [] { assert::fail(); }
  };
}
