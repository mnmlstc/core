#include <core/utility.hpp>

#include <unittest/unittest.hpp>

int main () {
  using namespace unittest;

  test("scope") = {
    task("value-constructor") = [] {
      bool value { false };
      assert::is_false(value);
      { core::scope scope { [&value]{ value = true; } }; }
      assert::is_true(value);
    },

    task("value-assignment") = [] {
      bool value { false };
      assert::is_false(value);
      {
        core::scope scope { [&value] { value = true; } };
        scope = [] { };
      }
      assert::is_false(value);
    },

    task("swap") = [] {
      bool lhs { false };
      bool rhs { true };
      assert::is_false(lhs);
      assert::is_true(rhs);
      {
        core::scope lhs_scope { [&lhs] { lhs = true; } };
        core::scope rhs_scope { [&rhs] { rhs = false; } };
        std::swap(lhs_scope, rhs_scope);
      }
      assert::is_false(rhs);
      assert::is_true(lhs);
    }
  };

  monitor::run();
}
