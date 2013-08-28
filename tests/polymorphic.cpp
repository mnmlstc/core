#include <core/polymorphic.hpp>

#include <unittest/unittest.hpp>

struct base {
  base (base const&) = default;
  base (base&&) = default;
  base () = default;
  virtual ~base () { }
};

struct derived : base {

};

int main () {
  using namespace unittest;

  test("polymorphic<T>") = {
    task("default-constructor") = []{ assert::fail(); },
    task("value-constructor") = []{ assert::fail(); },
    task("copy-constructor") = []{ assert::fail(); },
    task("move-constructor") = []{ assert::fail(); },
    task("value-assignment") = []{ assert::fail(); },
    task("copy-assignment") = []{ assert::fail(); },
    task("move-assignment") = []{ assert::fail(); },

    task("operator-bool") = [] {
      core::polymorphic<base> value;
      assert::is_true(not value);
      value = derived { };
      assert::is_true(bool(value));
    },
  };

  monitor::run();
}
