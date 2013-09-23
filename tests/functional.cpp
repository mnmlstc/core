#include <core/functional.hpp>

#include <unordered_map>
#include <vector>
#include <string>
#include <array>
#include <deque>
#include <map>

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
    task("invoke-runtime-unpack-basic-string") = [] { assert::fail(); },
    task("invoke-runtime-unpack-vector") = [] { assert::fail(); },
    task("invoke-runtime-unpack-deque") = [] { assert::fail(); },
    task("invoke-runtime-unpack-array") = [] { assert::fail(); },
    task("invoke-runtime-unpack-map") = [] { assert::fail(); },
    task("invoke-unpack-tuple") = [] { assert::fail(); },
    task("invoke-unpack-array") = [] { assert::fail(); },
    task("invoke-unpack-pair") = [] { assert::fail(); },
    task("invoke") = [] { assert::fail(); },
    task("function-traits") = [] { assert::fail(); },
  };

  monitor::run();
}
