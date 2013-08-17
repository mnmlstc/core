#include <core/any.hpp>
#include <cstdint>

#include <unittest/unittest.hpp>

int main () {
  using namespace unittest;

  test("any") = {
    task("allocator-constructor") = [] { assert::fail(); },
    task("default-constructor") = [] { assert::fail(); },
    task("value-constructor") = [] { assert::fail(); },
    task("copy-constructor") = [] { assert::fail(); },
    task("move-constructor") = [] { assert::fail(); },

    task("rvalue-assign") = [] {
      assert::fail();
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

    task("swap") = [] {
      std::uint64_t integer = 42;
      core::any value { integer };
      core::any to_swap { };
      std::swap(value, to_swap);
      assert::is_true(value.empty());
      assert::is_false(to_swap.empty());
    },

    task("type") = [] {
      std::uint64_t integer = 42;
      core::any value { integer };
      assert::equal(typeid(integer), value.type());
    },

    task("cast-const-ref") = [] { assert::fail(); },
    task("cast-const-ptr") = [] { assert::fail(); },
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
      using core::any_cast;
      std::uint64_t integer = 42;
      core::any value { integer };
      auto integer_ptr = core::any_cast<std::uint64_t>(std::addressof(value));
      auto double_ptr = core::any_cast<double>(std::addressof(value));

      assert::is_not_null(integer_ptr);
      assert::is_null(double_ptr);
      assert::equal(*integer_ptr, integer);
    },
  };

}
