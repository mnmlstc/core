#include <core/memory.hpp>

#include <unittest/unittest.hpp>

namespace poly {

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

} /* namespace poly */

int main () {
  using namespace unittest;

  test("poly-ptr") = {
    task("default-constructor") = []{
      core::poly_ptr<poly::base> value;
      assert::is_true(not value);
    },

    task("value-constructor") = [] {
      core::poly_ptr<poly::base> value { new poly::derived { } };
      assert::is_false(not value);
      assert::equal(typeid(poly::derived), typeid(*value));
    },

    task("copy-constructor") = [] {
      core::poly_ptr<poly::base> value { new poly::derived { } };
      core::poly_ptr<poly::base> copy { value };

      assert::is_false(not value);
      assert::is_false(not copy);

      assert::equal(typeid(poly::derived), typeid(*value));
      assert::equal(typeid(poly::derived), typeid(*copy));

      assert::equal(dynamic_cast<poly::derived&>(*copy).value, 42);
      assert::equal(
        dynamic_cast<poly::derived&>(*copy).value,
        dynamic_cast<poly::derived&>(*value).value
      );
    },

    task("move-constructor") = []{
      core::poly_ptr<poly::base> value { new poly::derived { } };
      core::poly_ptr<poly::base> move { std::move(value) };

      assert::is_true(not value);
      assert::is_true(bool(move));

      assert::equal(dynamic_cast<poly::derived&>(*move).value, 42);
    },

    task("unique-ptr-assignment") = []{
      core::poly_ptr<poly::base> poly { };
      std::unique_ptr<poly::derived> ptr { new poly::derived { 56 } };
      poly = std::move(ptr);

      assert::is_true(bool(poly));
      assert::is_true(not ptr);
      assert::equal(typeid(poly::derived), typeid(*poly));
      assert::equal(poly->get(), 56);
    },

    task("raw-ptr-assignment") = []{
      core::poly_ptr<poly::base> poly { };
      poly = new poly::derived { };

      assert::is_true(bool(poly));
      assert::equal(typeid(poly::derived), typeid(*poly));
      assert::equal(poly->get(), 42);
    },

    task("copy-assignment") = []{
      core::poly_ptr<poly::base> value { new poly::derived { } };
      core::poly_ptr<poly::base> copy { };
      copy = value;

      assert::is_true(bool(value));
      assert::is_true(bool(copy));

      assert::equal(typeid(poly::derived), typeid(*value));
      assert::equal(typeid(poly::derived), typeid(*copy));

      assert::equal(dynamic_cast<poly::derived&>(*copy).value, 42);
      assert::equal(
        dynamic_cast<poly::derived&>(*copy).value,
        dynamic_cast<poly::derived&>(*value).value
      );
    },

    task("move-assignment") = []{
      core::poly_ptr<poly::base> value { new poly::derived { } };
      core::poly_ptr<poly::base> move { };
      move = std::move(value);

      assert::is_true(not value);
      assert::is_true(bool(move));

      assert::equal(dynamic_cast<poly::derived&>(*move).value, 42);
    },

    task("operator-bool") = [] {
      core::poly_ptr<poly::base> value;
      assert::is_true(not value);
      value = new poly::derived { };
      assert::is_true(bool(value));
    },

    task("dereference-operator") = [] {
      core::poly_ptr<poly::base> poly { new poly::derived { } };
      poly::base& parent = *poly;
      assert::equal(typeid(parent), typeid(poly::derived));
    },

    task("arrow-operator") = [] {
      core::poly_ptr<poly::base> poly { new poly::derived { } };
      assert::equal(poly->get(), 42);
    },

    task("release") = [] {
      core::poly_ptr<poly::base> poly { new poly::derived { } };
      assert::is_true(bool(poly));
      poly.release();
      assert::is_true(not poly);
    },

    task("reset") = [] {
      struct second_derived : poly::base { };
      core::poly_ptr<poly::base> poly { new poly::derived { } };
      auto improper = new second_derived { };
      auto proper = new poly::derived { };

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
      core::poly_ptr<poly::base> lhs { new poly::derived { } };
      core::poly_ptr<poly::base> rhs { };

      assert::is_true(bool(lhs));
      assert::is_true(not rhs);

      std::swap(lhs, rhs);

      assert::is_true(not lhs);
      assert::is_true(bool(rhs));
    },

    task("get") = [] {
      core::poly_ptr<poly::base> const poly { new poly::derived { } };
      poly::derived* ptr = dynamic_cast<poly::derived*>(poly.get());
      assert::is_not_null(ptr);
    },

    task("get-copier") = [] {
      core::poly_ptr<poly::base> value { new poly::derived { } };
      auto poly_copy = core::default_poly_copy<
        poly::base,
        std::default_delete<poly::base>,
        poly::derived
      >;

      assert::equal(value.get_copier(), poly_copy);
    },

    task("operator-equal") = [] {
      core::poly_ptr<poly::base> lhs, rhs;
      core::poly_ptr<poly::derived> der;
      assert::equal(lhs, rhs);
      assert::equal(lhs, nullptr);
      assert::equal(nullptr, lhs);
    },

    task("operator-not-equal") = [] {
      core::poly_ptr<poly::base> lhs { new poly::derived { } };
      core::poly_ptr<poly::base> rhs { };
      assert::not_equal(lhs, rhs);
      assert::not_equal(lhs, nullptr);
      assert::not_equal(nullptr, lhs);
    },

    task("operator-greater-than-or-equal") = [] {
      core::poly_ptr<poly::base> lhs { new poly::derived { } };
      core::poly_ptr<poly::base> rhs { };

      assert::greater_equal(lhs, rhs);
      assert::greater_equal(lhs, nullptr);
      assert::greater_equal(nullptr, rhs);
    },

    task("operator-less-than-or-equal") = [] {
      core::poly_ptr<poly::base> lhs { };
      core::poly_ptr<poly::base> rhs { new poly::derived { } };

      assert::less_equal(lhs, rhs);
      assert::less_equal(lhs, nullptr);
      assert::less_equal(nullptr, lhs);
      assert::less_equal(nullptr, rhs);
    },

    task("operator-greater-than") = [] {
      core::poly_ptr<poly::base> lhs { new poly::derived { } };
      core::poly_ptr<poly::base> rhs { };

      assert::greater(lhs, rhs);
      assert::greater(lhs, nullptr);
    },

    task("operator-less-than") = [] {
      core::poly_ptr<poly::base> lhs { };
      core::poly_ptr<poly::base> rhs { new poly::derived { } };

      assert::less(lhs, rhs);
      assert::less(nullptr, rhs);
    },

    task("make-poly") = [] { assert::fail(); }
  };

  test("deep-ptr") = {
    task("default-constructor") = [] {
      core::deep_ptr<poly::derived> value { };
      assert::is_true(not value);
    },

    task("value-constructor") = [] {
      core::deep_ptr<poly::derived> value { new poly::derived { } };

      assert::is_true(bool(value));
      assert::equal((*value).value, 42);
    },

    task("move-constructor") = [] {
      core::deep_ptr<poly::derived> value { new poly::derived { } };
      core::deep_ptr<poly::derived> move { std::move(value) };

      assert::is_true(not value);
      assert::is_true(bool(move));
      assert::equal((*move).value, 42);
    },

    task("copy-constructor") = [] {
      core::deep_ptr<poly::derived> value { new poly::derived { } };
      core::deep_ptr<poly::derived> copy { value };

      assert::is_true(bool(value));
      assert::is_true(bool(copy));

      assert::equal(value->get(), copy->get());
      assert::equal(value->get(), 42);
      assert::equal(copy->get(), 42);
    },

    task("unique-ptr-assign-operator") = [] {
      core::deep_ptr<poly::derived> value { };
      std::unique_ptr<poly::derived> unique { new poly::derived { } };

      value = std::move(unique);

      assert::is_true(not unique);
      assert::is_true(bool(value));
      assert::equal(value->get(), 42);
    },

    task("copy-assignment") = []{
      core::deep_ptr<poly::derived> value { new poly::derived { } };
      core::deep_ptr<poly::derived> copy { };

      assert::is_true(bool(value));
      assert::is_true(not copy);

      copy = value;

      assert::is_true(bool(value));
      assert::is_true(bool(copy));
      assert::equal(value->get(), copy->get());
      assert::equal(copy->get(), 42);
    },

    task("move-assignment") = []{
      core::deep_ptr<poly::derived> value { new poly::derived { } };
      core::deep_ptr<poly::derived> move { };

      assert::is_true(bool(value));
      assert::is_true(not move);

      move = std::move(value);

      assert::is_true(not value);
      assert::is_true(bool(move));
      assert::equal(move->get(), 42);
    },

    task("operator-bool") = []{
      core::deep_ptr<poly::derived> true_value { new poly::derived { } };
      core::deep_ptr<poly::derived> false_value { };

      assert::is_true(bool(true_value));
      assert::is_true(not false_value);
    },

    task("dereference-operator") = [] {
      core::deep_ptr<poly::derived> value { new poly::derived { } };
      assert::is_true(bool(value));
      assert::equal((*value).value, 42);
    },

    task("arrow-operator") = [] {
      core::deep_ptr<poly::derived> value { new poly::derived { } };
      assert::is_true(bool(value));
      assert::equal(value->get(), 42);
    },

    task("release") = []{
      core::deep_ptr<poly::derived> value { new poly::derived { } };

      assert::is_true(bool(value));

      auto ptr = value.release();

      assert::is_null(value.get());
      assert::is_true(not value);
      assert::is_not_null(ptr);
      assert::equal(ptr->get(), 42);
      delete ptr;
    },

    task("reset") = [] {
      core::deep_ptr<poly::derived> value { new poly::derived { } };
      assert::is_true(bool(value));
      value.reset();
      assert::is_true(not value);
    },

    task("get") = [] { assert::fail(); },

    task("get-copier") = [] { assert::fail(); },
    task("operator-equal") = [] { assert::fail(); },
    task("operator-not-equal") = [] { assert::fail(); },
    task("operator-greater-than-or-equal") = [] { assert::fail(); },
    task("operator-less-than-or-equal") = [] { assert::fail(); },
    task("operator-greater-than") = [] { assert::fail(); },
    task("operator-less-than") = [] { assert::fail(); },
    task("make-deep") = [] { assert::fail(); }
  };

  test("make-unique") = {
    task("make-unique-single") = []{
      auto unique = core::make_unique<std::vector<int>>(
        std::vector<int> { 1, 2, 3, 4, 5}
      );
      assert::is_not_null(unique.get());
      assert::equal((*unique)[0], 1);
      assert::equal((*unique)[1], 2);
      assert::equal((*unique)[2], 3);
      assert::equal((*unique)[3], 4);
      assert::equal((*unique)[4], 5);
    },

    task("make-unique-array") = []{
      auto unique = core::make_unique<int>(7);
      assert::is_not_null(unique.get());
    }
  };

  monitor::run();
}
