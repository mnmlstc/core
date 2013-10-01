#include <core/string.hpp>

#include <unittest/unittest.hpp>

int main () {
  using namespace unittest;

  test("string") = {
    task("default-constructor") = [] { assert::fail(); },
    task("copy-constructor") = [] { assert::fail(); },
    task("string-constructor") = [] { assert::fail(); },
    task("pointer-constructor") = [] { assert::fail(); },
    task("pointer-len-constructor") = [] { assert::fail(); },
    task("copy-assignment-operator") = [] { assert::fail(); },
    task("explicit-string-cast") = [] { assert::fail(); },
    task("begin") = [] { assert::fail(); },
    task("end") = [] { assert::fail(); },
    task("cbegin") = [] { assert::fail(); },
    task("cend") = [] { assert::fail(); },
    task("rbegin") = [] { assert::fail(); },
    task("rend") = [] { assert::fail(); },
    task("crbegin") = [] { assert::fail(); },
    task("crend") = [] { assert::fail(); },
    task("max-size") = [] { assert::fail(); },
    task("length") = [] { assert::fail(); },
    task("size") = [] { assert::fail(); },
    task("empty") = [] { assert::fail(); },
    task("subscript-operator") = [] { assert::fail(); },
    task("front") = [] { assert::fail(); },
    task("back") = [] { assert::fail(); },
    task("data") = [] { assert::fail(); },
    task("remove-prefix") = [] { assert::fail(); },
    task("remove-suffix") = [] { assert::fail(); },
    task("clear") = [] { assert::fail(); },
    task("substr") = [] { assert::fail(); },
    task("starts-with") = [] { assert::fail(); },
    task("ends-with") = [] { assert::fail(); },
    task("compare") = [] { assert::fail(); },
    task("at") = [] { assert::fail(); },
    task("find-first-not-of") = [] { assert::fail(); },
    task("find-last-not-of") = [] { assert::fail(); },
    task("find-first-of") = [] { assert::fail(); },
    task("find-last-of") = [] { assert::fail(); },
    task("rfind") = [] { assert::fail(); },
    task("find") = [] { assert::fail(); },
    task("swap") = [] { assert::fail(); },
    task("operator-equal") = [] { assert::fail(); },
    task("operator-not-equal") = [] { assert::fail(); },
    task("operator-greater-equal") = [] { assert::fail(); },
    task("operator-less-equal") = [] { assert::fail(); },
    task("operator-greater") = [] { assert::fail(); },
    task("operator-less") = [] { assert::fail(); },
    task("operator-stream-insert") = [] { assert::fail(); },
    task("hash") = [] { assert::fail(); }
  };

  monitor::run();
}
