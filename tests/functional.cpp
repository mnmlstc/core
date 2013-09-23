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
        core::runpack<3> { },
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
        core::runpack<3> { },
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
          core::runpack<4> { },
          [] (char a, char b, char c, char d) -> std::string {
            return std::string { };
          },
          string
        );
      });

    },

    task("invoke-runtime-unpack-vector") = [] {
      std::vector<int> values { 1, 2, 3 };
      auto result = core::invoke(
        core::runpack<3> { },
        [](int x, int y, int z) -> int { return x + y + z; },
        values
      );

      assert::equal(result, 6);
    },

    task("invoke-runtime-unpack-deque") = [] {
      std::deque<int> values { 1, 2, 3 };
      auto result = core::invoke(
        core::runpack<3> { },
        [](int x, int y, int z) -> int { return x + y + z; },
        values
      );

      assert::equal(result, 6);
    },

    task("invoke-runtime-unpack-array") = [] {
      std::array<int, 3> values { { 1, 2, 3 } };
      auto result = core::invoke(
        core::runpack<3> { },
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
      auto result = core::invoke(
        core::runpack<2> { },
        [] (std::string x, std::string y) { return x + y; },
        values
      );
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

    task("invoke-unpack-array") = [] { assert::fail(); },

    task("invoke-unpack-pair") = [] {
      auto result = core::invoke(
        core::unpack,
        [](int x, std::string y) { return std::to_string(x) + y; },
        std::make_pair(7, "unpack")
      );

      assert::equal(result, std::string { "7unpack" });
    },

    task("invoke") = [] { assert::fail(); },
    task("function-traits") = [] { assert::fail(); },
  };

  monitor::run();
}
