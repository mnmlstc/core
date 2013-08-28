#include <core/optional.hpp>
#include <string>
#include <cstdint>

#include <unittest/unittest.hpp>

int main () {
  using namespace unittest;

  test("optional") = {
    task("default-constructor") = [] {
      core::optional<int> opt;
      assert::is_true(not opt);
      assert::equal(opt, core::nullopt);
    },

    task("copy-constructor") = [] {
      core::optional<int> opt { 4 };
      core::optional<int> copy { opt };
      assert::is_false(not opt);
      assert::is_false(not copy);
      assert::equal(opt, copy);
      assert::equal(copy.value(), 4);
    },
    task("move-constructor") = [] { assert::fail(); },
    task("null-constructor") = [] { assert::fail(); },
    task("copy-value-constructor") = [] { assert::fail(); },
    task("move-value-constructor") = [] {
      std::string text { "move-value" };
      core::optional<std::string> opt { std::move(text) };
      assert::is_true(bool(opt));
      assert::is_true(text.empty());
      assert::equal(opt.value(), std::string { "move-value" });
    },
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
