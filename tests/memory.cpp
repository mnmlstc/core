#include <core/memory.hpp>

#include "catch.hpp"

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

TEST_CASE("poly-constructors", "[poly][constructors]") {
  SECTION("default") {
    core::poly_ptr<poly::base> value;
    CHECK_FALSE(value);
  }

  SECTION("value") {
    core::poly_ptr<poly::base> value { new poly::derived { } };
    CHECK(value);
    CHECK(typeid(poly::derived) == typeid(*value));
  }

  SECTION("copy") {
    core::poly_ptr<poly::base> value { new poly::derived { } };
    core::poly_ptr<poly::base> copy { value };

    CHECK(value);
    CHECK(copy);

    CHECK(typeid(poly::derived) == typeid(*value));
    CHECK(typeid(poly::derived) == typeid(*copy));

    CHECK((dynamic_cast<poly::derived&>(*copy).value) == 42);
    CHECK(
      dynamic_cast<poly::derived&>(*copy).value ==
      dynamic_cast<poly::derived&>(*value).value
    );
  }

  SECTION("move") {
    core::poly_ptr<poly::base> value { new poly::derived { } };
    core::poly_ptr<poly::base> move { std::move(value) };

    CHECK_FALSE(value);
    CHECK(move);

    CHECK(dynamic_cast<poly::derived&>(*move).value == 42);
  }
}

TEST_CASE("poly-assignment", "[poly][assignment]") {
  SECTION("unique-ptr") {
    core::poly_ptr<poly::base> poly { };
    std::unique_ptr<poly::derived> ptr { new poly::derived { 56 } };
    poly = std::move(ptr);

    CHECK(poly);
    CHECK_FALSE(ptr);
    CHECK(typeid(poly::derived) == typeid(*poly));
    CHECK(poly->get() == 56);
  }

  SECTION("raw-pointer") {
    core::poly_ptr<poly::base> poly { };
    poly = new poly::derived { };

    CHECK(poly);
    CHECK(typeid(poly::derived) == typeid(*poly));
    CHECK(poly->get() == 42);
  }

  SECTION("copy") {
    core::poly_ptr<poly::base> value { new poly::derived { } };
    core::poly_ptr<poly::base> copy { };
    copy = value;

    CHECK(value);
    CHECK(copy);

    CHECK(typeid(poly::derived) == typeid(*value));
    CHECK(typeid(poly::derived) == typeid(*copy));

    CHECK(dynamic_cast<poly::derived&>(*copy).value == 42);
    CHECK(
      dynamic_cast<poly::derived&>(*copy).value ==
      dynamic_cast<poly::derived&>(*value).value
    );
  }

  SECTION("move") {
    core::poly_ptr<poly::base> value { new poly::derived { } };
    core::poly_ptr<poly::base> move { };
    move = std::move(value);

    CHECK_FALSE(value);
    CHECK(move);

    CHECK(dynamic_cast<poly::derived&>(*move).value == 42);
  }
}

TEST_CASE("poly-operators", "[poly][operators]") {
  SECTION("bool") {
    core::poly_ptr<poly::base> value;
    CHECK_FALSE(value);
    value = new poly::derived { };
    CHECK(value);
  }

  SECTION("dereference") {
    core::poly_ptr<poly::base> poly { new poly::derived { } };
    poly::base& parent = *poly;
    CHECK(typeid(parent) == typeid(poly::derived));
  }

  SECTION("arrow") {
    core::poly_ptr<poly::base> poly { new poly::derived { } };
    CHECK(poly->get() == 42);
  }

  SECTION("equal") {
    core::poly_ptr<poly::base> lhs { };
    core::poly_ptr<poly::base> rhs { };
    CHECK(lhs == rhs);
    CHECK(lhs == nullptr);
    CHECK(nullptr == lhs);
  }

  SECTION("not-equal") {
    core::poly_ptr<poly::base> lhs { new poly::derived { } };
    core::poly_ptr<poly::base> rhs { };
    CHECK(lhs != rhs);
    CHECK(lhs != nullptr);
    CHECK(nullptr != lhs);
  }

  SECTION("greater-equal") {
    core::poly_ptr<poly::base> lhs { new poly::derived { } };
    core::poly_ptr<poly::base> rhs { };

    CHECK(lhs >= rhs);
    CHECK(lhs >= nullptr);
    CHECK(nullptr >= rhs);
  }

  SECTION("less-equal") {
    core::poly_ptr<poly::base> lhs { };
    core::poly_ptr<poly::base> rhs { new poly::derived { } };

    CHECK(lhs <= rhs);
    CHECK(lhs <= nullptr);
    CHECK(nullptr <= lhs);
    CHECK(nullptr <= rhs);
  }

  SECTION("greater") {
    core::poly_ptr<poly::base> lhs { new poly::derived { } };
    core::poly_ptr<poly::base> rhs { };

    CHECK(lhs > rhs);
    CHECK(lhs > nullptr);
  }

  SECTION("less") {
    core::poly_ptr<poly::base> lhs { };
    core::poly_ptr<poly::base> rhs { new poly::derived { } };

    CHECK(lhs < rhs);
    CHECK(nullptr < rhs);
  }
}

TEST_CASE("poly-methods", "[poly][methods]") {
  SECTION("release") {
    core::poly_ptr<poly::base> poly { new poly::derived { } };
    CHECK(poly);
    poly.release();
    CHECK_FALSE(poly);
  }

#ifndef CORE_NO_EXCEPTIONS
  SECTION("reset") {
    struct second_derived : poly::base { };
    core::poly_ptr<poly::base> poly { new poly::derived { } };
    auto improper = new second_derived { };
    auto proper = new poly::derived { };

    CHECK(poly);
    poly.reset(proper);
    CHECK(poly);
    CHECK_THROWS_AS(poly.reset(improper), core::bad_polymorphic_reset);
    poly.reset();
    CHECK_FALSE(poly);
  }
#endif /* CORE_NO_EXCEPTIONS */

  SECTION("get") {
    core::poly_ptr<poly::base> const poly { new poly::derived { } };
    poly::derived* ptr = dynamic_cast<poly::derived*>(poly.get());
    CHECK(ptr);
  }

  SECTION("get-copier") {
    core::poly_ptr<poly::base> value { new poly::derived { } };
    auto poly_copy = core::default_poly_copy<
      poly::base,
      std::default_delete<poly::base>,
      poly::derived
    >;

    CHECK(value.get_copier() == poly_copy);
  }
}

TEST_CASE("poly-functions", "[poly][functions]") {
  SECTION("make-poly") {
    poly::derived derived { };
    auto poly = core::make_poly<poly::base>(std::move(derived));
    CHECK(poly);
    CHECK(typeid(derived) == typeid(*poly));
    CHECK(poly->get() == 42);
  }

  SECTION("swap") {
    using std::swap;
    core::poly_ptr<poly::base> lhs { new poly::derived { } };
    core::poly_ptr<poly::base> rhs { };

    CHECK(lhs);
    CHECK_FALSE(rhs);

    swap(lhs, rhs);

    CHECK_FALSE(lhs);
    CHECK(rhs);
  }
}

TEST_CASE("deep-constructors", "[deep][constructors]") {
  SECTION("default") {
    core::deep_ptr<poly::derived> value { };
    CHECK_FALSE(value);
  }

  SECTION("value") {
    core::deep_ptr<poly::derived> value { new poly::derived { } };

    CHECK(value);
    CHECK((*value).value == 42);
  }

  SECTION("move") {
    core::deep_ptr<poly::derived> value { new poly::derived { } };
    core::deep_ptr<poly::derived> move { std::move(value) };

    CHECK_FALSE(value);
    CHECK(move);
    CHECK((*move).value == 42);
  }

  SECTION("copy") {
    core::deep_ptr<poly::derived> value { new poly::derived { } };
    core::deep_ptr<poly::derived> copy { value };

    CHECK(value);
    CHECK(copy);

    CHECK(value->get() == copy->get());
    CHECK(value->get() == 42);
    CHECK(copy->get() == 42);
  }
}

TEST_CASE("deep-assignment", "[deep][assignment]") {
  SECTION("unique-ptr") {
    core::deep_ptr<poly::derived> value { };
    std::unique_ptr<poly::derived> unique { new poly::derived { } };

    value = std::move(unique);

    CHECK_FALSE(unique);
    CHECK(value);
    CHECK(value->get() == 42);
  }

  SECTION("copy") {
    core::deep_ptr<poly::derived> value { new poly::derived { } };
    core::deep_ptr<poly::derived> copy { };

    CHECK(value);
    CHECK_FALSE(copy);

    copy = value;

    CHECK(value);
    CHECK(copy);
    CHECK(value->get() == copy->get());
    CHECK(copy->get() == 42);
  }

  SECTION("move") {
    core::deep_ptr<poly::derived> value { new poly::derived { } };
    core::deep_ptr<poly::derived> move { };

    CHECK(value);
    CHECK_FALSE(move);

    move = std::move(value);

    CHECK_FALSE(value);
    CHECK(move);
    CHECK(move->get() == 42);
  }
}

TEST_CASE("deep-operators", "[deep][operators]") {
  SECTION("bool") {
    core::deep_ptr<poly::derived> true_value { new poly::derived { } };
    core::deep_ptr<poly::derived> false_value { };

    CHECK(true_value);
    CHECK_FALSE(false_value);
  }

  SECTION("dereference") {
    core::deep_ptr<poly::derived> value { new poly::derived { } };
    CHECK(value);
    CHECK((*value).value == 42);
  }

  SECTION("arrow") {
    core::deep_ptr<poly::derived> value { new poly::derived { } };
    CHECK(value);
    CHECK(value->get() == 42);
  }

  SECTION("equal") {
    core::deep_ptr<poly::derived> lhs { };
    core::deep_ptr<poly::derived> rhs { };
    CHECK(lhs == rhs);
    CHECK(lhs == nullptr);
    CHECK(nullptr == rhs);
  }

  SECTION("not-equal") {
    core::deep_ptr<poly::derived> lhs { new poly::derived { } };
    core::deep_ptr<poly::derived> rhs { };
    CHECK(lhs != rhs);
    CHECK(lhs != nullptr);
    CHECK(nullptr != lhs);
  }

  SECTION("greater-equal") {
    core::deep_ptr<poly::derived> lhs { new poly::derived { } };
    core::deep_ptr<poly::derived> rhs { };

    CHECK(lhs >= rhs);
    CHECK(lhs >= nullptr);
    CHECK(nullptr >= rhs);
  }

  SECTION("less-equal") {
    core::deep_ptr<poly::derived> lhs { };
    core::deep_ptr<poly::derived> rhs { new poly::derived { } };

    CHECK(lhs <= rhs);
    CHECK(lhs <= nullptr);
    CHECK(nullptr <= lhs);
    CHECK(nullptr <= rhs);
  }

  SECTION("greater") {
    core::poly_ptr<poly::derived> lhs { new poly::derived { } };
    core::poly_ptr<poly::derived> rhs { };

    CHECK(lhs > rhs);
    CHECK(lhs > nullptr);
  }

  SECTION("less") {
    core::deep_ptr<poly::derived> lhs { };
    core::deep_ptr<poly::derived> rhs { new poly::derived { } };

    CHECK(lhs < rhs);
    CHECK(nullptr < rhs);
  }
}

TEST_CASE("deep-methods", "[deep][methods]") {
  SECTION("release") {
    core::deep_ptr<poly::derived> value { new poly::derived { } };

    CHECK(value);

    auto ptr = value.release();

    CHECK_FALSE(value.get());
    CHECK_FALSE(value);
    CHECK(ptr);
    CHECK(ptr->get() == 42);
    delete ptr;
  }

  SECTION("reset") {
    core::deep_ptr<poly::derived> value { new poly::derived { } };
    CHECK(value);
    value.reset();
    CHECK_FALSE(value);
  }

  SECTION("get") {
    core::deep_ptr<poly::derived> value { };
    CHECK_FALSE(value.get());
  }

  SECTION("get-copier") {
    core::deep_ptr<poly::derived> value { new poly::derived { } };
    auto ptr = value.get_copier()(value.get());
    CHECK(ptr);
    delete ptr;
  }
}

TEST_CASE("deep-functions", "[deep][functions]") {
  SECTION("make-deep") {
    auto deep = core::make_deep<std::tuple<std::string, std::string>>(
      "hello",
      "world!"
    );
    CHECK(deep);
  }
}

TEST_CASE("observer-constructors", "[observer][constructors]") {
  SECTION("default") {
    core::observer_ptr<int> value { };
    CHECK_FALSE(value);
  }

  SECTION("value") {
    int value { 42 };
    core::observer_ptr<int> observer { std::addressof(value) };
    CHECK(std::addressof(value) == observer.get());
  }

  SECTION("copy") {
    int value { 42 };
    core::observer_ptr<int> observer { std::addressof(value) };
    core::observer_ptr<int> copy { observer };
    CHECK(copy.get() == std::addressof(value));
    CHECK(copy.get() == observer.get());
  }
}

TEST_CASE("observer-assignment", "[observer][assignment]") {
  SECTION("copy") {
    int value { 42 };
    core::observer_ptr<int> observer { std::addressof(value) };
    core::observer_ptr<int> copy { };

    CHECK_FALSE(copy.get());
    copy = observer;
    CHECK(observer.get() == copy.get());
  }

  SECTION("null") {
    int value { 42 };
    core::observer_ptr<int> observer { std::addressof(value) };
    CHECK(observer.get());
    observer = nullptr;
    CHECK_FALSE(observer.get());
  }
}

TEST_CASE("observer-operators", "[observer][operators]") {
  SECTION("const-pointer") {
    int value { 42 };
    core::observer_ptr<int> observer { std::addressof(value) };
    auto const ptr = static_cast<int const*>(observer);
    CHECK(observer.get() == ptr);
  }

  SECTION("pointer") {
    int value { 42 };
    core::observer_ptr<int> observer { std::addressof(value) };
    auto ptr = static_cast<int*>(observer);
    CHECK(observer.get() == ptr);
  }

  SECTION("dereference") {
    int value { 42 };
    core::observer_ptr<int> observer { std::addressof(value) };
    CHECK(42 == *observer);
  }

  SECTION("arrow") {
    std::string value { "arrow" };
    core::observer_ptr<std::string const> observer { std::addressof(value) };
    CHECK(observer->size() == 5);
  }

  SECTION("equal") {
    int value { 42 };
    core::observer_ptr<int> lhs { std::addressof(value) };
    core::observer_ptr<int> rhs { std::addressof(value) };
    CHECK(core::observer_ptr<int> { } == nullptr);
    CHECK(nullptr == core::observer_ptr<int> { });
    CHECK(lhs == rhs);
  }

  SECTION("not-equal") {
    int value { 42 };
    int other { };
    core::observer_ptr<int> lhs { std::addressof(value) };
    core::observer_ptr<int> rhs { std::addressof(other) };
    CHECK(lhs != nullptr);
    CHECK(nullptr != rhs);
    CHECK(lhs != rhs);
  }

  SECTION("greater-equal") {
    int value { };
    core::observer_ptr<int> lhs { std::addressof(value) };
    core::observer_ptr<int> rhs1 { lhs };
    core::observer_ptr<int> rhs2 { };
    CHECK(lhs >= rhs1);
    CHECK(lhs >= rhs2);
  }

  SECTION("less-equal") {
    int value { };
    core::observer_ptr<int> lhs { };
    core::observer_ptr<int> rhs1 { };
    core::observer_ptr<int> rhs2 { std::addressof(value) };
    CHECK(lhs <= rhs1);
    CHECK(lhs <= rhs2);
  }

  SECTION("greater") {
    int value { };
    core::observer_ptr<int> lhs { std::addressof(value) };
    core::observer_ptr<int> rhs { };
    CHECK(lhs > rhs);
  }

  SECTION("less") {
    int value { };
    core::observer_ptr<int> lhs { };
    core::observer_ptr<int> rhs { std::addressof(value) };
    CHECK(lhs < rhs);
  }
}

TEST_CASE("observer-methods", "[observer][methods]") {
  SECTION("release") {
    int value { 42 };
    core::observer_ptr<int> observer { std::addressof(value) };

    CHECK(observer.get());

    auto ptr = observer.release();

    CHECK_FALSE(observer.get());
    CHECK(ptr == std::addressof(value));
  }

  SECTION("reset") {
    int value { 42 };
    int reset { 47 };
    core::observer_ptr<int> observer { std::addressof(value) };
    CHECK(observer.get());
    CHECK(std::addressof(value) == observer.get());

    observer.reset(std::addressof(reset));
    CHECK(observer.get());
    CHECK(std::addressof(reset) == observer.get());

    observer.reset();
    CHECK_FALSE(observer.get());
  }
}

TEST_CASE("observer-functions", "[observer][functions]") {
  SECTION("swap") {
    using std::swap;
    int value_one { 42 };
    int value_two { 43 };

    core::observer_ptr<int> lhs { std::addressof(value_one) };
    core::observer_ptr<int> rhs { std::addressof(value_two) };

    CHECK(std::addressof(value_one) == lhs.get());
    CHECK(std::addressof(value_two) == rhs.get());

    swap(lhs, rhs);

    CHECK(std::addressof(value_one) == rhs.get());
    CHECK(std::addressof(value_two) == lhs.get());
  }

  SECTION("make-observer") {
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

    CHECK(unique_observer.get() == unique.get());
    CHECK(shared_observer.get() == shared.get());
    CHECK(deep_observer.get() == deep.get());
    CHECK(weak_observer.get() == shared.get());
    CHECK(ptr_observer.get() == std::addressof(value));
  }

  SECTION("make-unique-single") {
    auto unique = core::make_unique<std::vector<int>>(
      std::vector<int> { 1, 2, 3, 4, 5}
    );
    CHECK(unique.get());
    CHECK((*unique)[0] == 1);
    CHECK((*unique)[1] == 2);
    CHECK((*unique)[2] == 3);
    CHECK((*unique)[3] == 4);
    CHECK((*unique)[4] == 5);
  }

  SECTION("make-unique-array") {
    auto unique = core::make_unique<int[]>(7);
    CHECK(unique.get());
  }
}
