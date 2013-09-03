#include <core/polymorphic.hpp>

#include <unittest/unittest.hpp>

struct base {
  base (base const&) = default;
  base (base&&) = default;
  base () = default;
  virtual ~base () { }

  virtual int get () const noexcept { return 0; }
};

struct derived : base {
  int value = 42;

  virtual int get () const noexcept override { return this->value; }
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
      core::polymorphic<base> value { new derived { } };
      core::polymorphic<base> move { std::move(value) };

      assert::is_true(not value);
      assert::is_true(bool(move));

      assert::equal(dynamic_cast<derived&>(*move).value, 42);
    },

    task("value-assignment") = []{ assert::fail(); },

    task("copy-assignment") = []{
      core::polymorphic<base> value { new derived { } };
      core::polymorphic<base> copy { };
      copy = value;

      assert::is_true(bool(value));
      assert::is_true(bool(copy));

      assert::equal(typeid(derived), typeid(*value));
      assert::equal(typeid(derived), typeid(*copy));

      assert::equal(dynamic_cast<derived&>(*copy).value, 42);
      assert::equal(
        dynamic_cast<derived&>(*copy).value,
        dynamic_cast<derived&>(*value).value
      );
    },

    task("move-assignment") = []{
      core::polymorphic<base> value { new derived { } };
      core::polymorphic<base> move { };
      move = std::move(value);

      assert::is_true(not value);
      assert::is_true(bool(move));

      assert::equal(dynamic_cast<derived&>(*move).value, 42);
    },

    task("operator-bool") = [] {
      core::polymorphic<base> value;
      assert::is_true(not value);
      value = new derived { };
      assert::is_true(bool(value));
    },

    task("dereference-operator") = [] {
      core::polymorphic<base> poly { new derived { } };
      base& parent = *poly;
      assert::equal(typeid(parent), typeid(derived));
    },

    task("arrow-operator") = [] {
      core::polymorphic<base> poly { new derived { } };
      assert::equal(poly->get(), 42);
    },

    task("release") = [] {
      core::polymorphic<base> poly { new derived { } };
      assert::is_true(bool(poly));
      poly.release();
      assert::is_true(not poly);
    },

    task("reset") = [] {
      assert::fail();
    },

    task("swap") = [] {
      core::polymorphic<base> lhs { new derived { } };
      core::polymorphic<base> rhs { };

      assert::is_true(bool(lhs));
      assert::is_true(not rhs);

      std::swap(lhs, rhs);

      assert::is_true(not lhs);
      assert::is_true(bool(rhs));
    },

    task("get") = [] {
      core::polymorphic<base> const poly { new derived { } };
      derived* ptr = dynamic_cast<derived*>(poly.get());
      assert::is_not_null(ptr);
    },

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

    task("operator-not-equal") = [] {
      core::polymorphic<base> lhs { new derived { } };
      core::polymorphic<base> rhs { };
      assert::not_equal(lhs, rhs);
    },

    task("operator-greater-than-or-equal") = [] {
      core::polymorphic<base> lhs { new derived { } };
      core::polymorphic<base> rhs { };

      assert::greater_equal(lhs, rhs);
      assert::greater_equal(lhs, nullptr);
      assert::greater_equal(nullptr, rhs);
    },

    task("operator-less-than-or-equal") = [] {
      core::polymorphic<base> lhs { };
      core::polymorphic<base> rhs { new derived { } };

      assert::less_equal(lhs, rhs);
      assert::less_equal(lhs, nullptr);
      assert::less_equal(nullptr, lhs);
      assert::less_equal(nullptr, rhs);
    },

    task("operator-greater-than") = [] {
      core::polymorphic<base> lhs { new derived { } };
      core::polymorphic<base> rhs { };

      assert::greater(lhs, rhs);
      assert::greater(lhs, nullptr);
    },

    task("operator-less-than") = [] {
      core::polymorphic<base> lhs { };
      core::polymorphic<base> rhs { new derived { } };

      assert::less(lhs, rhs);
      assert::less(lhs, nullptr);
      assert::less(nullptr, rhs);
    }

  };

  monitor::run();
}
