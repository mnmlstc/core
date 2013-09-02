#include <core/polymorphic.hpp>

#include <unittest/unittest.hpp>

struct base {
  base (base const&) = default;
  base (base&&) = default;
  base () = default;
  virtual ~base () { }
};

struct derived : base {
  int value = 42;
};

int main () {
  using namespace unittest;

  test("polymorphic<T>") = {
    task("default-constructor") = []{
      core::polymorphic<base> value;
      assert::is_true(not value);
    },

    task("value-constructor") = [] {
      core::polymorphic<base> value { new derived { } };
      assert::is_false(not value);
      assert::equal(typeid(derived), typeid(*value));
    },

    task("copy-constructor") = [] {
      core::polymorphic<base> value { new derived { } };
      core::polymorphic<base> copy { value };

      assert::is_false(not value);
      assert::is_false(not copy);

      assert::equal(typeid(derived), typeid(*value));
      assert::equal(typeid(derived), typeid(*copy));

      assert::equal(dynamic_cast<derived&>(*copy).value, 42);
      assert::equal(
        dynamic_cast<derived&>(*copy).value,
        dynamic_cast<derived&>(*value).value
      );
    },

    task("move-constructor") = []{
      assert::fail();
    },
    task("value-assignment") = []{ assert::fail(); },
    task("copy-assignment") = []{ assert::fail(); },
    task("move-assignment") = []{ assert::fail(); },

    task("operator-bool") = [] {
      core::polymorphic<base> value;
      assert::is_true(not value);
      value = new derived { };
      assert::is_true(bool(value));
    },

    task("dereference-operator") = [] { assert::fail(); },
    task("arrow-operator") = [] { assert::fail(); },
    task("release") = [] { assert::fail(); },
    task("reset") = [] { assert::fail(); },
    task("swap") = [] { assert::fail(); },
    task("get") = [] { assert::fail(); },

    task("get-copier") = [] {
      core::polymorphic<base> value { new derived { } };
      auto poly_copy = core::default_poly_copy<
        base,
        std::default_delete<base>,
        derived
      >;

      assert::equal(value.get_copier(), poly_copy);
    },

    task("operator-equal") = [] {
      core::polymorphic<base> lhs, rhs;
      core::polymorphic<derived> der;
      assert::equal(lhs, rhs);
      assert::equal(lhs, nullptr);
    },

    task("operator-not-equal") = [] { assert::fail(); },
    task("operator-greater-than-or-equal") = [] { assert::fail(); },
    task("operator-less-than-or-equal") = [] { assert::fail(); },
    task("operator-greater-than") = [] { assert::fail(); },
    task("operator-less-than") = [] { assert::fail(); }

  };

  monitor::run();
}
