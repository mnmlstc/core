#include <core/any.hpp>
#include <type_traits>
#include <string>

#include <cstdint>

#include <unittest/unittest.hpp>

int main () {
  using namespace unittest;

  test("any") = {
    task("default-constructor") = [] {
      core::any value;

      assert::is_true(value.empty());
      assert::equal(value.type(), typeid(void));
    },
    task("value-constructor") = [] {
      std::string text { "value-constructor" };
      core::any copy { text };

      assert::is_false(copy.empty());
      assert::is_false(text.empty());
      assert::equal(copy.type(), typeid(text));

      core::any move { std::move(text) };

      assert::is_false(move.empty());
      assert::is_true(text.empty());
      assert::equal(move.type(), typeid(text));
    },

    task("copy-constructor") = [] {
      core::any value { std::string { "copy-constructor" } };
      core::any ctor { value };

      assert::is_false(value.empty());
      assert::is_false(ctor.empty());
      assert::equal(ctor.type(), value.type());
    },

    task("move-constructor") = [] {
      core::any value { std::string { "move-constructor" } };
      core::any ctor { std::move(value) };

      assert::is_false(ctor.empty());
      assert::is_true(value.empty());
      assert::equal(ctor.type(), typeid(std::string));
    },

    task("rvalue-assign") = [] {
      std::string text { "rvalue-assign works!" };
      core::any value;
      value = std::move(text);

      assert::is_false(value.empty());
      assert::is_true(text.empty());
      assert::equal(value.type(), typeid(text));
    },

    task("lvalue-assign") = [] {
      std::uint64_t integer = 42;
      core::any value;
      value = integer;
      assert::is_false(value.empty());
      assert::equal(value.type(), typeid(integer));
    },

    task("copy-assign") = [] {
      std::uint64_t integer = 42;
      core::any value { integer };
      core::any assigned;
      assigned = value;
      assert::is_false(value.empty());
      assert::is_false(assigned.empty());
      assert::equal(typeid(integer), assigned.type());
    },

    task("move-assign") = [] {
      std::uint64_t integer = 42;
      core::any value { integer };
      core::any assigned;
      assigned = std::move(value);
      assert::is_true(value.empty());
      assert::is_false(assigned.empty());
      assert::equal(typeid(integer), assigned.type());
    },

    task("value-assign") = [] {
      std::uint64_t integer { 42 };
      core::any value { };
      value = integer;
      assert::is_false(value.empty());
      assert::equal(typeid(integer), value.type());
      assert::equal(core::any_cast<std::uint64_t>(value), 42u);
    },

    task("swap") = [] {
      using std::swap;
      std::uint64_t integer = 42;
      core::any value { integer };
      core::any to_swap { };
      swap(value, to_swap);
      assert::is_true(value.empty());
      assert::is_false(to_swap.empty());
    },

    task("clear") = [] {
      core::any value { std::string { "clear" } };
      assert::is_false(value.empty());
      value.clear();
      assert::is_true(value.empty());
    },

    task("type") = [] {
      std::uint64_t integer = 42;
      core::any value { integer };
      assert::equal(typeid(integer), value.type());
    },

    task("cast-const-ref") = [] {
      std::uint64_t integer = 42;
      core::any const value { integer };
      auto integer_value = core::any_cast<std::uint64_t>(value);

      assert::equal(integer_value, integer);
      assert::throws<core::bad_any_cast>([&value] {
        core::any_cast<double>(value);
      });
    },

    task("cast-const-ptr") = [] {
      std::uint64_t integer = 42;
      core::any const value { integer };
      auto integer_ptr = core::any_cast<std::uint64_t>(std::addressof(value));
      auto double_ptr = core::any_cast<double>(std::addressof(value));

      assert::is_not_null(integer_ptr);
      assert::is_null(double_ptr);
      assert::equal(*integer_ptr, integer);
      assert::is_true(std::is_pointer<decltype(integer_ptr)>::value);
    },

    task("cast-ref") = []{
      std::uint64_t integer = 42;
      core::any value { integer };
      auto integer_value = core::any_cast<std::uint64_t>(value);

      assert::equal(integer_value, integer);
      assert::throws<core::bad_any_cast>([&value]{
        core::any_cast<double>(value);
      });
    },

    task("cast-ptr") = []{
      std::uint64_t integer = 42;
      core::any value { integer };
      auto integer_ptr = core::any_cast<std::uint64_t>(std::addressof(value));
      auto double_ptr = core::any_cast<double>(std::addressof(value));

      assert::is_not_null(integer_ptr);
      assert::is_null(double_ptr);
      assert::equal(*integer_ptr, integer);
    },
  };
  monitor::run();
}
