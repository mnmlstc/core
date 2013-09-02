#include <core/optional.hpp>

#include <unordered_map>
#include <utility>
#include <string>

#include <cstdint>

#include <unittest/unittest.hpp>

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
      assert::is_true(not opt);
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

      assert::is_true(not opt);
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
      });
    },

    task("value_or") = [] {
      assert::fail();
    },

    task("emplace") = [] {
      core::optional<int> opt { };
      opt.emplace(4);
      assert::is_false(not opt);
      assert::equal(*opt, 4);
    },

    task("equality-comparable") = [] { assert::fail(); },

    task("less-than-comparable") = [] { assert::fail(); },

    task("make-optional") = [] {
      auto opt = core::make_optional<std::string>("make-optional");
      assert::is_true(bool(opt));
      assert::equal(*opt, std::string { "make-optional" });
    },

    task("hash") = [] {
      std::unordered_map<core::optional<std::string>, int> values = {
        std::make_pair(core::make_optional<std::string>("text1"), 0),
        std::make_pair(core::make_optional<std::string>("text2"), 1),
        std::make_pair(core::make_optional<std::string>("text3"), 2),
      };

      assert::fail();
    }
  };

  monitor::run();
}
