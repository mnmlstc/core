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
