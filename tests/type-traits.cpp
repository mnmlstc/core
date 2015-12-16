#include <core/type_traits.hpp>

#include "catch.hpp"

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

TEST_CASE("type-traits") {
  SECTION("is-null-pointer") {
    using null = std::nullptr_t;

    CHECK(core::is_null_pointer<null>::value);
    CHECK(core::is_null_pointer<null const>::value);
    CHECK(core::is_null_pointer<null volatile>::value);
    CHECK(core::is_null_pointer<null volatile const>::value);
  }

  SECTION("is-nothrow-swappable") {
    CHECK(core::is_nothrow_swappable<int>::value);
    CHECK_FALSE(core::is_nothrow_swappable<A>::value);
    CHECK_FALSE(core::is_nothrow_swappable<B>::value);
    CHECK(core::is_nothrow_swappable<C>::value);
    CHECK(core::is_nothrow_swappable<D>::value);
  }
}
