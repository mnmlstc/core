#include <core/optional.hpp>

#include <unordered_map>
#include <utility>
#include <string>
#include <array>

#include <cstdint>

#include "catch.hpp"

#if defined(_MSC_VER)
  #pragma warning(disable:4702)
#endif /* defined(_MSC_VER) */

namespace Catch {

template <> struct StringMaker<std::error_condition> {
  static std::string convert (std::error_condition const& value) {
    std::ostringstream stream;
    stream << value.category().name() << ':' << value.value();
    return stream.str();
  }
};

template <class T> struct StringMaker<core::result<T>> {
  static std::string convert (core::result<T> const& value) {
    if (value) { return Catch::toString(*value); }
    return StringMaker<std::error_condition>::convert(value.condition());
  }
};

template <> struct StringMaker<core::result<void>> {
  static std::string convert (core::result<void> const& value) {
    if (value) { return "result<void>"; }
    return StringMaker<std::error_condition>::convert(value.condition());
  }
};

} /* namespace Catch */

struct variadic {
  template <class... Args> constexpr variadic (Args&&...) { }
  template <class T, class... Args>
  constexpr variadic (std::initializer_list<T>, Args&&...) { }
};

TEST_CASE("optional-constructors", "[optional][constructors]") {
  SECTION("default") {
    core::optional<int> opt;
    CHECK_FALSE(opt);
    CHECK(opt == core::nullopt);
  }

  SECTION("copy") {
    core::optional<int> opt { 4 };
    core::optional<int> copy { opt };
    CHECK(opt);
    CHECK(copy);
    CHECK(opt == copy);
    CHECK(copy.value() == 4);
  }

  SECTION("move") {
    core::optional<int> opt { 4 };
    core::optional<int> move { std::move(opt) };
    CHECK(opt); // should be in a valid, but null state
    CHECK(move);
    CHECK(move.value() == 4);
  }

  SECTION("null") {
    core::optional<int> opt { core::nullopt };
    CHECK_FALSE(opt);
  }

  SECTION("copy-value") {
    std::string text { "copy-value" };
    core::optional<std::string> opt { text };

    CHECK_FALSE(text.empty());
    CHECK(opt);
    CHECK(opt.value() == text);
    CHECK(opt.value() == "copy-value");
  }

  SECTION("move-value") {
    std::string text { "move-value" };
    core::optional<std::string> opt { std::move(text) };
    CHECK(opt);
    CHECK(text.empty());
    CHECK(opt.value() == "move-value");
  }

  SECTION("in-place") {
    core::optional<std::string> opt1 { core::in_place, "in-place" };
    core::optional<std::string> opt2 { core::in_place,
      { 'i', 'n', '-', 'p', 'l', 'a', 'c', 'e' }
    };
    REQUIRE(opt1);
    REQUIRE(opt2);
    CHECK(*opt1 == "in-place");
    CHECK(*opt2 == "in-place");
    CHECK(opt1 == opt2);
  }
}

TEST_CASE("optional-assignment", "[optional][assignment]") {
  SECTION("nullopt") {
    core::optional<int> opt { 4 };
    CHECK(opt);
    opt = core::nullopt;
    CHECK_FALSE(opt);
  }

  SECTION("copy") {
    core::optional<int> opt { 4 };
    core::optional<int> copy { };
    copy = opt;

    REQUIRE(opt);
    REQUIRE(copy);
    CHECK(opt == copy);
    CHECK(*copy == 4);
  }

  SECTION("move") {
    core::optional<int> opt { 4 };
    core::optional<int> move { };
    move = std::move(opt);

    CHECK(opt); // in a valid, but null state
    CHECK(move);
    CHECK(*move == 4);
  }

  SECTION("copy-value") {
    core::optional<int> opt { };
    int value = 4;
    opt = value;
    CHECK(opt);
    CHECK(*opt == 4);
  }

  SECTION("move-value") {
    core::optional<std::string> opt { };
    std::string value { "move-value" };
    opt = std::move(value);
    CHECK(value.empty());
    CHECK(opt);
    CHECK(*opt == "move-value");
  }
}

TEST_CASE("optional-operators", "[optional][operators]") {
  SECTION("arrow") {
    core::optional<std::string> opt { "arrow" };
    CHECK(opt);
    CHECK_FALSE(opt->empty());
  }

  SECTION("dereference") {
    core::optional<int> opt { 56 };
    CHECK(opt);
    CHECK(*opt == 56);
  }

  SECTION("equal") {
    core::optional<int> lhs_int { };
    core::optional<int> rhs_int { };

    rhs_int.emplace(7);

    CHECK(core::nullopt != rhs_int);
    CHECK(lhs_int == decltype(rhs_int) { });
    CHECK(lhs_int == core::nullopt);
    CHECK(rhs_int == 7);
    CHECK(7 == rhs_int);
  }

  SECTION("less-than") {
    core::optional<int> empty_lhs { };
    core::optional<int> empty_rhs { };
    core::optional<int> lhs { 4 };
    core::optional<int> rhs { 8 };

    CHECK_FALSE(empty_lhs < empty_rhs);
    CHECK(empty_lhs < rhs);
    CHECK(lhs < rhs);

    CHECK(empty_lhs < core::nullopt);
    CHECK(core::nullopt < rhs);
    CHECK(lhs < 5);
  }
}

TEST_CASE("optional-methods", "[optional][methods]") {

#ifndef CORE_NO_EXCEPTIONS
  SECTION("value") {
    core::optional<int> opt { };
    CHECK_THROWS_AS(opt.value(), core::bad_optional_access);
  }
#endif /* CORE_NO_EXCEPTIONS */

  SECTION("value-or") {
    core::optional<std::string> opt { };
    core::optional<std::string> val { "value-or" };

    auto first = opt.value_or("value-or");
    auto second = val.value_or("not-value");

    CHECK(first == "value-or");
    CHECK(second =="value-or");
  }

  SECTION("emplace") {
    core::optional<int> opt { };
    opt.emplace(4);
    CHECK(opt);
    CHECK(*opt == 4);
  }
}

TEST_CASE("optional-functions", "[optional][functions]") {
  SECTION("make-optional") {
    auto opt = core::make_optional<std::string>("make-optional");
    CHECK(opt);
    CHECK(*opt == "make-optional");
  }

  SECTION("swap") {
    using std::swap;
    core::optional<int> lhs { 4 };
    core::optional<int> rhs { };
    core::optional<int> rhs2 { 4 };

    swap(lhs, rhs);

    CHECK_FALSE(lhs);
    CHECK(rhs);
    CHECK(rhs == 4);
    CHECK(rhs == rhs2);

    swap(lhs, rhs);

    CHECK_FALSE(rhs);
    CHECK(lhs);
    CHECK(lhs == 4);
    CHECK(lhs == rhs2);

  }

  SECTION("hash") {
    std::unordered_map<core::optional<std::string>, int> values = {
      std::make_pair(core::make_optional<std::string>("text1"), 0),
      std::make_pair(core::make_optional<std::string>("text2"), 1),
      std::make_pair(core::make_optional<std::string>("text3"), 2),
    };

    CHECK(values[core::make_optional<std::string>("text1")] == 0);
    CHECK(values[core::make_optional<std::string>("text2")] == 1);
    CHECK(values[core::make_optional<std::string>("text3")] == 2);
  }
}

TEST_CASE("optional-misc", "[optional][misc]") {
  SECTION("force-disengage") {
    core::optional<int> value { 5 };
    CHECK(value);
    value = {};
    CHECK_FALSE(value);
  }
}

TEST_CASE("optional-issues", "[optional][issues]") {
  SECTION("issue-23") {
    struct A { };
    auto foo = [] () -> core::optional<A> { return core::nullopt; };
    auto bar = [] () -> core::optional<A> { return A { }; };
    auto const a = foo();
    auto const b = bar();
    auto const c = b;

    CHECK_FALSE(a);
    CHECK(b);
    CHECK(c);
  }
}

TEST_CASE("constexpr-constructors", "[constexpr][constructors]") {
  SECTION("default") {
    constexpr core::optional<int> opt { };
    constexpr bool value = static_cast<bool>(opt);
    static_assert(not value, "");
  }

  SECTION("nullopt") {
    constexpr core::optional<int> opt { core::nullopt };
    constexpr bool value = static_cast<bool>(opt);
    static_assert(not value, "");
  }

  SECTION("value") {
    constexpr int value = 7;
    constexpr core::optional<int> copy { value };
    constexpr core::optional<int> move { 5 };
    static_assert(copy, "could not initialize core::optional<int>");
    static_assert(move, "could not initialize core::optional<int>");
    constexpr bool copy_result = *copy == value;
    constexpr bool move_result = *move == 5;
    static_assert(copy_result, "");
    static_assert(move_result, "");
  }

  SECTION("variadic-value") {
    constexpr core::optional<variadic> list { core::in_place, { 1, 2, 3, 4 } };
    constexpr core::optional<variadic> nolist { core::in_place, 1, "words" };
    constexpr bool list_initd = static_cast<bool>(list);
    constexpr bool nolist_initd = static_cast<bool>(nolist);
    static_assert(list_initd, "");
    static_assert(nolist_initd, "");
  }
}

TEST_CASE("constexpr-methods", "[constexpr][methods]") {
  SECTION("value") {
    constexpr core::optional<int> opt { 5 };
    constexpr int value { opt.value() };
    static_assert(value == 5, "");
  }

  SECTION("value-or") {
    constexpr core::optional<int> engaged { 5 };
    constexpr core::optional<int> disengaged { };
    constexpr int value_or_e = engaged.value_or(7);
    constexpr int value_or_d = disengaged.value_or(7);
    static_assert(value_or_e == 5, "");
    static_assert(value_or_d == 7, "");
  }
}

TEST_CASE("constexpr-operators", "[constexpr][operators]") {
  SECTION("operator-star") {
    constexpr core::optional<int> lhs { 5 };
    constexpr int value { *lhs };
    static_assert(value == 5, "");
  }

  SECTION("equal") {
    constexpr core::optional<int> lhs { 4 };
    constexpr core::optional<int> rhs { 4 };
    constexpr int value { 4 };

    constexpr bool first_result = lhs == rhs;
    constexpr bool second_result = lhs == value;
    constexpr bool third_result = value == rhs;
    constexpr bool fourth_result = core::optional<int> { } == core::nullopt;
    constexpr bool fifth_result = core::nullopt == core::optional<int> { };

    static_assert(first_result, "");
    static_assert(second_result, "");
    static_assert(third_result, "");
    static_assert(fourth_result, "");
    static_assert(fifth_result, "");
  }

  SECTION("not-equal") {
    constexpr core::optional<int> lhs { 4 };
    constexpr core::optional<int> rhs { 3 };
    constexpr int value { 2 };

    constexpr bool first_result = lhs != rhs;
    constexpr bool second_result = lhs != value;
    constexpr bool third_result = value != rhs;
    constexpr bool fourth_result = lhs != core::nullopt;
    constexpr bool fifth_result = core::nullopt != rhs;

    static_assert(first_result, "");
    static_assert(second_result, "");
    static_assert(third_result, "");
    static_assert(fourth_result, "");
    static_assert(fifth_result, "");
  }

  SECTION("greater-equal") {
    constexpr core::optional<int> lhs { 4 };
    constexpr core::optional<int> rhs { 3 };
    constexpr int value { 3 };

    constexpr bool first_result = lhs >= rhs;
    constexpr bool second_result = lhs >= value;
    constexpr bool third_result = value >= rhs;
    constexpr bool fourth_result = lhs >= core::nullopt;
    constexpr bool fifth_result = core::nullopt >= core::optional<int> { };

    static_assert(first_result, "");
    static_assert(second_result, "");
    static_assert(third_result, "");
    static_assert(fourth_result, "");
    static_assert(fifth_result, "");
  }

  SECTION("less-equal") {
    constexpr core::optional<int> lhs { 3 };
    constexpr core::optional<int> rhs { 3 };
    constexpr int rhs_value { 4 };
    constexpr int lhs_value { 2 };

    constexpr bool first_result = lhs <= rhs;
    constexpr bool second_result = lhs <= rhs_value;
    constexpr bool third_result = lhs_value <= rhs;
    constexpr bool fourth_result = core::optional<int> { } <= core::nullopt;
    constexpr bool fifth_result = core::nullopt <= rhs;

    static_assert(first_result, "");
    static_assert(second_result, "");
    static_assert(third_result, "");
    static_assert(fourth_result, "");
    static_assert(fifth_result, "");
  }

  SECTION("greater") {
    constexpr core::optional<int> lhs { 3 };
    constexpr core::optional<int> rhs { 2 };
    constexpr int rhs_value { 2 };
    constexpr int lhs_value { 3 };

    constexpr bool first_result = lhs > rhs;
    constexpr bool second_result = lhs > rhs_value;
    constexpr bool third_result = lhs_value > rhs;
    constexpr bool fourth_result = lhs > core::nullopt;
    constexpr bool fifth_result = core::nullopt > core::optional<int> { };

    static_assert(first_result, "");
    static_assert(second_result, "");
    static_assert(third_result, "");
    static_assert(fourth_result, "");
    static_assert(not fifth_result, "");
  }

  SECTION("less") {
    constexpr core::optional<int> lhs { 3 };
    constexpr core::optional<int> rhs { 4 };
    constexpr int rhs_value { 4 };
    constexpr int lhs_value { 3 };

    constexpr bool first_result = lhs < rhs;
    constexpr bool second_result = lhs < rhs_value;
    constexpr bool third_result = lhs_value < rhs;
    constexpr bool fourth_result = core::optional<int> { } < core::nullopt;
    constexpr bool fifth_result = core::nullopt < rhs;

    static_assert(first_result, "");
    static_assert(second_result, "");
    static_assert(third_result, "");
    static_assert(fourth_result, "");
    static_assert(fifth_result, "");
  }
}

#ifndef CORE_NO_EXCEPTIONS
TEST_CASE("expected-constructors", "[expected][constructors]") {
  SECTION("copy-value") {
    auto value = 53;
    core::expected<int> copy { value };

    CHECK(copy);
    CHECK(copy.value() == value);
  }

  SECTION("move-value") {
    std::string value { "move-value" };
    core::expected<std::string> move { std::move(value) };

    CHECK(move);
    CHECK(value.empty());

    CHECK(move.value() == "move-value");
  }

  SECTION("copy") {
    core::expected<int> value { 5 };
    core::expected<int> copy { value };

    CHECK(value);
    CHECK(copy);

    CHECK(value.value() == copy.value());
    CHECK(copy.value() == 5);
  }

  SECTION("move") {
    core::expected<std::string> value { "move" };
    core::expected<std::string> move { std::move(value) };

    CHECK(value);
    CHECK(move);

    CHECK_FALSE(move.value().empty());
    CHECK(value.value().empty());
    CHECK(move.value() == "move");
  }
}

TEST_CASE("expected-constructor-exception-ptr", "[expected][constructor]") {
  auto ptr = std::make_exception_ptr(std::logic_error { "test" });
  core::expected<int> value { ptr };

  CHECK_FALSE(value);
  auto exception = value.expect<std::logic_error>();
  CHECK(std::string { exception.what() } == "test");
}

TEST_CASE("expected-assignment-copy-value", "[expected][assignment]") {
  core::expected<int> value { };
  value = 5;
  CHECK(value);
  CHECK(value.value() == 5);
}

TEST_CASE("expected-assignment-move-value", "[expected][assignment]") {
  core::expected<std::string> value { };
  std::string str { "move" };
  value = std::move(str);
  CHECK(value);
  CHECK(str.empty());
  CHECK(value.value() == "move");
}

TEST_CASE("expected-assignment-copy", "[expected][assignment]") {
  core::expected<std::string> value { "copy" };
  core::expected<std::string> copy { };
  copy = value;

  CHECK(value);
  CHECK(copy);
  CHECK(copy.value() == value.value());

  CHECK(copy.value() == "copy");
}

TEST_CASE("expected-move-assignment", "[expected][assignment]") {
  core::expected<std::string> value { "move" };
  core::expected<std::string> move { };
  move = std::move(value);

  CHECK(value);
  CHECK(move);
  CHECK(value.value().empty());

  CHECK(move.value() == "move");
}

TEST_CASE("expected-assignment-exception-ptr", "[expected][assignment]") {
  std::logic_error exception { "error" };
  auto ptr = std::make_exception_ptr(exception);
  core::expected<std::string> value { };
  value = ptr;

  CHECK_FALSE(value);
  CHECK(value == ptr);
  CHECK_THROWS_AS(value.raise(), std::logic_error);
}

TEST_CASE("expected-operator-dereference", "[expected][operators]") {
  core::expected<int> nothrow { 51 };

  CHECK(nothrow);
  CHECK(*nothrow == 51);
}

TEST_CASE("expected-operator-arrow", "[expected][operators]") {
  core::expected<std::string> nothrow { "words" };
  CHECK(nothrow);
  CHECK(nothrow->at(0) == 'w');
}

TEST_CASE("expected-operator-equal", "[expected][operators]") {
  core::expected<int> lhs { 5 };
  core::expected<int> rhs { 6 };

  CHECK(lhs == 5);
  CHECK(6 == rhs);
  CHECK(lhs == core::expected<int> { 5 });
  CHECK(core::expected<int> { 6 } == rhs);
}

TEST_CASE("expected-operator-not-equal", "[expected][operators]") {
  core::expected<int> lhs { 5 };
  core::expected<int> rhs { 6 };

  core::expected<int> invalid { ::std::exception_ptr { } };

  CHECK(lhs);
  CHECK(rhs);
  CHECK(lhs != rhs);
  CHECK(lhs != invalid);
  CHECK(invalid != rhs);
}

TEST_CASE("expected-operator-greater-equal", "[expected][operators]") {
  core::expected<int> lhs { 5 };
  core::expected<int> rhs { 4 };
  core::expected<int> invalid { ::std::exception_ptr { } };

  CHECK(lhs >= rhs);
  CHECK(lhs >= invalid);
  CHECK(invalid >= ::std::exception_ptr { });
  CHECK(lhs >= 3);
  CHECK(lhs >= 5);
  CHECK(6 >= rhs);
  CHECK(4 >= rhs);
}

TEST_CASE("expected-operator-less-equal", "[expected][operators]") {
  core::expected<int> lhs { 5 };
  core::expected<int> rhs { 6 };
  core::expected<int> invalid { ::std::exception_ptr { } };

  CHECK(lhs <= rhs);
  CHECK(invalid <= rhs);
  CHECK(::std::exception_ptr { } <= invalid);
  CHECK(lhs <= 5);
  CHECK(lhs <= 6);
  CHECK(5 <= rhs);
  CHECK(6 <= rhs);
}

TEST_CASE("expected-operator-greater", "[expected][operators]") {
  core::expected<int> lhs { 6 };
  core::expected<int> rhs { 5 };

  CHECK(lhs > rhs);
  CHECK(lhs > 5);
  CHECK(6 > rhs);
}

TEST_CASE("expected-operator-less", "[expected][operators]") {
  core::expected<int> lhs { 5 };
  core::expected<int> rhs { 6 };

  CHECK(lhs < rhs);
  CHECK(lhs < 6);
}

TEST_CASE("expected-method-value-or", "[expected][methods]") {
  std::logic_error error { "error" };
  core::expected<std::string> value1 { std::make_exception_ptr(error) };
  core::expected<std::string> value2 { "value-or" };
  auto first = value1.value_or("value-or");
  auto second = value2.value_or("not-value");
  auto third = core::expected<std::string> { "value-or" }.value_or("empty");

  CHECK(first == "value-or");
  CHECK(second == "value-or");
  CHECK(third == "value-or");
}

TEST_CASE("expected-method-value", "[expected][methods]") {
  auto ptr = std::make_exception_ptr(std::logic_error { "" });
  core::expected<std::string> value { ptr };
  CHECK_THROWS_AS(value.value(), std::logic_error);
}

TEST_CASE("expected-method-expect", "[expected][methods]") {
  core::expected<int> value { };
  core::expected<int> error {
    std::make_exception_ptr(std::logic_error { "error" })
  };

  CHECK(value);
  CHECK_THROWS_AS(
    value.expect<std::nested_exception>(),
    core::bad_expected_type);
  auto err = error.expect<std::logic_error>();
  CHECK(std::string { err.what() } == "error");
}

TEST_CASE("expected-method-raise", "[expected][methods]") {
  core::expected<int> value { };
  core::expected<int> error {
    std::make_exception_ptr(std::logic_error { "raise" })
  };
  CHECK(value);
  CHECK_THROWS_AS(value.raise(), core::bad_expected_type);
  CHECK_THROWS_AS(error.raise(), std::logic_error);
}

TEST_CASE("expected-function-swap", "[expected][functions]") {
  using std::swap;
  auto ptr = std::make_exception_ptr(std::logic_error { "swap" });
  core::expected<int> error { ptr };
  core::expected<int> value { 5 };

  CHECK(value);
  CHECK_FALSE(error);

  swap(value, error);

  CHECK_FALSE(value);
  CHECK(error);
}

TEST_CASE("expected-function-make-expected", "[expected][functions]") {
  auto value = core::make_expected(std::string { "make-expected" });
  auto error = core::make_expected<std::string>(
    std::make_exception_ptr(std::logic_error { "error" })
  );
  auto logic_error = core::make_expected<std::string>(
    std::logic_error { "logic-error" }
  );

  CHECK(value);
  CHECK_FALSE(logic_error);
  CHECK_FALSE(error);
  CHECK(*value == "make-expected");
}

TEST_CASE("expected-issue-23", "[expected][issues]") {
  struct A { };
  auto foo = [] () -> core::expected<A> {
    return std::make_exception_ptr(std::logic_error { "error" });
  };
  auto bar = [] () -> core::expected<A> { return A { }; };
  auto const a = foo();
  auto const b = bar();
  auto const c = b;

  CHECK_FALSE(a);
  CHECK(b);
  CHECK(c);
}
#endif /* CORE_NO_EXCEPTIONS */

TEST_CASE("result-constructor-copy-value", "[result][constructors]") {
  std::string value { "copy" };
  core::result<std::string> result { value };

  CHECK(result);
  CHECK(*result == "copy");
}

TEST_CASE("result-constructor-move-value", "[result][constructors]") {
  std::string value { "move" };
  core::result<std::string> result { std::move(value) };

  CHECK(result);
  CHECK(value.size() == 0u);
  CHECK(*result == "move");
}

TEST_CASE("result-constructor-copy", "[result][constructors]") {
  core::result<int> value { 7 };
  core::result<int> copy { value };

  CHECK(value);
  CHECK(copy);
  CHECK(*value == 7);
  CHECK(*copy == 7);
}

TEST_CASE("result-constructor-move", "[result][constructors]") {
  core::result<std::string> value { "move" };
  core::result<std::string> move { core::move(value) };

  CHECK(value);
  CHECK(move);

  CHECK(value->empty());
  CHECK(move == std::string("move"));
}

TEST_CASE("result-constructor-condition", "[result][constructors]") {
  ::std::error_condition error { std::errc::permission_denied };
  core::result<std::string> value { error };
  CHECK_FALSE(value);
  CHECK(value.condition() == error);
}

TEST_CASE("result-constructor-enum", "[result][constructors]") {
  core::result<std::string> value { std::errc::permission_denied };
  CHECK_FALSE(value);
  constexpr auto compare = static_cast<
    core::underlying_type_t<std::errc>
  >(std::errc::permission_denied);
  CHECK(value.condition().value() == compare);
}

TEST_CASE("result-assignment-copy-value", "[result][assignment]") {
  core::result<int> copy { };
  copy = 4;

  CHECK(copy);
  CHECK(copy == 4);
}

TEST_CASE("result-assignment-move-value", "[result][assignment]") {
  core::result<std::string> move { };

  CHECK(move->empty());
  move = "move";
  CHECK(move == std::string { "move" });
}

TEST_CASE("result-assignment-copy", "[result][assignment]") {
  core::result<std::string> value { "copy" };
  core::result<std::string> copy { };
  copy = value;

  CHECK(value);
  CHECK(copy);
  CHECK(value == copy);
  CHECK(copy == std::string("copy"));
}

TEST_CASE("result-assignment-move", "[result][assignment]") {
  core::result<std::string> value { "move" };
  core::result<std::string> move { };

  move = core::move(value);

  CHECK(value);
  CHECK(move);
  CHECK(value->empty());
  CHECK(move == std::string("move"));
}

TEST_CASE("result-assignment-condition", "[result][assignment]") {
  auto error = make_error_condition(std::errc::permission_denied);
  core::result<std::string> value { };
  value = error;
  CHECK_FALSE(value);
  CHECK(value.condition() == error);
}

TEST_CASE("result-assignment-enum", "[result][assignment]") {
  core::result<std::string> value { };
  value = std::errc::permission_denied;
  CHECK_FALSE(value);
}

TEST_CASE("result-operator-dereference", "[result][operators]") {
  core::result<int> value { 6 };
  CHECK(*value == 6);
}

TEST_CASE("result-operator-arrow", "[result][operators]") {
  core::result<std::string> value { };
  CHECK(value->empty());
}

TEST_CASE("result-operator-equal", "[result][operators]") {
  core::result<std::string> lhs_valid { };
  core::result<std::string> rhs_valid { };
  core::result<std::string> invalid { std::errc::permission_denied };
  std::string value { };
  auto const condition = make_error_condition(std::errc::permission_denied);

  CHECK(lhs_valid == rhs_valid);
  CHECK(invalid == condition);
  CHECK(condition == invalid);
  CHECK(lhs_valid == value);
  CHECK(value == rhs_valid);
}

TEST_CASE("result-operator-not-equal", "[result][operators]") {
  core::result<std::string> lhs_valid { "lhs" };
  core::result<std::string> rhs_valid { "rhs" };
  std::string value { "value" };
  auto const condition = make_error_condition(std::errc::permission_denied);
  auto const code = make_error_code(std::errc::permission_denied);

  CHECK(lhs_valid != rhs_valid);
  CHECK(lhs_valid != condition);
  CHECK(condition != rhs_valid);
  CHECK(lhs_valid != code);
  CHECK(code != rhs_valid);
  CHECK(lhs_valid != value);
  CHECK(value != rhs_valid);
}

TEST_CASE("result-operator-greater-equal", "[result][operators]") {
  core::result<int> lhs { 5 };
  core::result<int> rhs { 4 };
  auto const condition = make_error_condition(std::errc::permission_denied);
  core::result<int> invalid { condition };

  CHECK(lhs >= rhs);
  CHECK(lhs >= invalid);
  CHECK(invalid >= condition);
  CHECK(lhs >= 3);
  CHECK(lhs >= 5);
  CHECK(6 >= rhs);
  CHECK(4 >= rhs);
}

TEST_CASE("result-operator-less-equal", "[result][operators]") {
  core::result<int> lhs { 3 };
  core::result<int> rhs { 6 };
  auto const condition = make_error_condition(std::errc::permission_denied);
  core::result<int> invalid { condition };

  CHECK(lhs <= rhs);
  CHECK(invalid <= rhs);
  CHECK(condition <= invalid);
  CHECK(lhs <= 3);
  CHECK(lhs <= 5);
  CHECK(6 <= rhs);
  CHECK(4 <= rhs);
}

TEST_CASE("result-operator-greater", "[result][operators]") {
  core::result<std::string> valid { "valid" };
  auto const error = make_error_condition(std::errc::permission_denied);
  auto const less_error = std::error_condition {
    1,
    std::generic_category()
  };
  core::result<std::string> invalid { error };

  core::result<std::string> less_invalid { less_error };

  CHECK(valid > invalid);
  CHECK(invalid > less_invalid);
  CHECK(valid > error);
  CHECK(error > less_invalid);
  CHECK(std::string { "zalid" } > valid);
  CHECK(valid > std::string { "alid" });
}

TEST_CASE("result-operator-less", "[result][operators]") {
  core::result<std::string> valid { "valid" };
  auto const error = make_error_condition(std::errc::permission_denied);
  auto const less_error = std::error_condition {
    1,
    std::generic_category()
  };
  core::result<std::string> invalid { error };

  core::result<std::string> less_invalid { less_error };

  CHECK(invalid < valid);
  CHECK(less_invalid < invalid);
  CHECK(error < valid);
  CHECK(less_invalid < error);
  CHECK(valid < std::string { "zalid" });
  CHECK(std::string { "alid" } < valid);
}

TEST_CASE("result-method-value-or", "[result][methods]") {
  core::result<std::string> value { "value" };
  core::result<std::string> error { std::errc::permission_denied };
  CHECK(value.value_or("") == "value");
  CHECK(error.value_or("error") == "error");
}

TEST_CASE("result-method-value", "[result][methods]") {
  core::result<std::string> value { "value" };
  core::result<std::string> thrower { std::errc::permission_denied };

  CHECK(value.value() == "value");
  CHECK_THROWS_AS(thrower.value(), std::system_error);
}

TEST_CASE("result-method-condition", "[result][methods]") {
  auto const error = make_error_condition(std::errc::permission_denied);
  core::result<std::string> value { error };
  CHECK(value.condition() == error);
}

TEST_CASE("result-function-swap", "[result][functions]") {
  using ::std::swap;
  core::result<std::string> lhs_valid { "lhs" };
  core::result<std::string> rhs_valid { "rhs" };
  auto const error = make_error_condition(std::errc::permission_denied);
  core::result<std::string> invalid { error };

  swap(lhs_valid, rhs_valid);
  CHECK(lhs_valid == std::string { "rhs" });
  CHECK(rhs_valid == std::string { "lhs" });

  swap(lhs_valid, invalid);

  CHECK_FALSE(lhs_valid);
  CHECK(invalid);
  CHECK(lhs_valid == error);
  CHECK(invalid == std::string { "rhs" });
}

TEST_CASE("result-function-make-result", "[result][functions]") {
  auto result = core::make_result(std::string { "value" });
  auto condition = core::make_result<std::string>(
    make_error_condition(std::errc::permission_denied)
  );
  auto error = core::make_result<std::string>(std::errc::permission_denied);
  constexpr auto value = std::is_same<
    decltype(result),
    core::result<std::string>
  >::value;

  CHECK(value);
  CHECK_FALSE(condition);
  CHECK_FALSE(error);
}

TEST_CASE("result-issue-23", "[result][issues]") {
  struct A { };
  auto foo = [] () -> core::result<A> {
    return std::errc::permission_denied;
  };
  auto bar = [] () -> core::result<A> { return A { }; };
  auto const a = foo();
  auto const b = bar();
  auto const c = b;

  CHECK_FALSE(a);
  CHECK(b);
  CHECK(c);
}

#ifndef CORE_NO_EXCEPTIONS
TEST_CASE("expected-void-constructor", "[expected][void][constructor]") {
  core::expected<void> value { };
  CHECK(value);
}

TEST_CASE("expected-void-constructor-copy", "[expected][void][constructor]") {
  auto ptr = std::make_exception_ptr(std::logic_error { "" });
  core::expected<void> value { ptr };
  core::expected<void> copy { value };

  CHECK_FALSE(value);
  CHECK_FALSE(copy);

  CHECK(value.pointer() == copy.pointer());
  CHECK(value.pointer() == ptr);
  CHECK(copy.pointer() == ptr);
}

TEST_CASE("expected-void-constructor-move", "[expected][void][assignment]") {
  auto ptr = std::make_exception_ptr(std::logic_error { "" });
  core::expected<void> value { ptr };
  core::expected<void> move { std::move(value) };

  CHECK_FALSE(move);
  CHECK(move.pointer() == ptr);
}

TEST_CASE("expected-void-constructor-ptr", "[expected][void][constructor]") {
  auto ptr = std::make_exception_ptr(std::logic_error { "" });
  core::expected<void> value { ptr };

  CHECK_FALSE(value);
  CHECK(ptr == value.pointer());
}

TEST_CASE("expected-void-assignment-copy", "[expected][void][assignment]") {
  auto ptr = std::make_exception_ptr(std::logic_error { "" });
  core::expected<void> value { ptr };
  core::expected<void> copy { };

  CHECK(copy);

  copy = value;

  CHECK_FALSE(value);
  CHECK_FALSE(copy);

  CHECK(value.pointer() == copy.pointer());
  CHECK(value.pointer() == ptr);
  CHECK(copy.pointer() == ptr);
}

TEST_CASE("expected-void-assignment-move", "[expected][void][assignment]") {
  auto ptr = std::make_exception_ptr(std::logic_error { "" });
  core::expected<void> value { ptr };
  core::expected<void> move { };

  CHECK(move);

  move = std::move(value);

  CHECK_FALSE(move);

  CHECK(move.pointer() == ptr);
}

TEST_CASE("expected-void-operator-equal", "[expected][void][operators]") {
  auto ptr = std::make_exception_ptr(std::logic_error { "" });
  core::expected<void> empty_lhs { };
  core::expected<void> empty_rhs { };

  core::expected<void> lhs { ptr };
  core::expected<void> rhs { ptr };

  CHECK(empty_lhs);
  CHECK(empty_rhs);

  CHECK(empty_lhs == empty_rhs);
  CHECK(lhs.pointer() == rhs.pointer());
  CHECK(lhs.pointer() == ptr);
  CHECK(rhs.pointer() == ptr);
  CHECK(lhs == ptr);
  CHECK(ptr == rhs);
}

TEST_CASE("expected-void-method-expect", "[expected][void][methods]") {
  core::expected<void> value { };
  core::expected<void> error {
    std::make_exception_ptr(std::logic_error { "error" })
  };
  CHECK(value);
  CHECK_THROWS_AS(
    value.expect<std::nested_exception>(),
    core::bad_expected_type);
  auto err = error.expect<std::logic_error>();
  CHECK(std::string { err.what() } == "error");
}

TEST_CASE("expected-void-method-raise", "[expected][void][methods]") {
  core::expected<void> value { };
  core::expected<void> error {
    std::make_exception_ptr(std::logic_error { "raise" })
  };

  CHECK(value);
  CHECK_THROWS_AS(value.raise(), core::bad_expected_type);
  CHECK_THROWS_AS(error.raise(), std::logic_error);
}

TEST_CASE("expected-void-function-swap", "[expected][void][functions]") {
  using std::swap;
  auto lhs_ptr = std::make_exception_ptr(std::logic_error { "" });
  auto rhs_ptr = std::make_exception_ptr(core::bad_expected_type { "" });

  core::expected<void> lhs { lhs_ptr };
  core::expected<void> rhs { rhs_ptr };

  CHECK_FALSE(lhs);
  CHECK_FALSE(rhs);
  CHECK(lhs.pointer() == lhs_ptr);
  CHECK(rhs.pointer() == rhs_ptr);

  swap(lhs, rhs);

  CHECK_FALSE(lhs);
  CHECK_FALSE(rhs);
  CHECK(lhs.pointer() == rhs_ptr);
  CHECK(rhs.pointer() == lhs_ptr);
}
#endif /* CORE_NO_EXCDEPTIONS */

TEST_CASE("result-void-constructor", "[result][void][constructors]") {
  core::result<void> value;
  CHECK(value);
}

TEST_CASE("result-void-constructor-copy", "[result][void][constructors]") {
  core::result<void> value { };
  core::result<void> copy { value };
  CHECK(copy == value);
}

TEST_CASE("result-void-constructor-move", "[result][void][constructors]") {
  core::result<void> value { };
  core::result<void> move { core::move(value) };
  CHECK(value == move);
}

TEST_CASE("result-void-constructor-cond", "[result][void][constructors]") {
  auto const error = make_error_condition(std::errc::permission_denied);
  core::result<void> value { error };
  CHECK_FALSE(value);
  CHECK(value == error);
}

TEST_CASE("result-void-constructor-enum", "[result][void][constructors]") {
  core::result<void> value { std::errc::permission_denied };
  CHECK_FALSE(value);
  CHECK(value == make_error_condition(std::errc::permission_denied));
}

TEST_CASE("result-void-assignment-copy", "[result][void][assignment]") {
  core::result<void> value { std::errc::permission_denied };
  core::result<void> copy { };

  CHECK_FALSE(value);
  value = copy;
  CHECK(value);
}

TEST_CASE("result-void-assignment-move", "[result][void][assignment]") {
  core::result<void> value { std::errc::permission_denied };
  core::result<void> move { };
  CHECK_FALSE(value);
  value = core::move(move);
  CHECK(value);
}

TEST_CASE("result-void-assignment-error-cond", "[result][void][assignment]") {
  core::result<void> value { };
  auto const error = make_error_condition(std::errc::permission_denied);
  CHECK(value);
  value = error;
  CHECK_FALSE(value);
}

TEST_CASE("result-void-assignment-error-enum", "[result][void][assignment]") {
  core::result<void> value { };
  CHECK(value);
  value = std::errc::permission_denied;
  CHECK_FALSE(value);
}

TEST_CASE("result-void-operator-equal", "[result][void][operators]") {
  core::result<void> lhs_valid { };
  core::result<void> rhs_valid { };
  core::result<void> lhs_invalid { std::errc::permission_denied };
  core::result<void> rhs_invalid { std::errc::permission_denied };

  CHECK(lhs_valid == rhs_valid);
  CHECK(lhs_invalid == rhs_invalid);
}

#ifndef CORE_NO_EXCEPTIONS
TEST_CASE("result-void-method-condition", "[result][void][functions]") {
  auto const condition = make_error_condition(std::errc::permission_denied);
  core::result<void> valid { };
  core::result<void> invalid { condition };

  CHECK(valid);
  CHECK_FALSE(invalid);

  CHECK(invalid == condition);
  CHECK_THROWS_AS(valid.condition(), core::bad_result_condition);
}
#endif /* CORE_NO_EXCEPTIONS */
