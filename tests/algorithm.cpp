#include <core/algorithm.hpp>

#include <unittest/unittest.hpp>

int main () {
  using namespace unittest;

  test("algorithm") = {
    task("all-of") = [] { assert::fail(); },
    task("any-of") = [] { assert::fail(); },
    task("none-of") = [] { assert::fail(); },
    task("for-each") = [] { assert::fail(); },
    task("count") = [] { assert::fail(); },
    task("count-if") = [] { assert::fail(); },
    task("mismatch") = [] { assert::fail(); },
    task("equal") = [] { assert::fail(); },
    task("find") = [] { assert::fail(); },
    task("find-if") = [] { assert::fail(); },
    task("find-if-not") = [] { assert::fail(); },
    task("find-end") = [] { assert::fail(); },
    task("find-first-of") = [] { assert::fail(); },
    task("adjacent-find") = [] { assert::fail(); },
    task("search") = [] { assert::fail(); },
    task("search-n") = [] { assert::fail(); },
    task("copy") = [] { assert::fail(); },
    task("copy-if") = [] { assert::fail(); },
    task("copy-n") = [] { assert::fail(); },
    task("copy-backward") = [] { assert::fail(); },
    task("move") = [] { assert::fail(); },
    task("move-backward") = [] { assert::fail(); },
    task("fill") = [] { assert::fail(); },
    task("fill-n") = [] { assert::fail(); },
    task("transform") = [] { assert::fail(); }
  };

  monitor::run();
}
