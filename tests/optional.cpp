#include <core/optional.hpp>

#include <unordered_map>
#include <utility>
#include <string>
#include <array>

#include <cstdint>

#include <unittest/unittest.hpp>

struct variadic {
  template <class... Args> constexpr variadic (Args&&...) { }
  template <class T, class... Args>
  constexpr variadic (std::initializer_list<T>, Args&&...) { }
};

std::ostream& operator << (
  std::ostream& os,
  ::core::v1::expected<void> const&
) { return os << "core::expected<void>"; }

int main () {
  using namespace unittest;

  test("optional") = {
    task("default-constructor") = [] {
      core::optional<int> opt;
      assert::is_true(not opt);
      assert::equal(opt, core::nullopt);
    },

    task("copy-constructor") = [] {
      core::optional<int> opt { 4 };
      core::optional<int> copy { opt };
      assert::is_false(not opt);
      assert::is_false(not copy);
      assert::equal(opt, copy);
      assert::equal(copy.value(), 4);
    },

    task("move-constructor") = [] {
      core::optional<int> opt { 4 };
      core::optional<int> move { std::move(opt) };
      assert::is_false(not opt); // should be in a valid, but null state
      assert::is_false(not move);
      assert::equal(move.value(), 4);
    },

    task("null-constructor") = [] {
      core::optional<int> opt { core::nullopt };
      assert::is_true(not opt);
    },

    task("copy-value-constructor") = [] {
      std::string text { "copy-value" };
      core::optional<std::string> opt { text };

      assert::is_false(text.empty());
      assert::is_true(bool(opt));
      assert::equal(opt.value(), text);
      assert::equal(opt.value(), std::string { "copy-value" });
    },

    task("move-value-constructor") = [] {
      std::string text { "move-value" };
      core::optional<std::string> opt { std::move(text) };
      assert::is_true(bool(opt));
      assert::is_true(text.empty());
      assert::equal(opt.value(), std::string { "move-value" });
    },

    task("in-place-constructor") = [] {
      core::optional<std::string> opt1 { core::in_place, "in-place" };
      core::optional<std::string> opt2 { core::in_place,
        { 'i', 'n', '-', 'p', 'l', 'a', 'c', 'e' }
      };
      assert::is_true(bool(opt1));
      assert::is_true(bool(opt2));
      assert::equal(opt1, std::string { "in-place" });
      assert::equal(opt2, std::string { "in-place" });
      assert::equal(opt1, opt2);
    },

    task("null-assign-operator") = [] {
      core::optional<int> opt { 4 };
      assert::is_false(not opt);
      opt = core::nullopt;
      assert::is_true(not opt);
    },

    task("copy-assign-operator") = [] {
      core::optional<int> opt { 4 };
      core::optional<int> copy { };
      copy = opt;

      assert::is_false(not opt);
      assert::is_false(not copy);
      assert::equal(opt, copy);
      assert::equal(*copy, 4);
    },

    task("move-assign-operator") = [] {
      core::optional<int> opt { 4 };
      core::optional<int> move { };
      move = std::move(opt);

      assert::is_false(not opt); // in a valid, but null state
      assert::is_false(not move);
      assert::equal(*move, 4);
    },

    task("copy-value-assign-operator") = [] {
      core::optional<int> opt { };
      int value = 4;
      opt = value;
      assert::is_true(bool(opt));
      assert::equal(*opt, 4);
    },

    task("move-value-assign-operator") = [] {
      core::optional<std::string> opt { };
      std::string value { "move-value" };
      opt = std::move(value);
      assert::is_true(value.empty());
      assert::is_true(bool(opt));
      assert::equal(*opt, std::string { "move-value" });
    },

    task("arrow-operator") = [] {
      core::optional<std::string> opt { "arrow" };
      assert::is_false(not opt);
      assert::is_false(opt->empty());
    },

    task("dereference-operator") = [] {
      core::optional<int> opt { 56 };
      assert::is_false(not opt);
      assert::equal(*opt, 56);
    },

    task("value") = [] {
      assert::throws<core::bad_optional_access>([] {
        core::optional<int> opt { };
        int x = opt.value();
        std::ignore = x;
      });
    },

    task("value-or") = [] {
      core::optional<std::string> opt { };
      core::optional<std::string> val { "value-or" };

      auto first = opt.value_or("value-or");
      auto second = val.value_or("not-value");

      assert::equal(first, std::string { "value-or" });
      assert::equal(second, std::string { "value-or" });
    },

    task("emplace") = [] {
      core::optional<int> opt { };
      opt.emplace(4);
      assert::is_false(not opt);
      assert::equal(*opt, 4);
    },

    task("equality-comparable") = [] {
      core::optional<int> lhs_int { };
      core::optional<int> rhs_int { };

      rhs_int.emplace(7);

      assert::is_false(core::nullopt == rhs_int);
      assert::equal(lhs_int, decltype(rhs_int) { });
      assert::equal(lhs_int, core::nullopt);
      assert::equal(rhs_int, 7);
      assert::equal(7, rhs_int);
    },

    task("less-than-comparable") = [] {
      core::optional<int> empty_lhs { };
      core::optional<int> empty_rhs { };
      core::optional<int> lhs { 4 };
      core::optional<int> rhs { 8 };

      assert::is_false(empty_lhs < empty_rhs);
      assert::less(empty_lhs, rhs);
      assert::less(lhs, rhs);

      assert::less(empty_lhs, core::nullopt);
      assert::less(core::nullopt, rhs);
      assert::less(lhs, 5);
    },

    task("make-optional") = [] {
      auto opt = core::make_optional<std::string>("make-optional");
      assert::is_true(bool(opt));
      assert::equal(*opt, std::string { "make-optional" });
    },

    task("swap") = [] {
      using std::swap;
      core::optional<int> lhs { 4 };
      core::optional<int> rhs { };
      core::optional<int> rhs2 { 4 };

      swap(lhs, rhs);

      assert::is_true(not lhs);
      assert::is_true(bool(rhs));
      assert::equal(rhs, 4);
      assert::equal(rhs, rhs2);

      swap(lhs, rhs);

      assert::is_true(not rhs);
      assert::is_true(bool(lhs));
      assert::equal(lhs, 4);
      assert::equal(lhs, rhs2);

    },

    task("hash") = [] {
      std::unordered_map<core::optional<std::string>, int> values = {
        std::make_pair(core::make_optional<std::string>("text1"), 0),
        std::make_pair(core::make_optional<std::string>("text2"), 1),
        std::make_pair(core::make_optional<std::string>("text3"), 2),
      };

      assert::equal(values[core::make_optional<std::string>("text1")], 0);
      assert::equal(values[core::make_optional<std::string>("text2")], 1);
      assert::equal(values[core::make_optional<std::string>("text3")], 2);
    },

    task("force-disengage") = [] {
      core::optional<int> value { 5 };
      assert::is_true(static_cast<bool>(value));
      value = {};
      assert::is_false(static_cast<bool>(value));
    }
  };

  test("constexpr-optional") = {
    task("default-constructor") = [] {
      constexpr core::optional<int> opt { };
      constexpr bool value = static_cast<bool>(opt);
      static_assert(not value, "");
    },

    task("nullopt-constructor") = [] {
      constexpr core::optional<int> opt { core::nullopt };
      constexpr bool value = static_cast<bool>(opt);
      static_assert(not value, "");
    },

    task("value-constructor") = [] {
      constexpr int value = 7;
      constexpr core::optional<int> copy { value };
      constexpr core::optional<int> move { 5 };
      static_assert(copy, "could not initialize core::optional<int>");
      static_assert(move, "could not initialize core::optional<int>");
      constexpr bool copy_result = *copy == value;
      constexpr bool move_result = *move == 5;
      static_assert(copy_result, "");
      static_assert(move_result, "");
    },

    task("variadic-value-constructor") = [] {
      constexpr core::optional<variadic> list { core::in_place, { 1, 2, 3, 4 } };
      constexpr core::optional<variadic> nolist { core::in_place, 1, "words" };
      constexpr bool list_initd = static_cast<bool>(list);
      constexpr bool nolist_initd = static_cast<bool>(nolist);
      static_assert(list_initd, "");
      static_assert(nolist_initd, "");
    },

    task("value") = [] {
      constexpr core::optional<int> opt { 5 };
      constexpr int value { opt.value() };
      static_assert(value == 5, "");
    },

    task("value-or") = [] {
      constexpr core::optional<int> engaged { 5 };
      constexpr core::optional<int> disengaged { };
      constexpr int value_or_e = engaged.value_or(7);
      constexpr int value_or_d = disengaged.value_or(7);
      static_assert(value_or_e == 5, "");
      static_assert(value_or_d == 7, "");
    },

    task("operator-star") = [] {
      constexpr core::optional<int> lhs { 5 };
      constexpr int value { *lhs };
      static_assert(value == 5, "");
    },

    task("operator-equal") = [] {
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
    },

    task("operator-not-equal") = [] {
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
    },

    task("operator-greater-equal") = [] {
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
    },

    task("operator-less-equal") = [] {
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
    },

    task("operator-greater") = [] {
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
    },

    task("operator-less") = [] {
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
  };

  test("expected") = {
    task("copy-value-constructor") = [] {
      auto value = 53;
      core::expected<int> copy { value };

      assert::is_true(bool(copy));
      assert::equal(copy.value(), value);
    },

    task("move-value-constructor") = [] {
      std::string value { "move-value" };
      core::expected<std::string> move { std::move(value) };

      assert::is_true(bool(move));
      assert::is_true(value.empty());

      assert::equal(move.value(), std::string { "move-value" });
    },

    task("copy-constructor") = [] {
      core::expected<int> value { 5 };
      core::expected<int> copy { value };

      assert::is_true(bool(value));
      assert::is_true(bool(copy));

      assert::equal(value.value(), copy.value());
      assert::equal(copy.value(), 5);
    },

    task("move-constructor") = [] {
      core::expected<std::string> value { "move" };
      core::expected<std::string> move { std::move(value) };

      assert::is_true(bool(value));
      assert::is_true(bool(move));

      assert::is_true(value.value().empty());
      assert::is_false(move.value().empty());
      assert::equal(move.value(), std::string { "move" });
    },

    task("ptr-constructor") = [] {
      auto ptr = std::make_exception_ptr(std::logic_error { "test" });
      core::expected<int> value { ptr };

      assert::is_true(not value);
      auto exception = value.expect<std::logic_error>();
      assert::equal(std::string { exception.what() }, std::string { "test" });
    },

    task("copy-value-assign-operator") = [] {
      core::expected<int> value { };
      value = 5;
      assert::is_true(bool(value));
      assert::equal(value.value(), 5);
    },

    task("move-value-assign-operator") = [] {
      core::expected<std::string> value { };
      std::string str { "move" };
      value = std::move(str);
      assert::is_true(bool(value));
      assert::is_true(str.empty());
      assert::equal(value.value(), std::string { "move" });
    },

    task("move-assign-operator") = [] {
      core::expected<std::string> value { "move" };
      core::expected<std::string> move { };
      move = std::move(value);

      assert::is_true(bool(value));
      assert::is_true(bool(move));
      assert::is_true(value.value().empty());

      assert::equal(move.value(), std::string { "move" });
    },

    task("copy-assign-operator") = [] {
      core::expected<std::string> value { "copy" };
      core::expected<std::string> copy { };
      copy = value;

      assert::is_true(bool(value));
      assert::is_true(bool(copy));
      assert::equal(copy.value(), value.value());

      assert::equal(copy.value(), std::string { "copy" });
    },

    task("ptr-assign-operator") = [] {
      std::logic_error exception { "error" };
      auto ptr = std::make_exception_ptr(exception);
      core::expected<std::string> value { };
      value = ptr;

      assert::is_true(not value);
      assert::equal(value, ptr);
      assert::throws<std::logic_error>([&value] { value.raise(); });
    },

    task("dereference-operator") = [] {
      core::expected<int> nothrow { 51 };

      assert::is_true(bool(nothrow));

      assert::equal(*nothrow, 51);
    },

    task("arrow-operator") = [] {
      core::expected<std::string> nothrow { "words" };
      assert::is_true(bool(nothrow));
      assert::equal(nothrow->at(0), 'w');
    },

    task("operator-equal") = [] {
      core::expected<int> lhs { 5 };
      core::expected<int> rhs { 6 };

      assert::is_true(bool(lhs));
      assert::is_true(bool(rhs));
      assert::equal(lhs, 5);
      assert::equal(6, rhs);
      assert::equal(lhs, core::expected<int> { 5 });
      assert::equal(core::expected<int> { 6 }, rhs);
    },

    task("operator-not-equal") = [] {
      core::expected<int> lhs { 5 };
      core::expected<int> rhs { 6 };

      core::expected<int> invalid { ::std::exception_ptr { } };

      assert::is_true(bool(lhs));
      assert::is_true(bool(rhs));
      assert::not_equal(lhs, rhs);
      assert::not_equal(lhs, invalid);
      assert::not_equal(invalid, rhs);
    },

    task("operator-greater-equal") = [] {
      core::expected<int> lhs { 5 };
      core::expected<int> rhs { 4 };
      core::expected<int> invalid { ::std::exception_ptr { } };

      assert::greater_equal(lhs, rhs);
      assert::greater_equal(lhs, invalid);
      assert::greater_equal(invalid, ::std::exception_ptr { });
      assert::greater_equal(lhs, 3);
      assert::greater_equal(lhs, 5);
      assert::greater_equal(6, rhs);
      assert::greater_equal(4, rhs);
    },

    task("operator-less-equal") = [] {
      core::expected<int> lhs { 5 };
      core::expected<int> rhs { 6 };
      core::expected<int> invalid { ::std::exception_ptr { } };

      assert::less_equal(lhs, rhs);
      assert::less_equal(invalid, rhs);
      assert::less_equal(::std::exception_ptr { }, invalid);
      assert::less_equal(lhs, 5);
      assert::less_equal(lhs, 6);
      assert::less_equal(5, rhs);
      assert::less_equal(6, rhs);
    },

    task("operator-greater") = [] {
      core::expected<int> lhs { 6 };
      core::expected<int> rhs { 5 };

      assert::greater(lhs, rhs);
      assert::greater(lhs, 5);
      assert::greater(6, rhs);
    },

    task("operator-less") = [] {
      core::expected<int> lhs { 5 };
      core::expected<int> rhs { 6 };

      assert::is_true(bool(lhs));
      assert::is_true(bool(rhs));
      assert::less(lhs, rhs);
      assert::less(lhs, 6);
    },

    task("value-or") = [] {
      std::logic_error error { "error" };
      core::expected<std::string> value1 { std::make_exception_ptr(error) };
      core::expected<std::string> value2 { "value-or" };
      auto first = value1.value_or("value-or");
      auto second = value2.value_or("not-value");
      auto third = core::expected<std::string> { "value-or" }.value_or("empty");

      assert::equal(first, std::string { "value-or" });
      assert::equal(second, std::string { "value-or" });
      assert::equal(third, std::string { "value-or" });
    },

    task("value") = [] {
      assert::throws<std::logic_error>([]{
        auto ptr = std::make_exception_ptr(std::logic_error { "" });
        core::expected<std::string> value { ptr };
        std::ignore = value.value();
      });
    },

    task("expect") = [] {
      core::expected<int> value { };
      core::expected<int> error {
        std::make_exception_ptr(std::logic_error { "error" })
      };
      assert::is_true(bool(value));
      assert::throws<core::bad_expected_type>([&value] {
        std::ignore = value.expect<std::nested_exception>();
      });
      auto err = error.expect<std::logic_error>();
      assert::equal(std::string { err.what() }, std::string { "error" });
    },

    task("raise") = [] {
      core::expected<int> value { };
      core::expected<int> error {
        std::make_exception_ptr(std::logic_error { "raise" })
      };
      assert::is_true(bool(value));
      assert::throws<core::bad_expected_type>([&value] { value.raise(); });
      assert::throws<std::logic_error>([&error] { error.raise(); });
    },

    task("swap") = [] {
      using std::swap;
      auto ptr = std::make_exception_ptr(std::logic_error { "swap" });
      core::expected<int> error { ptr };
      core::expected<int> value { 5 };

      assert::is_true(bool(value));
      assert::is_true(not error);

      swap(value, error);

      assert::is_true(bool(error));
      assert::is_true(not value);
    },

    task("make_expected") = [] {
      auto value = core::make_expected(std::string { "make-expected" });
      auto error = core::make_expected<std::string>(
        std::make_exception_ptr(std::logic_error { "error" })
      );
      auto logic_error = core::make_expected<std::string>(
        std::logic_error { "logic-error" }
      );

      assert::is_true(bool(value));
      assert::is_true(not logic_error);
      assert::is_true(not error);

      assert::equal(*value, std::string { "make-expected" });
    }
  };

  test("result") = {
    task("copy-value-constructor") = [] {
      std::string value { "copy" };
      core::result<std::string> result { value };

      assert::is_true(bool(result));
      assert::equal(*result, "copy");
    },

    task("move-value-constructor") = [] {
      std::string value { "move" };
      core::result<std::string> result { std::move(value) };

      assert::is_true(bool(result));
      assert::equal(value.size(), 0u);
      assert::equal(*result, "move");
    },

    task("copy-constructor") = [] {
      core::result<int> value { 7 };
      core::result<int> copy { value };

      assert::is_true(bool(value));
      assert::is_true(bool(copy));
      assert::equal(*value, 7);
      assert::equal(*copy, 7);
    },

    task("move-constructor") = [] {
      core::result<std::string> value { "move" };
      core::result<std::string> move { core::move(value) };

      assert::is_true(bool(value));
      assert::is_true(bool(move));

      assert::is_true(value->empty());
      assert::equal(move, std::string { "move" });
    },

    task("error-condition-constructor") = [] {
      ::std::error_condition error { std::errc::permission_denied };
      core::result<std::string> value { error };
      assert::is_true(not value);
      assert::equal(value.condition(), error);
    },

    task("error-condition-enum-constructor") = [] {
      core::result<std::string> value { std::errc::permission_denied };
      assert::is_true(not value);
      constexpr auto compare = static_cast<
        core::underlying_type_t<std::errc>
      >(std::errc::permission_denied);
      assert::equal(value.condition().value(), compare);
    },

    task("copy-value-assign-operator") = [] {
      core::result<int> copy { };
      copy = 4;

      assert::is_true(bool(copy));
      assert::equal(copy, 4);
    },

    task("move-value-assign-operator") = [] {
      core::result<std::string> move { };

      assert::is_true(move->empty());
      move = "move";
      assert::equal(move, std::string { "move" });
    },

    task("copy-assign-operator") = [] {
      core::result<std::string> value { "copy" };
      core::result<std::string> copy { };
      copy = value;

      assert::is_true(bool(value));
      assert::is_true(bool(copy));
      assert::equal(value, copy);
      assert::equal(copy, std::string { "copy" });
    },

    task("move-assign-operator") = [] {
      core::result<std::string> value { "move" };
      core::result<std::string> move { };

      move = core::move(value);

      assert::is_true(bool(value));
      assert::is_true(bool(move));
      assert::is_true(value->empty());
      assert::equal(move, std::string { "move" });
    },

    task("error-condition-assign-operator") = [] {
      auto error = make_error_condition(std::errc::permission_denied);
      core::result<std::string> value { };
      value = error;
      assert::is_true(not value);
      assert::equal(value.condition(), error);
    },

    task("error-condition-enum-assign-operator") = [] {
      core::result<std::string> value { };
      value = std::errc::permission_denied;
      assert::is_false(bool(value));
    },

    task("operator-star") = [] {
      core::result<int> value { 6 };
      assert::equal(*value, 6);
    },

    task("operator-arrow") = [] {
      core::result<std::string> value { };
      assert::is_true(value->empty());
    },

    task("operator-equal") = [] {
      core::result<std::string> lhs_valid { };
      core::result<std::string> rhs_valid { };
      core::result<std::string> invalid { std::errc::permission_denied };
      std::string value { };
      auto const condition = make_error_condition(std::errc::permission_denied);
      auto const code = make_error_code(std::errc::permission_denied);

      assert::equal(lhs_valid, rhs_valid);
      assert::equal(invalid, condition);
      assert::equal(condition, invalid);
      assert::equal(invalid, code);
      assert::equal(code, invalid);
      assert::equal(lhs_valid, value);
      assert::equal(value, rhs_valid);
    },

    task("operator-not-equal") = [] {
      core::result<std::string> lhs_valid { "lhs" };
      core::result<std::string> rhs_valid { "rhs" };
      std::string value { "value" };
      auto const condition = make_error_condition(std::errc::permission_denied);
      auto const code = make_error_code(std::errc::permission_denied);

      assert::not_equal(lhs_valid, rhs_valid);
      assert::not_equal(lhs_valid, condition);
      assert::not_equal(condition, rhs_valid);
      assert::not_equal(lhs_valid, code);
      assert::not_equal(code, rhs_valid);
      assert::not_equal(lhs_valid, value);
      assert::not_equal(value, rhs_valid);
    },

    task("operator-greater-equal") = [] {
      core::result<int> lhs { 5 };
      core::result<int> rhs { 4 };
      auto const condition = make_error_condition(std::errc::permission_denied);
      core::result<int> invalid { condition };

      assert::greater_equal(lhs, rhs);
      assert::greater_equal(lhs, invalid);
      assert::greater_equal(invalid, condition);
      assert::greater_equal(lhs, 3);
      assert::greater_equal(lhs, 5);
      assert::greater_equal(6, rhs);
      assert::greater_equal(4, rhs);
    },

    task("operator-less-equal") = [] {
      core::result<int> lhs { 3 };
      core::result<int> rhs { 6 };
      auto const condition = make_error_condition(std::errc::permission_denied);
      core::result<int> invalid { condition };

      assert::less_equal(lhs, rhs);
      assert::less_equal(invalid, rhs);
      assert::less_equal(condition, invalid);
      assert::less_equal(lhs, 3);
      assert::less_equal(lhs, 5);
      assert::less_equal(6, rhs);
      assert::less_equal(4, rhs);
    },

    task("operator-greater") = [] {
      core::result<std::string> valid { "valid" };
      auto const error = make_error_condition(std::errc::permission_denied);
      auto const less_error = std::error_condition {
        1,
        std::generic_category()
      };
      core::result<std::string> invalid { error };

      core::result<std::string> less_invalid { less_error };

      assert::greater(valid, invalid);
      assert::greater(invalid, less_invalid);
      assert::greater(valid, error);
      assert::greater(error, less_invalid);
      assert::greater(std::string { "zalid" }, valid);
      assert::greater(valid, std::string { "alid" });
    },

    task("operator-less") = [] {
      core::result<std::string> valid { "valid" };
      auto const error = make_error_condition(std::errc::permission_denied);
      auto const less_error = std::error_condition {
        1,
        std::generic_category()
      };
      core::result<std::string> invalid { error };

      core::result<std::string> less_invalid { less_error };

      assert::less(invalid, valid);
      assert::less(less_invalid, invalid);
      assert::less(error, valid);
      assert::less(less_invalid, error);
      assert::less(valid, std::string { "zalid" });
      assert::less(std::string { "alid" }, valid);
    },

    task("value-or") = [] {
      core::result<std::string> value { "value" };
      core::result<std::string> error { std::errc::permission_denied };
      assert::equal(value.value_or(""), "value");
      assert::equal(error.value_or("error"), "error");
    },

    task("value") = [] {
      core::result<std::string> value { "value" };
      core::result<std::string> thrower { std::errc::permission_denied };

      assert::equal(value.value(), "value");
      assert::throws<std::system_error>([&] {
        std::ignore = thrower.value();
      });
    },

    task("condition") = [] {
      auto const error = make_error_condition(std::errc::permission_denied);
      core::result<std::string> value { error };
      assert::equal(value.condition(), error);
    },

    task("swap") = [] {
      using ::std::swap;
      core::result<std::string> lhs_valid { "lhs" };
      core::result<std::string> rhs_valid { "rhs" };
      auto const error = make_error_condition(std::errc::permission_denied);
      core::result<std::string> invalid { error };

      swap(lhs_valid, rhs_valid);
      assert::equal(lhs_valid, std::string { "rhs" });
      assert::equal(rhs_valid, std::string { "lhs" });

      swap(lhs_valid, invalid);

      assert::is_false(bool(lhs_valid));
      assert::is_true(bool(invalid));
      assert::equal(lhs_valid, error);
      assert::equal(invalid, std::string { "rhs" });
    },

    task("make-result") = [] {
      auto result = core::make_result(std::string { "value" });
      auto condition = core::make_result<std::string>(
        make_error_condition(std::errc::permission_denied)
      );
      auto error = core::make_result<std::string>(std::errc::permission_denied);
      constexpr auto value = std::is_same<
        decltype(result),
        core::result<std::string>
      >::value;

      assert::is_true(bool(value));
      assert::is_false(bool(condition));
      assert::is_false(bool(error));
    }
  };

  test("expected<void>") = {
    task("default-constructor") = [] {
      core::expected<void> value { };
      assert::is_true(bool(value));
    },

    task("copy-constructor") = [] {
      auto ptr = std::make_exception_ptr(std::logic_error { "" });
      core::expected<void> value { ptr };
      core::expected<void> copy { value };

      assert::is_true(not value);
      assert::is_true(not copy);

      assert::equal(value.pointer(), copy.pointer());
      assert::equal(value.pointer(), ptr);
      assert::equal(copy.pointer(), ptr);
    },

    task("move-constructor") = [] {
      auto ptr = std::make_exception_ptr(std::logic_error { "" });
      core::expected<void> value { ptr };
      core::expected<void> move { std::move(value) };

      assert::is_true(not move);

      assert::equal(move.pointer(), ptr);
    },

    task("ptr-constructor") = [] {
      auto ptr = std::make_exception_ptr(std::logic_error { "" });
      core::expected<void> value { ptr };

      assert::is_true(not value);
      assert::equal(ptr, value.pointer());
    },

    task("copy-assign-operator") = [] {
      auto ptr = std::make_exception_ptr(std::logic_error { "" });
      core::expected<void> value { ptr };
      core::expected<void> copy { };

      assert::is_true(bool(copy));

      copy = value;

      assert::is_true(not value);
      assert::is_true(not copy);

      assert::equal(value.pointer(), copy.pointer());
      assert::equal(value.pointer(), ptr);
      assert::equal(copy.pointer(), ptr);
    },

    task("move-assign-operator") = [] {
      auto ptr = std::make_exception_ptr(std::logic_error { "" });
      core::expected<void> value { ptr };
      core::expected<void> move { };

      assert::is_true(bool(move));

      move = std::move(value);

      assert::is_true(not move);

      assert::equal(move.pointer(), ptr);
    },

    task("equality-comparable") = [] {
      auto ptr = std::make_exception_ptr(std::logic_error { "" });
      core::expected<void> empty_lhs { };
      core::expected<void> empty_rhs { };

      core::expected<void> lhs { ptr };
      core::expected<void> rhs { ptr };

      assert::is_true(bool(empty_lhs));
      assert::is_true(bool(empty_rhs));

      assert::equal(empty_lhs, empty_rhs);
      assert::equal(lhs.pointer(), rhs.pointer());
      assert::equal(lhs.pointer(), ptr);
      assert::equal(rhs.pointer(), ptr);
      assert::equal(lhs, ptr);
      assert::equal(ptr, rhs);
    },

    task("expect") = [] {
      core::expected<void> value { };
      core::expected<void> error {
        std::make_exception_ptr(std::logic_error { "error" })
      };
      assert::is_true(bool(value));
      assert::throws<core::bad_expected_type>([&value] {
        std::ignore = value.expect<std::nested_exception>();
      });
      auto err = error.expect<std::logic_error>();
      assert::equal(std::string { err.what() }, std::string { "error" });
    },

    task("raise") = [] {
      core::expected<void> value { };
      core::expected<void> error {
        std::make_exception_ptr(std::logic_error { "raise" })
      };
      assert::is_true(bool(value));
      assert::throws<core::bad_expected_type>([&value] { value.raise(); });
      assert::throws<std::logic_error>([&error] { error.raise(); });
    },

    task("swap") = [] {
      using std::swap;
      auto lhs_ptr = std::make_exception_ptr(std::logic_error { "" });
      auto rhs_ptr = std::make_exception_ptr(core::bad_expected_type { "" });

      core::expected<void> lhs { lhs_ptr };
      core::expected<void> rhs { rhs_ptr };

      assert::is_true(not lhs);
      assert::is_true(not rhs);
      assert::equal(lhs.pointer(), lhs_ptr);
      assert::equal(rhs.pointer(), rhs_ptr);

      swap(lhs, rhs);

      assert::is_true(not lhs);
      assert::is_true(not rhs);
      assert::equal(lhs.pointer(), rhs_ptr);
      assert::equal(rhs.pointer(), lhs_ptr);
    }
  };


  test("result<void>") = {
    task("default-constructor") = [] {
      core::result<void> value;
      assert::is_true(bool(value));
    },

    task("copy-constructor") = [] {
      core::result<void> value { };
      core::result<void> copy { value };
      assert::equal(copy, value);
    },

    task("move-constructor") = [] {
      core::result<void> value { };
      core::result<void> move { core::move(value) };
      assert::equal(value, move);
    },

    task("error-condition-constructor") = [] {
      auto const error = make_error_condition(std::errc::permission_denied);
      core::result<void> value { error };
      assert::is_false(bool(value));
      assert::equal(value, error);
    },

    task("error-condition-enum-constructor") = [] {
      core::result<void> value { std::errc::permission_denied };
      assert::is_false(bool(value));
      assert::equal(value, make_error_condition(std::errc::permission_denied));
    },

    task("copy-assign-operator") = [] {
      core::result<void> value { std::errc::permission_denied };
      core::result<void> copy { };

      assert::is_false(bool(value));
      value = copy;
      assert::is_true(bool(value));
    },

    task("move-assign-operator") = [] {
      core::result<void> value { std::errc::permission_denied };
      core::result<void> move { };
      assert::is_false(bool(value));
      value = core::move(move);
      assert::is_true(bool(value));
    },

    task("error-condition-assign-operator") = [] {
      core::result<void> value { };
      auto const error = make_error_condition(std::errc::permission_denied);
      assert::is_true(bool(value));
      value = error;
      assert::is_false(bool(value));
    },

    task("error-condition-enum-assign-operator") = [] {
      core::result<void> value { };
      assert::is_true(bool(value));
      value = std::errc::permission_denied;
      assert::is_false(bool(value));
    },

    task("operator-equal") = [] {
      core::result<void> lhs_valid { };
      core::result<void> rhs_valid { };
      core::result<void> lhs_invalid { std::errc::permission_denied };
      core::result<void> rhs_invalid { std::errc::permission_denied };

      assert::equal(lhs_valid, rhs_valid);
      assert::equal(lhs_invalid, rhs_invalid);
    },

    task("condition") = [] {
      auto const condition = make_error_condition(std::errc::permission_denied);
      core::result<void> valid { };
      core::result<void> invalid { condition };

      assert::is_true(bool(valid));
      assert::is_false(bool(invalid));

      assert::equal(invalid, condition);
      assert::throws<core::bad_result_condition>([&] {
        std::ignore = valid.condition();
      });
    },
  };

  monitor::run();
}
