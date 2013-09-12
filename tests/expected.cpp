#include <core/expected.hpp>

#include <string>

#include <unittest/unittest.hpp>

struct type {
  type (type const&) = default;
  type (type&&) = default;
  type () = default;
  ~type () { std::clog << "It's working!" << std::endl; }

  type& operator = (type const&) = default;
  type& operator = (type&&) = default;
};

int main () {
  using namespace unittest;

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
      core::expected<int> throws {
        std::make_exception_ptr(std::logic_error { "" })
      };

      assert::is_true(bool(nothrow));
      assert::is_true(not throws);

      assert::equal(*nothrow, 51);
      assert::throws<std::logic_error>([&throws] {
        int value = *throws;
        std::ignore = value;
      });
    },

    task("equality-comparable") = [] {
      core::expected<int> lhs { 5 };
      core::expected<int> rhs { 6 };

      assert::is_true(bool(lhs));
      assert::is_true(bool(rhs));
      assert::equal(lhs, 5);
      assert::equal(6, rhs);
      assert::equal(lhs, core::expected<int> { 5 });
      assert::equal(core::expected<int> { 6 }, rhs);
    },

    task("less-than-comparable") = [] {
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
      auto ptr = std::make_exception_ptr(std::logic_error { "swap" });
      core::expected<int> error { ptr };
      core::expected<int> value { 5 };

      assert::is_true(bool(value));
      assert::is_true(not error);

      std::swap(value, error);

      assert::is_true(bool(error));
      assert::is_true(not value);
    },

    task("make_expected") = [] {
      auto value = core::make_expected(std::string { "make-expected" });
      auto error = core::make_expected<std::string>(
        std::make_exception_ptr(std::logic_error { "error" })
      );

      assert::is_true(bool(value));
      assert::is_true(not error);

      assert::equal(*value, std::string { "make-expected" });
    }
  };

  test("expected<void>") = {
    task("default-constructor") = [] { assert::fail(); },
    task("copy-constructor") = [] { assert::fail(); },
    task("move-constructor") = [] { assert::fail(); },
    task("ptr-constructor") = [] { assert::fail(); },
    task("copy-assign-operator") = [] { assert::fail(); },
    task("move-assign-operator") = [] { assert::fail(); },
    task("equality-comparable") = [] { assert::fail(); },
    task("expect") = [] { assert::fail(); },
    task("raise") = [] { assert::fail(); },
    task("swap") = [] { assert::fail(); }
  };

  monitor::run();
}
