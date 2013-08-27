#include <core/polymorphic.hpp>

#include <unittest/unittest.hpp>

int main () {
  using namespace unittest;

  test("polymorphic<T>") = {
    task("default-constructor") = []{ assert::fail(); }
  };

  monitor::run();
}
