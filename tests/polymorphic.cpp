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

  explicit derived (int value) : value { value } { }

  derived (derived const&) = default;
  derived (derived&&) = default;
  derived () = default;

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

    task("unique-ptr-assignment") = []{
      core::polymorphic<base> poly { };
      std::unique_ptr<derived> ptr { new derived { 56 } };
      poly = std::move(ptr);

      assert::is_true(bool(poly));
      assert::is_true(not ptr);
      assert::equal(typeid(derived), typeid(*poly));
      assert::equal(poly->get(), 56);
    },

    task("raw-ptr-assignment") = []{
      core::polymorphic<base> poly { };
      poly = new derived { };

      assert::is_true(bool(poly));
      assert::equal(typeid(derived), typeid(*poly));
      assert::equal(poly->get(), 42);
    },

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
      struct second_derived : base { };
      core::polymorphic<base> poly { new derived { } };
      auto improper = new second_derived { };
      auto proper = new derived { };

      assert::is_true(bool(poly));
      poly.reset(proper);
      assert::is_true(bool(poly));
      assert::throws<core::bad_polymorphic_reset>([poly, improper] () mutable {
        poly.reset(improper);
      });
      poly.reset();
      assert::is_true(not poly);
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
      assert::equal(nullptr, lhs);
    },

    task("operator-not-equal") = [] {
      core::polymorphic<base> lhs { new derived { } };
      core::polymorphic<base> rhs { };
      assert::not_equal(lhs, rhs);
      assert::not_equal(lhs, nullptr);
      assert::not_equal(nullptr, lhs);
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
      assert::less(nullptr, rhs);
    }

  };

  monitor::run();
}
