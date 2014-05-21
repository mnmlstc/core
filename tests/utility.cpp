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

    task("dismiss") = [] {
      bool value { false };
      {
        auto scope = core::make_scope_guard([&] { value = true; });
        scope.dismiss();
      }
      assert::is_false(value);
    }
  };

  test("value-at") = {
    task("runtime") = [] {
      auto value = core::value_at<3>(1, 2, 3, 4);
      assert::equal(value, 4);
    },

    task("compile-time") = [] {
      constexpr auto value = core::value_at<3>(1, 2, 3, 4);
      constexpr auto second = core::value_at<2>(1, 2, 4.0f, "");
      static_assert(value == 4, "");
      static_assert(second > 3.9f, "");
    }
  };

  monitor::run();
}
