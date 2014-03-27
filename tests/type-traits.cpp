#include <core/type_traits.hpp>

#include <unittest/unittest.hpp>

int main () {
  using namespace unittest;

  test("type-traits") = {
    task("is-null-pointer") = [] {
      using null = std::nullptr_t;

      assert::is_true(core::is_null_pointer<null>::value);
      assert::is_true(core::is_null_pointer<null const>::value);
      assert::is_true(core::is_null_pointer<null volatile>::value);
      assert::is_true(core::is_null_pointer<null volatile const>::value);
    }
  };

  monitor::run();
}
