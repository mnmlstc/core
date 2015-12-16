#include <core/utility.hpp>

#include "catch.hpp"

TEST_CASE("scope-guard", "[scope-guard]") {
  SECTION("value-constructor") {
    bool value { false };
    { auto scope = core::make_scope_guard([&]{ value = true; }); }
    CHECK(value);
  }

  SECTION("dismiss") {
    bool value { false };
    {
      auto scope = core::make_scope_guard([&] { value = true; });
      scope.dismiss();
    }
    CHECK_FALSE(value);
  }
}

TEST_CASE("value-at") {
  SECTION("runtime") {
    auto value = core::value_at<3>(1, 2, 3, 4);
    CHECK(value == 4);
  }

  SECTION("compile-time") {
    constexpr auto value = core::value_at<3>(1, 2, 3, 4);
    constexpr auto second = core::value_at<2>(1, 2, 4.0f, "");
    static_assert(value == 4, "");
    static_assert(second > 3.9f, "");
  }
}
