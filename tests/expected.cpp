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
      assert::fail();
    },

    task("move-value-assign-operator") = [] { assert::fail(); },
    task("move-assign-operator") = [] { assert::fail(); },
    task("copy-assign-operator") = [] { assert::fail(); },
    task("ptr-assign-operator") = [] { assert::fail(); },

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

    task("equality-comparable") = [] { assert::fail(); },
    task("less-than-comparable") = [] { assert::fail(); },
    task("value-or") = [] { assert::fail(); },
    task("value") = [] { assert::fail(); },
    task("expect") = [] { assert::fail(); },
    task("raise") = [] { assert::fail(); },
    task("swap") = [] { assert::fail(); }
  };

  monitor::run();
}
