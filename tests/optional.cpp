#include <core/optional.hpp>
#include <cstdint>

#include <unittest/unittest.hpp>

int main () {
  using namespace unittest;

  test("optional") = {
    task("default-constructor") = [] { assert::fail(); },
    task("copy-constructor") = [] { assert::fail(); },
    task("move-constructor") = [] { assert::fail(); },
    task("null-constructor") = [] { assert::fail(); },
    task("copy-value-constructor") = [] { assert::fail(); },
    task("move-value-constructor") = [] { assert::fail(); },
    task("in-place-constructor") = [] { assert::fail(); },
    task("null-assign-operator") = [] { assert::fail(); },
    task("copy-assign-operator") = [] { assert::fail(); },
    task("move-assign-operator") = [] { assert::fail(); },
    task("copy-value-assign-operator") = [] { assert::fail(); },
    task("move-value-assign-operator") = [] { assert::fail(); },
    task("arrow-operator") = [] { assert::fail(); },
    task("dereference-operator") = [] { assert::fail(); },
    task("operator-bool") = [] { assert::fail(); },
    task("value") = [] { assert::fail(); },
    task("value_or") = [] { assert::fail(); },
    task("emplace") = [] { assert::fail(); },

    task("equality-comparable") = [] { assert::fail(); },
    task("less-than-comparable") = [] { assert::fail(); },
    task("make-optional") = [] { assert::fail(); },
    task("swap") = [] { assert::fail(); },
    task("hash") = [] { assert::fail(); }
  };

  monitor::run();
}
