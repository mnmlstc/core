#include <core/range.hpp>

#include <forward_list>
#include <vector>
#include <string>
#include <list>

#include <sstream>

#include "catch.hpp"

TEST_CASE("range-constructors", "[range][constructors]") {
  SECTION("iterator") {
    std::string value { "iterator" };
    core::range<std::string::iterator> range { value.begin(), value.end() };
    CHECK(value.begin() == range.begin());
    CHECK(value.end() == range.end());
  }

  SECTION("default") {
    core::range<std::string::iterator> range { };
    CHECK(range.empty());
  }

  SECTION("range") {
    std::string value { "range" };
    core::range<std::string::iterator> range { value };
    CHECK(value.begin() == range.begin());
    CHECK(value.end() == range.end());
  }

  SECTION("pair") {
    std::string value { "pair" };
    core::range<std::string::iterator> range {
      std::make_pair(value.begin(), value.end())
    };
    CHECK(value.begin() == range.begin());
    CHECK(value.end() == range.end());
  }

  SECTION("copy") {
    std::string value { "copy" };
    core::range<std::string::iterator> range { value };
    core::range<std::string::iterator> copy { range };
    CHECK(value.begin() == range.begin());
    CHECK(value.begin() == copy.begin());
    CHECK(value.end() == range.end());
    CHECK(value.end() == copy.end());
  }

  SECTION("move") {
    std::string value { "move" };
    core::range<std::string::iterator> range { value };
    core::range<std::string::iterator> move { std::move(range) };

    CHECK(value.begin() == move.begin());
    CHECK(value.end() == move.end());
    CHECK(range.empty());
  }
}

TEST_CASE("range-assignment", "[range][assignment]") {
  SECTION("copy") {
    std::string value { "copy" };
    core::range<std::string::iterator> range { value };
    core::range<std::string::iterator> copy { };

    CHECK(copy.empty());

    copy = range;

    CHECK_FALSE(copy.empty());
    CHECK(copy.begin() == range.begin());
    CHECK(copy.end() == range.end());
  }

  SECTION("move") {
    std::string value { "move" };
    core::range<std::string::iterator> range { value };
    core::range<std::string::iterator> move { };

    CHECK(move.empty());

    move = std::move(range);

    CHECK_FALSE(move.empty());
    CHECK(range.empty());

    CHECK(value.begin() == move.begin());
    CHECK(value.end() == move.end());
  }
}

TEST_CASE("range-methods", "[range][methods]") {
  SECTION("begin") {
    std::string value { "begin" };
    core::range<std::string::iterator> range { value };
    auto begin = std::begin(range);
    CHECK(value.begin() == begin);
  }

  SECTION("end") {
    std::string value { "end" };
    core::range<std::string::iterator> range { value };
    auto end = std::end(range);
    CHECK(value.end() == end);
  }

  SECTION("front") {
    std::string value { "front" };
    core::range<std::string::iterator> range { value };
    CHECK(range.front() == 'f');
  }

  SECTION("back") {
    std::string value { "back" };
    core::range<std::string::iterator> range { value };
    CHECK(range.back() == 'k');
  }

  SECTION("empty") {
    std::string value { };
    core::range<std::string::iterator> range { value };
    CHECK(range.empty());
  }

  SECTION("size") {
    std::string value { "size" };
    core::range<std::string::iterator> range { value };
    CHECK(
      value.size() ==
      static_cast<std::string::size_type>(range.size()));
  }

  SECTION("slice") {
    std::string value { "slice-a-long-string" };
    core::range<std::string::iterator> range { value };

    auto slice = range.slice(6);

    CHECK(("a-long-string" == std::string { slice.begin(), slice.end() }));

    CHECK(slice.begin() == range.begin() + 6);
    CHECK(slice.end() == range.end());
    CHECK(slice.size() == 13);

    slice = range.slice(-7);

    CHECK(("-string" == std::string { slice.begin(), slice.end() }));

    CHECK(slice.begin() == range.end() - 7);
    CHECK(slice.end() == range.end());

    slice = range.slice(6, -7);

    CHECK(("a-long" == std::string { slice.begin(), slice.end() }));
  }

  SECTION("slice-bidirectional") {
    std::string bidir_value { "bidirectional-range" };
    std::list<char> bidirectional { bidir_value.begin(), bidir_value.end() };
    core::range<std::list<char>::iterator> bidir { bidirectional };
    auto bislice = bidir.slice(2, -6);

    CHECK(("directional" == std::string { bislice.begin(), bislice.end() }));
  }

  SECTION("slice-forward") {
    // forward-range specific
    std::string forward_value { "a-forward-range" };
    std::forward_list<char> forward_list {
      forward_value.begin(),
      forward_value.end()
    };
    core::range<std::forward_list<char>::iterator> forward_range {
      forward_list
    };

    auto fslice = forward_range.slice(2, -6);
    CHECK(("forward" == std::string { fslice.begin(), fslice.end() }));
  }

  SECTION("split") {
    std::string value { "splitstring" };
    core::range<std::string::iterator> range { value };
    auto pair = range.split(5);
    std::string string { std::get<1>(pair).begin(), std::get<1>(pair).end() };
    std::string split { std::get<0>(pair).begin(), std::get<0>(pair).end() };

    CHECK("string" == string);
    CHECK("split" == split);
  }

  SECTION("pop-front") {
    std::string value { "pop-front" };
    core::range<std::string::iterator> range { value };
    CHECK_FALSE(range.empty());

    range.pop_front();

    CHECK(range[0] == 'o');
    CHECK(range[1] == 'p');
    CHECK(range[2] == '-');
    CHECK(range[3] == 'f');
    CHECK(range[4] == 'r');
    CHECK(range[5] == 'o');
    CHECK(range[6] == 'n');
    CHECK(range[7] == 't');

    range.pop_front(3);

    CHECK(range[0] == 'f');
    CHECK(range[1] == 'r');
    CHECK(range[2] == 'o');
    CHECK(range[3] == 'n');
    CHECK(range[4] == 't');

    range.pop_front(-4);

    CHECK(range[0] == 'p');
    CHECK(range[1] == 'o');
    CHECK(range[2] == 'p');
    CHECK(range[3] == '-');
    CHECK(range[4] == 'f');
    CHECK(range[5] == 'r');
    CHECK(range[6] == 'o');
    CHECK(range[7] == 'n');
    CHECK(range[8] == 't');
  }

  SECTION("pop-back") {
    std::string value { "pop-back" };
    core::range<std::string::iterator> range { value };

    CHECK(range[0] == 'p');
    CHECK(range[1] == 'o');
    CHECK(range[2] == 'p');
    CHECK(range[3] == '-');
    CHECK(range[4] == 'b');
    CHECK(range[5] == 'a');
    CHECK(range[6] == 'c');
    CHECK(range[7] == 'k');

    range.pop_back();

    CHECK(range.size() == 7);

    CHECK(range[0] == 'p');
    CHECK(range[1] == 'o');
    CHECK(range[2] == 'p');
    CHECK(range[3] == '-');
    CHECK(range[4] == 'b');
    CHECK(range[5] == 'a');
    CHECK(range[6] == 'c');

    range.pop_back(4);

    CHECK(range.size() == 3);

    CHECK(range[0] == 'p');
    CHECK(range[1] == 'o');
    CHECK(range[2] == 'p');

    range.pop_back(-5);

    CHECK(range.size() == value.size());

    CHECK(range[0] == 'p');
    CHECK(range[1] == 'o');
    CHECK(range[2] == 'p');
    CHECK(range[3] == '-');
    CHECK(range[4] == 'b');
    CHECK(range[5] == 'a');
    CHECK(range[6] == 'c');
    CHECK(range[7] == 'k');
  }

  SECTION("pop-front-upto") {
    std::string value { "pop-front-upto" };
    core::range<std::string::iterator> range { value };

    CHECK(value.begin() == range.begin());
    CHECK(value.end() == range.end());

    range.pop_front_upto(4);

    CHECK(value.begin() != range.begin());
    CHECK(value.end() == range.end());

    CHECK(range[0] == 'f');
    CHECK(range[1] == 'r');
    CHECK(range[2] == 'o');
    CHECK(range[3] == 'n');
    CHECK(range[4] == 't');
    CHECK(range[5] == '-');
    CHECK(range[6] == 'u');
    CHECK(range[7] == 'p');
    CHECK(range[8] == 't');
    CHECK(range[9] == 'o');

    // Attempt to pop backwards and WAAAY past the beginning
    range.pop_front_upto(-15);

    CHECK(range.size() == 10);
  }

  SECTION("pop-back-upto") {
    std::string value { "pop-back-upto" };
    core::range<std::string::iterator> range { value };

    CHECK(value.begin() == range.begin());
    CHECK(value.end() == range.end());
    range.pop_back_upto(5);

    CHECK(value.end() != range.end());
    CHECK(value.begin() == range.begin());
    CHECK(range.size() == 8);

    CHECK(range[0] == 'p');
    CHECK(range[1] == 'o');
    CHECK(range[2] == 'p');
    CHECK(range[3] == '-');
    CHECK(range[4] == 'b');
    CHECK(range[5] == 'a');
    CHECK(range[6] == 'c');
    CHECK(range[7] == 'k');

    // Attempt to pop backwards and waaaay past the end
    range.pop_back_upto(-15);

    CHECK(range.size() == 8);
  }
}

TEST_CASE("range-operators", "[range][operators]") {
  SECTION("subscript") {
    std::string value { "subscript" };
    core::range<std::string::iterator> range { value };
    auto const size = static_cast<std::string::size_type>(range.size());

    CHECK_FALSE(range.empty());
    CHECK(size ==value.size());

    CHECK(range[0] == 's');
    CHECK(range[1] == 'u');
    CHECK(range[2] == 'b');
    CHECK(range[3] == 's');
    CHECK(range[4] == 'c');
    CHECK(range[5] == 'r');
    CHECK(range[6] == 'i');
    CHECK(range[7] == 'p');
    CHECK(range[8] == 't');
  }
}

TEST_CASE("range-functions", "[range][functions]") {
  SECTION("swap") {
    using std::swap;
    std::string second { "second" };
    std::string first { "first" };

    core::range<std::string::iterator> lhs { first };
    core::range<std::string::iterator> rhs { second };

    CHECK_FALSE(lhs.empty());
    CHECK_FALSE(rhs.empty());

    CHECK(lhs.size() == first.size());
    CHECK(rhs.size() == second.size());

    CHECK(lhs[0] == 'f');
    CHECK(lhs[1] == 'i');
    CHECK(lhs[2] == 'r');
    CHECK(lhs[3] == 's');
    CHECK(lhs[4] == 't');

    CHECK(rhs[0] == 's');
    CHECK(rhs[1] == 'e');
    CHECK(rhs[2] == 'c');
    CHECK(rhs[3] == 'o');
    CHECK(rhs[4] == 'n');
    CHECK(rhs[5] == 'd');

    swap(lhs, rhs);

    CHECK_FALSE(lhs.empty());
    CHECK_FALSE(rhs.empty());

    CHECK(lhs.size() == second.size());
    CHECK(rhs.size() == first.size());

    CHECK(rhs[0] == 'f');
    CHECK(rhs[1] == 'i');
    CHECK(rhs[2] == 'r');
    CHECK(rhs[3] == 's');
    CHECK(rhs[4] == 't');

    CHECK(lhs[0] == 's');
    CHECK(lhs[1] == 'e');
    CHECK(lhs[2] == 'c');
    CHECK(lhs[3] == 'o');
    CHECK(lhs[4] == 'n');
    CHECK(lhs[5] == 'd');
  }

  SECTION("make-range") {
    std::string value { "make-range" };
    auto string_range = core::make_range(value);

    CHECK((value == std::string { string_range.begin(), string_range.end() }));

    std::istringstream stream { value };
    auto stream_range = core::make_range<char>(stream);

    std::ostringstream ostream { };
    for (auto ch : stream_range) { ostream << ch; }

    CHECK(value == ostream.str());

    std::istringstream stream2 { value };
    auto buf_range = core::make_range(stream2.rdbuf());

    CHECK((value == std::string { buf_range.begin(), buf_range.end() }));
  }
}
