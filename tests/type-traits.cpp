#include <core/type_traits.hpp>

#include <unittest/unittest.hpp>

namespace {

struct A { A (A const&); };
struct B { };
struct C { };
struct D {
  D (D const&) noexcept;
  D& operator = (D const&) noexcept;
};

struct E {
  E (E const&) = delete;
  E (E&&) = delete;
  E& operator = (E const&) = delete;
  E& operator = (E&&) = delete;
};

void swap (B&, B&);
void swap (C&, C&) noexcept;

} /* nameless namespace */

int main () {
  using namespace unittest;

  test("type-traits") = {
    task("is-null-pointer") = [] {
      using null = std::nullptr_t;

      assert::is_true(core::is_null_pointer<null>::value);
      assert::is_true(core::is_null_pointer<null const>::value);
      assert::is_true(core::is_null_pointer<null volatile>::value);
      assert::is_true(core::is_null_pointer<null volatile const>::value);
    },

    task("is-nothrow-swappable") = [] {
      assert::is_true(core::is_nothrow_swappable<int>::value);
      assert::is_false(core::is_nothrow_swappable<A>::value);
      assert::is_false(core::is_nothrow_swappable<B>::value);
      assert::is_true(core::is_nothrow_swappable<C>::value);
      assert::is_true(core::is_nothrow_swappable<D>::value);
    },

    task("is-swappable") = [] {
      assert::is_true(core::is_swappable<A>::value);
      assert::is_true(core::is_swappable<B>::value);
      assert::is_true(core::is_swappable<C>::value);
      assert::is_true(core::is_swappable<D>::value);
    }
  };

  monitor::run();
}
