#include <core/iterator.hpp>
#include <vector>

#include <cstdint>

#include <unittest/unittest.hpp>

int main () {
  using namespace unittest;
  std::vector<std::uint64_t> value = { 1, 2, 3, 4, 5 };

  test("iterator") = {
    task("cbegin") = [value] {
      assert::equal(core::cbegin(value), value.begin());
    },
    task("cend") = [value] { assert::equal(core::cend(value), value.end()); }
  };
}
