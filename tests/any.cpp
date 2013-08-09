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

    task("rvalue-assign") = [] { assert::fail(); },
    task("lvalue-assign") = [] { assert::fail(); },
    task("copy-assign") = [] { assert::fail(); },
    task("move-assign") = [] { assert::fail(); },

    task("swap") = [] { assert::fail(); },
    task("type") = [] { assert::fail(); },

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
