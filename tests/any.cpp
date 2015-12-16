#include <core/any.hpp>
#include <type_traits>
#include <string>
#include <vector>

#include <cstdint>

#include "catch.hpp"

TEST_CASE("constructor", "[constructors]") {
  core::any value;

  CHECK(value.empty());
  CHECK(value.type() == typeid(void));
}

TEST_CASE("constructor-value", "[constructors]") {
  std::vector<char> text { 'v', 'a', 'l', 'u', 'e' };
  core::any copy { text };

  CHECK_FALSE(copy.empty());
  CHECK_FALSE(text.empty());
  CHECK(copy.type() == typeid(text));

  core::any move { std::move(text) };

  CHECK_FALSE(move.empty());
  CHECK(text.empty());
  CHECK(move.type() == typeid(text));
}

TEST_CASE("constructor-copy", "[constructors]") {
  core::any value { std::string { "copy-constructor" } };
  core::any ctor { value };

  CHECK_FALSE(value.empty());
  CHECK_FALSE(ctor.empty());
  CHECK(ctor.type() == value.type());
}

TEST_CASE("constructor-move", "[constructors]") {
  core::any value { std::string { "move-constructor" } };
  core::any ctor { std::move(value) };

  CHECK_FALSE(ctor.empty());
  CHECK(ctor.type() == typeid(std::string));
}

TEST_CASE("assignment", "[assignment]") {
  SECTION("rvalue") {
    std::string text { "rvalue-assign works!" };
    core::any value;
    value = std::move(text);

    CHECK_FALSE(value.empty());
    CHECK(text.empty());
    CHECK(value.type() == typeid(text));
  }

  SECTION("lvalue") {
    std::uint64_t integer = 42;
    core::any value;
    value = integer;
    CHECK_FALSE(value.empty());
    CHECK(value.type() == typeid(integer));
  }

  SECTION("copy") {
    std::uint64_t integer = 42;
    core::any value { integer };
    core::any assigned;
    assigned = value;
    CHECK_FALSE(value.empty());
    CHECK_FALSE(assigned.empty());
    CHECK(typeid(integer) == assigned.type());
  }

  SECTION("move") {
    std::uint64_t integer = 42;
    core::any value { integer };
    core::any assigned;
    assigned = std::move(value);
    CHECK_FALSE(assigned.empty());
    CHECK(typeid(integer) == assigned.type());
  }

  SECTION("value") {
    std::uint64_t integer { 42 };
    core::any value { };
    value = integer;
    CHECK_FALSE(value.empty());
    CHECK(typeid(integer) == value.type());
    CHECK(core::any_cast<std::uint64_t>(value) == 42u);
  }
}

TEST_CASE("swap", "[modifiers]") {
  using std::swap;
  std::uint64_t integer = 42;
  core::any value { integer };
  core::any to_swap { };
  swap(value, to_swap);
  CHECK(value.empty());
  CHECK_FALSE(to_swap.empty());
}

TEST_CASE("clear", "[modifiers]") {
  core::any value { std::string { "clear" } };
  CHECK_FALSE(value.empty());
  value.clear();
  CHECK(value.empty());
}

TEST_CASE("type", "[modifiers]") {
  std::uint64_t integer = 42;
  core::any value { integer };
  CHECK(typeid(integer) == value.type());
}

TEST_CASE("any-cast", "[any-cast]") {

#ifndef CORE_NO_EXCEPTIONS
  SECTION("const-reference") {
    std::uint64_t integer = 42;
    core::any const value { integer };
    auto integer_value = core::any_cast<std::uint64_t>(value);

    CHECK(integer_value == integer);
    CHECK_THROWS_AS(core::any_cast<double>(value), core::bad_any_cast);
  }
#endif /* CORE_NO_EXCEPTIONS */

  SECTION("const-pointer") {
    std::uint64_t integer = 42;
    core::any const value { integer };
    auto integer_ptr = core::any_cast<std::uint64_t>(std::addressof(value));
    auto double_ptr = core::any_cast<double>(std::addressof(value));

    REQUIRE(integer_ptr != nullptr);
    CHECK(double_ptr == nullptr);
    CHECK(*integer_ptr == integer);
    CHECK(std::is_pointer<decltype(integer_ptr)>::value);
  }

#ifndef CORE_NO_EXCEPTIONS
  SECTION("reference") {
    std::uint64_t integer = 42;
    core::any value { integer };
    auto integer_value = core::any_cast<std::uint64_t>(value);

    CHECK(integer_value == integer);
    CHECK_THROWS_AS(core::any_cast<double>(value), core::bad_any_cast);
  }
#endif /* CORE_NO_EXCEPTIONS */

  SECTION("pointer") {
    std::uint64_t integer = 42;
    core::any value { integer };
    auto integer_ptr = core::any_cast<std::uint64_t>(std::addressof(value));
    auto double_ptr = core::any_cast<double>(std::addressof(value));

    REQUIRE(integer_ptr != nullptr);
    CHECK(double_ptr == nullptr);
    CHECK(*integer_ptr == integer);
  }
}
