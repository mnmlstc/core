#include <core/iterator.hpp>
#include <algorithm>
#include <ostream>
#include <sstream>
#include <vector>

#include <cstdint>

#include <unittest/unittest.hpp>

template <class T>
std::ostream& operator << (std::ostream& os, std::vector<T> const& vec) {
  os << "[";
  std::copy(
    core::cbegin(vec),
    core::cend(vec),
    core::infix_ostream_iterator<T> { os, "," }
  );
  return os << "]";
}

int main () {
  using namespace unittest;
  std::vector<std::uint64_t> value = { 1, 2, 3, 4, 5 };

  test("iterator") = {
    task("size") = [value] {
      int array[5];
      assert::equal(core::size(value), value.size());
      static_assert(core::size(array) == 5, "core::size");
    },

    task("empty") = [value] {
      int array[5];
      assert::is_false(core::empty(value));
      static_assert(not core::empty(array), "core::empty");
    },

    task("front") = [value] {
      constexpr int array[5] { 0, 1, 2, 3, 4 };
      assert::equal(core::front(value), value.front());
      static_assert(core::front(array) == 0, "core::front");
    },

    task("back") = [value] {
      constexpr int array[5] { 0, 1, 2, 3, 4 };
      assert::equal(core::back(value), value.back());
      static_assert(core::back(array) == 4, "core::back");
    },

    task("data") = [value] {
      int array[5] { 1, 2, 3, 4, 5 };
      assert::equal(core::data(value), value.data());
      assert::equal(core::data(array), std::addressof(array[0]));
    },

    task("cbegin") = [value] {
      assert::equal(core::cbegin(value), value.begin());
    },
    task("cend") = [value] { assert::equal(core::cend(value), value.end()); },

    task("infix-ostream-iterator") = [value] {
      std::ostringstream stream;
      stream << value;
      assert::equal(stream.str(), std::string { "[1,2,3,4,5]" });
    }
  };

  monitor::run();
}
