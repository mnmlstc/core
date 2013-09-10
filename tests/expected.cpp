#include <core/expected.hpp>

#include <unittest/unittest.hpp>

struct type {
  type (type const&) = default;
  type (type&&) = default;
  type () = default;
  ~type () { std::clog << "It's working!" << std::endl; }

  type& operator = (type const&) = default;
  type& operator = (type&&) = default;
};

int main () {
  using namespace unittest;

  core::expected<type> value { type { } };

  test("expected") = {
    task("copy-value-constructor") = [] { assert::fail(); },
    task("move-value-constructor") = [] { assert::fail(); },
    task("move-constructor") = [] { assert::fail(); },
    task("copy-constructor") = [] { assert::fail(); },
    task("ptr-constructor") = [] { assert::fail(); },
    task("copy-value-assign-operator") = [] { assert::fail(); },
    task("move-value-assign-operator") = [] { assert::fail(); },
    task("move-assign-operator") = [] { assert::fail(); },
    task("copy-assign-operator") = [] { assert::fail(); },
    task("ptr-assign-operator") = [] { assert::fail(); },
    task("dereference-operator") = [] { assert::fail(); },
    task("equality-comparable") = [] { assert::fail(); },
    task("less-than-comparable") = [] { assert::fail(); },
    task("value-or") = [] { assert::fail(); },
    task("value") = [] { assert::fail(); },
    task("expect") = [] { assert::fail(); },
    task("raise") = [] { assert::fail(); },
    task("swap") = [] { assert::fail(); }
  };

  monitor::run();
}
