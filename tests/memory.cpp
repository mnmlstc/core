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
      using std::swap;
      core::poly_ptr<poly::base> lhs { new poly::derived { } };
      core::poly_ptr<poly::base> rhs { };

      assert::is_true(bool(lhs));
      assert::is_true(not rhs);

      swap(lhs, rhs);

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
      core::poly_ptr<poly::base> lhs { };
      core::poly_ptr<poly::base> rhs { };
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

    task("make-poly") = [] {
      poly::derived derived { };
      auto poly = core::make_poly<poly::base>(std::move(derived));
      assert::is_true(bool(poly));
      assert::equal(typeid(derived), typeid(*poly));
      assert::equal(poly->get(), 42);
    }
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

    task("get") = [] {
      core::deep_ptr<poly::derived> value { };
      assert::is_null(value.get());
    },

    task("get-copier") = [] {
      core::deep_ptr<poly::derived> value { new poly::derived { } };
      auto ptr = value.get_copier()(value.get());
      assert::is_not_null(ptr);
      delete ptr;
    },

    task("operator-equal") = [] {
      core::deep_ptr<poly::derived> lhs { };
      core::deep_ptr<poly::derived> rhs { };
      assert::equal(lhs, rhs);
      assert::equal(lhs, nullptr);
      assert::equal(nullptr, rhs);
    },

    task("operator-not-equal") = [] {
      core::deep_ptr<poly::derived> lhs { new poly::derived { } };
      core::deep_ptr<poly::derived> rhs { };
      assert::not_equal(lhs, rhs);
      assert::not_equal(lhs, nullptr);
      assert::not_equal(nullptr, lhs);
    },

    task("operator-greater-than-or-equal") = [] {
      core::deep_ptr<poly::derived> lhs { new poly::derived { } };
      core::deep_ptr<poly::derived> rhs { };

      assert::greater_equal(lhs, rhs);
      assert::greater_equal(lhs, nullptr);
      assert::greater_equal(nullptr, rhs);

    },

    task("operator-less-than-or-equal") = [] {
      core::deep_ptr<poly::derived> lhs { };
      core::deep_ptr<poly::derived> rhs { new poly::derived { } };

      assert::less_equal(lhs, rhs);
      assert::less_equal(lhs, nullptr);
      assert::less_equal(nullptr, lhs);
      assert::less_equal(nullptr, rhs);
    },

    task("operator-greater-than") = [] {
      core::poly_ptr<poly::derived> lhs { new poly::derived { } };
      core::poly_ptr<poly::derived> rhs { };

      assert::greater(lhs, rhs);
      assert::greater(lhs, nullptr);
    },

    task("operator-less-than") = [] {
      core::deep_ptr<poly::derived> lhs { };
      core::deep_ptr<poly::derived> rhs { new poly::derived { } };

      assert::less(lhs, rhs);
      assert::less(nullptr, rhs);
    },

    task("make-deep") = [] {
      auto deep = core::make_deep<std::tuple<std::string, std::string>>(
        "hello",
        "world!"
      );
      assert::is_true(bool(deep));
    }
  };

  test("observer-ptr") = {
    task("default-constructor") = [] {
      core::observer_ptr<int> value { };
      assert::is_true(not value);
    },

    task("value-constructor") = [] {
      int value { 42 };
      core::observer_ptr<int> observer { std::addressof(value) };
      assert::equal(std::addressof(value), observer.get());
    },

    task("copy-constructor") = [] {
      int value { 42 };
      core::observer_ptr<int> observer { std::addressof(value) };
      core::observer_ptr<int> copy { observer };
      assert::equal(copy.get(), std::addressof(value));
      assert::equal(copy.get(), observer.get());
    },

    task("copy-assign") = [] {
      int value { 42 };
      core::observer_ptr<int> observer { std::addressof(value) };
      core::observer_ptr<int> copy { };

      assert::is_null(copy.get());
      copy = observer;
      assert::equal(observer.get(), copy.get());
    },

    task("null-assign") = [] {
      int value { 42 };
      core::observer_ptr<int> observer { std::addressof(value) };
      assert::is_not_null(observer.get());
      observer = nullptr;
      assert::is_null(observer.get());
    },

    task("swap") = [] {
      using std::swap;
      int value_one { 42 };
      int value_two { 43 };

      core::observer_ptr<int> lhs { std::addressof(value_one) };
      core::observer_ptr<int> rhs { std::addressof(value_two) };

      assert::equal(std::addressof(value_one), lhs.get());
      assert::equal(std::addressof(value_two), rhs.get());

      swap(lhs, rhs);

      assert::equal(std::addressof(value_one), rhs.get());
      assert::equal(std::addressof(value_two), lhs.get());
    },

    task("const-pointer-conversion") = [] {
      int value { 42 };
      core::observer_ptr<int> observer { std::addressof(value) };
      auto const ptr = static_cast<int const*>(observer);
      assert::equal(observer.get(), ptr);
    },

    task("pointer-conversion") = [] {
      int value { 42 };
      core::observer_ptr<int> observer { std::addressof(value) };
      auto ptr = static_cast<int*>(observer);
      assert::equal(observer.get(), ptr);
    },

    task("deref-operator") = [] {
      int value { 42 };
      core::observer_ptr<int> observer { std::addressof(value) };
      assert::equal(42, *observer);
    },

    task("arrow-operator") = [] {
      std::string value { "arrow" };
      core::observer_ptr<std::string const> observer { std::addressof(value) };
      assert::equal(observer->size(), 5);
    },

    task("release") = [] {
      int value { 42 };
      core::observer_ptr<int> observer { std::addressof(value) };

      assert::is_not_null(observer.get());

      auto ptr = observer.release();

      assert::is_null(observer.get());
      assert::equal(ptr, std::addressof(value));
    },

    task("reset") = [] {
      int value { 42 };
      int reset { 47 };
      core::observer_ptr<int> observer { std::addressof(value) };
      assert::is_not_null(observer.get());
      assert::equal(std::addressof(value), observer.get());

      observer.reset(std::addressof(reset));
      assert::is_not_null(observer.get());
      assert::equal(std::addressof(reset), observer.get());

      observer.reset();
      assert::is_null(observer.get());
    },

    task("operator-equal") = [] {
      int value { 42 };
      core::observer_ptr<int> lhs { std::addressof(value) };
      core::observer_ptr<int> rhs { std::addressof(value) };
      assert::equal(core::observer_ptr<int> { }, nullptr);
      assert::equal(nullptr, core::observer_ptr<int> { });
      assert::equal(lhs, rhs);
    },

    task("operator-not-equal") = [] {
      int value { 42 };
      int other { };
      core::observer_ptr<int> lhs { std::addressof(value) };
      core::observer_ptr<int> rhs { std::addressof(other) };
      assert::not_equal(lhs, nullptr);
      assert::not_equal(nullptr, rhs);
      assert::not_equal(lhs, rhs);
    },

    task("operator-greater-equal") = [] {
      int value { };
      core::observer_ptr<int> lhs { std::addressof(value) };
      core::observer_ptr<int> rhs1 { lhs };
      core::observer_ptr<int> rhs2 { };
      assert::greater_equal(lhs, rhs1);
      assert::greater_equal(lhs, rhs2);
    },

    task("operator-less-equal") = [] {
      int value { };
      core::observer_ptr<int> lhs { };
      core::observer_ptr<int> rhs1 { };
      core::observer_ptr<int> rhs2 { std::addressof(value) };
      assert::less_equal(lhs, rhs1);
      assert::less_equal(lhs, rhs2);
    },

    task("operator-greater") = [] {
      int value { };
      core::observer_ptr<int> lhs { std::addressof(value) };
      core::observer_ptr<int> rhs { };
      assert::greater(lhs, rhs);
    },

    task("operator-less") = [] {
      int value { };
      core::observer_ptr<int> lhs { };
      core::observer_ptr<int> rhs { std::addressof(value) };
      assert::less(lhs, rhs);
    },

    task("make-observer") = [] {
      std::unique_ptr<int> unique { new int { } };
      std::shared_ptr<int> shared { new int { } };
      core::deep_ptr<int> deep { new int { } };
      std::weak_ptr<int> weak { shared };
      int value { };

      auto unique_observer = core::make_observer(unique);
      auto shared_observer = core::make_observer(shared);
      auto deep_observer = core::make_observer(deep);
      auto weak_observer = core::make_observer(weak);
      auto ptr_observer = core::make_observer(std::addressof(value));

      assert::equal(unique_observer.get(), unique.get());
      assert::equal(shared_observer.get(), shared.get());
      assert::equal(deep_observer.get(), deep.get());
      assert::equal(weak_observer.get(), shared.get());
      assert::equal(ptr_observer.get(), std::addressof(value));
    }
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
      auto unique = core::make_unique<int[]>(7);
      assert::is_not_null(unique.get());
    }
  };

  monitor::run();
}
