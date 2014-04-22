#include <core/optional.hpp>

#include <unordered_map>
#include <utility>
#include <string>
#include <array>

#include <cstdint>

#include <unittest/unittest.hpp>

struct variadic {
  template <class... Args> constexpr variadic (Args&&...) { }
  template <class... Args>
  constexpr variadic (std::initializer_list<int>, Args&&...) { }
};


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
      assert::is_false(value);
    },

    task("nullopt-constructor") = [] {
      constexpr core::optional<int> opt { core::nullopt };
      constexpr bool value = static_cast<bool>(opt);
      assert::is_false(value);
    },

    task("value-constructor") = [] {
      constexpr int value = 7;
      constexpr core::optional<int> copy { value };
      constexpr core::optional<int> move { 5 };
      static_assert(copy, "could not initialize core::optional<int>");
      static_assert(move, "could not initialize core::optional<int>");
      constexpr bool copy_result = *copy == value;
      constexpr bool move_result = *move == 5;
      assert::is_true(copy_result);
      assert::is_true(move_result);
    },

    task("variadic-value-constructor") = [] {
      constexpr core::optional<variadic> list { core::in_place, { 1, 2, 3, 4 } };
      constexpr core::optional<variadic> nolist { core::in_place, 1, 2.0f, "words" };
      assert::is_false(not list);
      assert::is_false(not nolist);
    },

    task("value") = [] {
      constexpr core::optional<int> opt { 5 };
      assert::is_false(not opt);
      assert::equal(opt.value(), 5);
    },

    task("value-or") = [] {
      constexpr core::optional<int> engaged { 5 };
      constexpr core::optional<int> disengaged { };
      assert::is_false(not engaged);
      assert::is_true(not disengaged);
      assert::equal(engaged.value_or(7), 5);
      assert::equal(disengaged.value_or(7), 7);
    },

    task("make-optional") = [] {
      constexpr auto opt = core::make_optional(5);
      assert::is_false(not opt);
      assert::equal(opt.value(), 5);
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

      assert::is_true(first_result);
      assert::is_true(second_result);
      assert::is_true(third_result);
      assert::is_true(fourth_result);
      assert::is_true(fifth_result);
    },

    task("operator-not-equal") = [] { assert::fail(); },
    task("operator-greater-equal") = [] { assert::fail(); },
    task("operator-less-equal") = [] { assert::fail(); },
    task("operator-greater") = [] { assert::fail(); },

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
      assert::is_true(first_result);
      assert::is_true(second_result);
      assert::is_true(third_result);
      assert::is_true(fourth_result);
      assert::is_true(fifth_result);
    }
  };

  monitor::run();
}
