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
  SECTION("invoke-runtime-unpack-unordered-map") {
    std::unordered_map<int, std::string> values = {
      { 0, "first" },
      { 1, "second" },
      { 2, "third" }
    };

    auto result = core::invoke(
      core::runpack,
      [](std::string& a, std::string& b, std::string& c) -> std::string {
        return a + b + c;
      },
      values
    );

    CHECK(result == "firstsecondthird");
  }

  SECTION("invoke-runtime-unpack-basic-string") {
    std::string string { "hi!" };
    auto result = core::invoke(
      core::runpack,
      [] (char a, char b, char c) -> std::string {
        std::ostringstream stream;
        stream << a << b << c;
        return stream.str();
      },
      string
    );

    CHECK(string == result);
    CHECK_THROWS([string] {
      std::ignore = core::invoke(
        core::runpack,
        [] (char, char, char, char) -> std::string {
          return std::string { };
        },
        string
      );
    }());

  }

  SECTION("invoke-runtime-unpack-vector") {
    std::vector<int> values { 1, 2, 3 };
    auto result = core::invoke(
      core::runpack,
      [](int x, int y, int z) -> int { return x + y + z; },
      values
    );

    CHECK(result == 6);
  }

  SECTION("invoke-runtime-unpack-deque") {
    std::deque<int> values { 1, 2, 3 };
    auto result = core::invoke(
      core::runpack,
      [](int x, int y, int z) -> int { return x + y + z; },
      values
    );

    CHECK(result == 6);
  }

  SECTION("invoke-runtime-unpack-array") {
    std::array<int, 3> values { { 1, 2, 3 } };
    auto result = core::invoke(
      core::runpack,
      [](int x, int y, int z) -> int { return x + y + z; },
      values
    );

    CHECK(result == 6);
  }

  SECTION("invoke-runtime-unpack-map") {
    std::map<int, std::string> values = {
      { 0, "first" },
      { 1, "second" },
      { 2, "third" }
    };

    auto lambda = [] (std::string x, std::string y) { return x + y; };

    auto result = core::invoke(core::runpack, lambda, values);
    CHECK(result == "firstsecond");
  }

  SECTION("invoke-unpack-tuple") {
    auto result = core::invoke(
      core::unpack,
      [](int x, std::string y) { return std::to_string(x) + y; },
      std::forward_as_tuple(4, "unpack")
    );

    CHECK(result == "4unpack");
  }

  SECTION("invoke-unpack-array") {
    std::array<int, 3> values = {{ 1, 2, 3 }};
    auto result = core::invoke(
      core::unpack,
      [](int x, int y, int z) { return x + y + z; },
      values
    );

    CHECK(result == 6);
  }

  SECTION("invoke-unpack-pair") {
    auto result = core::invoke(
      core::unpack,
      [](int x, std::string y) { return std::to_string(x) + y; },
      std::make_pair(7, "unpack")
    );

    CHECK(result == "7unpack");
  }

  SECTION("invoke-constexpr") {
    struct type {
      constexpr type () noexcept { }
      constexpr int operator ()() const noexcept { return 5; }
      constexpr int operator ()(int value) const noexcept { return value + 5; }
    };
    constexpr type value { };
    static_assert(core::invoke(value) == 5, "");
    static_assert(core::invoke(value, 6) == 11, "");
  }

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
