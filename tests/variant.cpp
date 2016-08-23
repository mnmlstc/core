#include <core/variant.hpp>

#include <unordered_map>
#include <string>
#include <vector>

#include <cstdint>

#include "catch.hpp"

TEST_CASE("variant-constructors", "[variant][constructors]") {
  SECTION("default") {
    core::variant<std::string, int> variant { };
    CHECK(typeid(std::string) == variant.type());
  }

  SECTION("value") {
    using variant_type = core::variant<std::string, std::uint64_t>;

    variant_type string { "value-constructor" };
    variant_type integer { 64u };

    CHECK(string.type() == typeid(std::string));
    CHECK(integer.type() == typeid(std::uint64_t));

    CHECK(core::get<0>(string) == std::string { "value-constructor" });
    CHECK(core::get<1>(integer) == 64u);
  }

  SECTION("move") {
    using variant_type = core::variant<std::string, double>;
    variant_type variant { std::string { "move" } };
    variant_type move { std::move(variant) };

    CHECK(move.type() == typeid(std::string));
    CHECK(core::get<0>(move) == std::string { "move" });
    CHECK(core::get<0>(variant).empty());
  }

  SECTION("copy") {
    using variant_type = core::variant<std::string, double>;
    variant_type variant { 0.8 };
    variant_type copy { variant };

    CHECK(copy.type() == typeid(double));
    CHECK(variant == copy);
  }
}

TEST_CASE("variant-assignment", "[variant][assignment]") {
  SECTION("value") {
    using variant_type = core::variant<std::string, double>;
    std::string string { "value" };
    double real { 0.6 };

    variant_type first { };
    variant_type second { };

    first = string;

    CHECK(first.type() == typeid(std::string));
    CHECK(core::get<0>(first) == string);

    second = real;

    CHECK(second.type() == typeid(double));
    CHECK(core::get<1>(second) == real);
  }

  SECTION("move") {
    using variant_type = core::variant<std::string, double>;
    variant_type variant { "move" };
    variant_type move { 9.8 };

    move = std::move(variant);

    CHECK(move.type() == typeid(std::string));
    CHECK(core::get<0>(move) == std::string { "move" });
    CHECK(core::get<0>(variant).empty());
  }

  SECTION("copy") {
    using variant_type = core::variant<std::string, double>;
    variant_type variant { 9.8 };
    variant_type copy { };

    copy = variant;

    CHECK(copy.type() == typeid(double));
    CHECK(core::get<1>(copy) == 9.8);
    CHECK(copy == variant);
  }
}

TEST_CASE("variant-methods", "[variant][methods]") {
  SECTION("visit") {
    using variant_type = core::variant<
      std::uint64_t,
      std::string,
      std::vector<int>
    >;

    variant_type vector { std::vector<int> { 1, 2, 3 } };

    struct visitor final {
      std::string operator ()(std::string const& str) const { return str; }
      std::string operator ()(std::uint64_t const& val) const {
        return std::to_string(val);
      }
      std::string operator ()(std::vector<int> const& vec) const {
        auto value = vec.at(0) + vec.at(1) + vec.at(2);
        return std::to_string(value);
      }
    };

    auto str = vector.visit(visitor { });
    CHECK(str == "6");
  }

  SECTION("match") {
    using variant_type = core::variant<
      std::uint64_t,
      std::string,
      std::vector<std::string>
    >;

    variant_type vector { std::vector<std::string> { "1", "2", "3" } };
    variant_type string { std::string { "match" } };
    variant_type integer { };

    CHECK(
      vector.match(
        [](std::vector<std::string> const&) { return true; },
        [](std::uint64_t const&) { return false; },
        [](std::string const&) { return false; }
      )
    );

    CHECK(
      integer.match(
        [](std::vector<std::string> const&) { return false; },
        [](std::uint64_t const&) { return true; },
        [](std::string const&) { return false; }
      )
    );

    CHECK(
      string.match(
        [](std::vector<std::string> const&) { return false; },
        [](std::uint64_t const&) { return false; },
        [](std::string const&) { return true; }
      )
    );
  }

  SECTION("index") {
    using variant_type = core::variant<
      std::uint64_t,
      std::string,
      std::vector<std::string>
    >;

    variant_type vector { std::vector<std::string> { "1", "2", "3" } };
    variant_type string { std::string { "index" } };
    variant_type integer { };

    CHECK(integer.index() == 0u);
    CHECK(string.index() == 1u);
    CHECK(vector.index() == 2u);
  }

  SECTION("empty") {
    core::variant<std::uint64_t, float> variant { };
    CHECK_FALSE(variant.empty());
  }

  SECTION("type") {
    using variant_type = core::variant<
      std::uint64_t,
      std::string,
      std::vector<std::string>
    >;

    variant_type vector { std::vector<std::string> { "1", "2", "3" } };
    variant_type string { std::string { "type" } };
    variant_type integer { };

    CHECK(typeid(std::vector<std::string>) == vector.type());
    CHECK(typeid(std::uint64_t) == integer.type());
    CHECK(typeid(std::string) == string.type());
  }
}

TEST_CASE("variant-functions", "[variant][functions]") {

#ifndef CORE_NO_EXCEPTIONS
  SECTION("get") {
    using variant_type = core::variant<
      std::uint64_t,
      std::string,
      std::vector<std::string>
    >;

    variant_type const vector { std::vector<std::string> { "1", "2", "3" } };
    variant_type string { std::string { "get" } };
    variant_type integer { static_cast<std::uint64_t>(64) };

    CHECK_THROWS_AS(core::get<0>(vector), core::bad_variant_get);
    CHECK(core::get<0>(core::move(integer)) == 64u);
    CHECK(core::get<1>(string) == "get");
    CHECK(core::get<2>(vector)[0] == std::string { "1" });
    CHECK(core::get<2>(vector)[1] == std::string { "2" });
    CHECK(core::get<2>(vector)[2] == std::string { "3" });
  }
#endif /* CORE_NO_EXCEPTIONS */

  SECTION("get-ptr") {
    using variant_type = core::variant<
      std::uint64_t,
      std::string,
      std::vector<std::string>
    >;

    variant_type const vector { std::vector<std::string> { "1", "2", "3" } };
    variant_type string { std::string { "get" } };
    variant_type integer { static_cast<std::uint64_t>(64) };

    CHECK_FALSE(core::get<0>(&vector));
    CHECK(*core::get<0>(&integer) == 64u);
    CHECK(*core::get<1>(&string) == "get");
    CHECK((*core::get<2>(&vector))[0] == std::string { "1" });
    CHECK((*core::get<2>(&vector))[1] == std::string { "2" });
    CHECK((*core::get<2>(&vector))[2] == std::string { "3" });
  }

  SECTION("get-type") {
    using variant_type = core::variant<
      std::uint64_t,
      std::string,
      std::vector<std::string>
    >;

    variant_type const vector { std::vector<std::string> { "1", "2", "3" } };
    variant_type string { std::string { "get" } };
    variant_type integer { static_cast<std::uint64_t>(64) };

    CHECK_THROWS_AS(core::get<0>(vector), core::bad_variant_get);
    CHECK(core::get<std::uint64_t>(core::move(integer)) == 64u);
    CHECK(core::get<std::string>(string) == "get");
    CHECK(core::get<std::vector<std::string>>(vector)[0] == "1");
    CHECK(core::get<std::vector<std::string>>(vector)[1] == "2");
    CHECK(core::get<std::vector<std::string>>(vector)[2] == "3");
  }

  SECTION("swap") {
    using std::swap;
    using variant_type = core::variant<std::string, double>;
    variant_type lhs_str { std::string { "lhs" } };
    variant_type rhs_str { std::string { "rhs" } };
    variant_type lhs { 0.8 };

    swap(lhs_str, rhs_str);
    CHECK(core::get<0>(lhs_str) == std::string { "rhs" });
    CHECK(core::get<0>(rhs_str) == std::string { "lhs" });

    swap(lhs, rhs_str);
    CHECK(lhs.type() == typeid(std::string));
    CHECK(rhs_str.type() == typeid(double));
    CHECK(core::get<0>(lhs) == std::string { "lhs" });
    CHECK(core::get<1>(rhs_str) == 0.8);
  }

  SECTION("hash") {
    using variant_type = core::variant<std::string, double>;
    using unordered_map = std::unordered_map<variant_type, int>;

    unordered_map map;
    map[variant_type { "hash" }] = 9;
    map[variant_type { 9.8 }] = 12;

    CHECK(map[variant_type { "hash" }] == 9);
    CHECK(map[variant_type { 9.8 }] == 12);
  }
}

TEST_CASE("variant-operators", "[variant][operators]") {
  SECTION("equality") {
    using variant_type = core::variant<
      std::string,
      double
    >;

    variant_type lhs { std::string { "equality-comparable" } };
    variant_type rhs { std::string { "equality-comparable" } };
    variant_type frhs { 0.7 };

    CHECK(lhs == rhs);
    CHECK_FALSE(lhs == frhs);
  }

  SECTION("less-than") {
    using variant_type = core::variant<
      std::string,
      double
    >;

    variant_type lhs { 0.2 };
    variant_type rhs { 0.4 };
    variant_type frhs { std::string { "less-than-comaprable" } };

    CHECK(lhs < rhs);
    CHECK_FALSE(lhs < frhs);
  }
}

TEST_CASE("variant-issues", "[variant][issues]") {
  SECTION("issue-23") {
    struct A { };
    struct B { };
    using variant_type = core::variant<A, B>;
    auto foo = [] () -> variant_type { return A { }; };
    auto bar = [] () -> variant_type { return B { }; };
    auto const a = foo();
    auto const b = bar();
    auto const c = a;

    CHECK(a.index() == 0u);
    CHECK(b.index() == 1u);
  }

  SECTION("issue-25") {
    struct A { A (int) { } };
    struct B { B (double) { } };
    using variant_type = core::variant<A, B>;

    variant_type variant { B { 0.0 } };
    CHECK(variant.index() == 1u);
  }
}
