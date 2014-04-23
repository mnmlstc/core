#include <core/functional.hpp>

#include <unordered_map>
#include <vector>
#include <string>
#include <array>
#include <deque>
#include <map>

#include <sstream>

#include <unittest/unittest.hpp>

int main () {
  using namespace unittest;

  test("functional") = {
    task("invoke-runtime-unpack-unordered-map") = [] {
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

      assert::equal(result, std::string { "firstsecondthird" });
    },

    task("invoke-runtime-unpack-basic-string") = [] {
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

      assert::equal(string, result);
      assert::throws<std::out_of_range>([string] {
        std::ignore = core::invoke(
          core::runpack,
          [] (char, char, char, char) -> std::string {
            return std::string { };
          },
          string
        );
      });

    },

    task("invoke-runtime-unpack-vector") = [] {
      std::vector<int> values { 1, 2, 3 };
      auto result = core::invoke(
        core::runpack,
        [](int x, int y, int z) -> int { return x + y + z; },
        values
      );

      assert::equal(result, 6);
    },

    task("invoke-runtime-unpack-deque") = [] {
      std::deque<int> values { 1, 2, 3 };
      auto result = core::invoke(
        core::runpack,
        [](int x, int y, int z) -> int { return x + y + z; },
        values
      );

      assert::equal(result, 6);
    },

    task("invoke-runtime-unpack-array") = [] {
      std::array<int, 3> values { { 1, 2, 3 } };
      auto result = core::invoke(
        core::runpack,
        [](int x, int y, int z) -> int { return x + y + z; },
        values
      );

      assert::equal(result, 6);
    },

    task("invoke-runtime-unpack-map") = [] {
      std::map<int, std::string> values = {
        { 0, "first" },
        { 1, "second" },
        { 2, "third" }
      };

      auto lambda = [] (std::string x, std::string y) { return x + y; };

      auto result = core::invoke(core::runpack, lambda, values);
      assert::equal(result, std::string { "firstsecond" });
    },

    task("invoke-unpack-tuple") = [] {
      auto result = core::invoke(
        core::unpack,
        [](int x, std::string y) { return std::to_string(x) + y; },
        std::forward_as_tuple(4, "unpack")
      );

      assert::equal(result, std::string { "4unpack" });
    },

    task("invoke-unpack-array") = [] {
      std::array<int, 3> values = {{ 1, 2, 3 }};
      auto result = core::invoke(
        core::unpack,
        [](int x, int y, int z) { return x + y + z; },
        values
      );

      assert::equal(result, 6);
    },

    task("invoke-unpack-pair") = [] {
      auto result = core::invoke(
        core::unpack,
        [](int x, std::string y) { return std::to_string(x) + y; },
        std::make_pair(7, "unpack")
      );

      assert::equal(result, std::string { "7unpack" });

      struct type {
        constexpr type () noexcept { }
        constexpr int operator ()() const noexcept { return 5; }
        constexpr int operator ()(int value) const noexcept { return value + 5; }
      };
      constexpr type value { };
      static_assert(core::invoke(value) == 5, "");
      static_assert(core::invoke(value, 6) == 11, "");
    },

    task("function-traits") = [] {
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
      assert::equal(mem_fn_arity, 1u);
      assert::equal(empty_arity, 0u);
    },
  };

  monitor::run();
}
