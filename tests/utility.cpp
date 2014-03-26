#include <core/utility.hpp>

#include <unittest/unittest.hpp>

int main () {
  using namespace unittest;

  test("scope-guard") = {
    task("value-constructor") = [] {
      bool value { false };
      { auto scope = core::make_scope_guard([&]{ value = true; }); }
      assert::is_true(value);
    },
    task("constructor") = [] {
      bool value { false };
      { core::scope_guard guard([&] { value = true;}); }
      assert::is_true(value);
    },
    task("ignore-return") = [] {
      bool value { false };
      { core::scope_guard guard([&] { return value = true;}); }
      assert::is_true(value);
    }
  };

  monitor::run();
}
