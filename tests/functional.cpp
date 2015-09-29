#define CATCH_CONFIG_MAIN
#include <core/functional.hpp>

#include <unordered_map>
#include <vector>
#include <string>
#include <array>
#include <deque>
#include <map>

#include <sstream>

#include "catch.hpp"

TEST_CASE("apply", "[functional]") {
  SECTION("tuple") {
    auto result = core::apply(
      [] (int x, std::string const& y) { return std::to_string(x) + y; },
      std::forward_as_tuple(4, "apply"));
    CHECK(result == "4apply");
  }

  SECTION("array") {
    std::array<int, 3> values {{ 1, 2, 3 }};
    auto result = core::apply(
      [] (int x, int y, int z) { return x + y + z; },
      values);
    CHECK(result == 6);
  }

  SECTION("pair") {
    auto result = core::apply(
      [] (int x, std::string y) { return std::to_string(x) + y; },
      std::make_pair(7, "apply"));
    CHECK(result == "7apply");
  }
}

TEST_CASE("functional") {
  SECTION("function-traits") {
    auto empty_lambda = []{};
    auto empty_arity = core::function_traits<decltype(empty_lambda)>::arity;
    auto mem_fn_arity = core::function_traits<
      decltype(&std::string::size)
    >::arity;

    static_assert(
      std::is_same<
        std::string const&,
        core::function_traits<decltype(&std::string::size)>::argument<0>
      >::value,
      "function-traits-arity incorrect size"
    );
    CHECK(mem_fn_arity == 1u);
    CHECK(empty_arity == 0u);
  }
}
