#include <core/range.hpp>

#include <forward_list>
#include <vector>
#include <string>
#include <list>

#include <sstream>

#include <unittest/unittest.hpp>

int main () {
  using namespace unittest;

  test("range") = {

    task("iterator-constructor") = [] {
      std::string value { "iterator" };
      core::range<std::string::iterator> range { value.begin(), value.end() };
      assert::equal(value.begin(), range.begin());
      assert::equal(value.end(), range.end());
    },

    task("default-constructor") = [] {
      core::range<std::string::iterator> range { };
      assert::is_true(range.empty());
    },

    task("range-constructor") = [] {
      std::string value { "range" };
      core::range<std::string::iterator> range { value };
      assert::equal(value.begin(), range.begin());
      assert::equal(value.end(), range.end());
    },

    task("pair-constructor") = [] {
      std::string value { "pair" };
      core::range<std::string::iterator> range {
        std::make_pair(value.begin(), value.end())
      };
      assert::equal(value.begin(), range.begin());
      assert::equal(value.end(), range.end());
    },

    task("copy-constructor") = [] {
      std::string value { "copy" };
      core::range<std::string::iterator> range { value };
      core::range<std::string::iterator> copy { range };
      assert::equal(value.begin(), range.begin());
      assert::equal(value.begin(), copy.begin());
      assert::equal(value.end(), range.end());
      assert::equal(value.end(), copy.end());
    },

    task("move-constructor") = [] {
      std::string value { "move" };
      core::range<std::string::iterator> range { value };
      core::range<std::string::iterator> move { std::move(range) };

      assert::equal(value.begin(), move.begin());
      assert::equal(value.end(), move.end());
      assert::is_true(range.empty());
    },

    task("copy-assign-operator") = [] {
      std::string value { "copy" };
      core::range<std::string::iterator> range { value };
      core::range<std::string::iterator> copy { };

      assert::is_true(copy.empty());

      copy = range;

      assert::is_true(not copy.empty());
      assert::equal(copy.begin(), range.begin());
      assert::equal(copy.end(), range.end());
    },

    task("move-assign-operator") = [] {
      std::string value { "move" };
      core::range<std::string::iterator> range { value };
      core::range<std::string::iterator> move { };

      assert::is_true(move.empty());

      move = std::move(range);

      assert::is_true(not move.empty());
      assert::is_true(range.empty());

      assert::equal(value.begin(), move.begin());
      assert::equal(value.end(), move.end());
    },

    task("subscript-operator") = [] {
      std::string value { "subscript" };
      core::range<std::string::iterator> range { value };

      assert::is_true(not range.empty());
      assert::equal(
        static_cast<std::string::size_type>(range.size()),
        value.size()
      );

      assert::equal(range[0], 's');
      assert::equal(range[1], 'u');
      assert::equal(range[2], 'b');
      assert::equal(range[3], 's');
      assert::equal(range[4], 'c');
      assert::equal(range[5], 'r');
      assert::equal(range[6], 'i');
      assert::equal(range[7], 'p');
      assert::equal(range[8], 't');
    },

    task("begin") = [] {
      std::string value { "begin" };
      core::range<std::string::iterator> range { value };
      auto begin = std::begin(range);
      assert::equal(value.begin(), begin);
    },

    task("end") = [] {
      std::string value { "end" };
      core::range<std::string::iterator> range { value };
      auto end = std::end(range);
      assert::equal(value.end(), end);
    },

    task("front") = [] {
      std::string value { "front" };
      core::range<std::string::iterator> range { value };
      assert::equal(range.front(), 'f');
    },

    task("back") = [] {
      std::string value { "back" };
      core::range<std::string::iterator> range { value };
      assert::equal(range.back(), 'k');
    },

    task("empty") = [] {
      std::string value { };
      core::range<std::string::iterator> range { value };
      assert::is_true(range.empty());
    },

    task("size") = [] {
      std::string value { "size" };
      core::range<std::string::iterator> range { value };
      assert::equal(
        value.size(),
        static_cast<std::string::size_type>(range.size())
      );
    },

    task("slice") = [] {
      std::string value { "slice-a-long-string" };
      core::range<std::string::iterator> range { value };

      auto slice = range.slice(6);

      assert::equal(
        std::string { "a-long-string" },
        std::string { slice.begin(), slice.end() }
      );

      assert::equal(slice.begin(), range.begin() + 6);
      assert::equal(slice.end(), range.end());
      assert::equal(slice.size(), 13);

      slice = range.slice(-7);

      assert::equal(
        std::string { "-string" },
        std::string { slice.begin(), slice.end() }
      );

      assert::equal(slice.begin(), range.end() - 7);
      assert::equal(slice.end(), range.end());

      slice = range.slice(6, -7);

      assert::equal(
        std::string { "a-long" },
        std::string { slice.begin(), slice.end() }
      );

      /* bidirectional-range specific */
      std::string bidir_value { "bidirectional-range" };
      std::list<char> bidirectional { bidir_value.begin(), bidir_value.end() };
      core::range<std::list<char>::iterator> bidir { bidirectional };
      auto bislice = bidir.slice(2, -6);

      assert::equal(
        std::string { "directional" },
        std::string { bislice.begin(), bislice.end() }
      );

      /* forward-range specific */
      std::string forward_value { "a-forward-range" };
      std::forward_list<char> forward_list {
        forward_value.begin(),
        forward_value.end()
      };
      core::range<std::forward_list<char>::iterator> forward_range {
        forward_list
      };

      auto forward_slice = forward_range.slice(2, -6);
      assert::equal(
        std::string { "forward" },
        std::string { forward_slice.begin(), forward_slice.end() }
      );
    },

    task("split") = [] {
      std::string value { "splitstring" };
      core::range<std::string::iterator> range { value };
      auto pair = range.split(5);

      assert::equal(
        std::string { "split" },
        std::string { std::get<0>(pair).begin(), std::get<0>(pair).end() }
      );

      assert::equal(
        std::string { "string" },
        std::string { std::get<1>(pair).begin(), std::get<1>(pair).end() }
      );
    },

    task("pop-front") = [] {
      std::string value { "pop-front" };
      core::range<std::string::iterator> range { value };
      assert::is_false(range.empty());

      range.pop_front();

      assert::equal(range[0], 'o');
      assert::equal(range[1], 'p');
      assert::equal(range[2], '-');
      assert::equal(range[3], 'f');
      assert::equal(range[4], 'r');
      assert::equal(range[5], 'o');
      assert::equal(range[6], 'n');
      assert::equal(range[7], 't');

      range.pop_front(3);

      assert::equal(range[0], 'f');
      assert::equal(range[1], 'r');
      assert::equal(range[2], 'o');
      assert::equal(range[3], 'n');
      assert::equal(range[4], 't');

      range.pop_front(-4);

      assert::equal(range[0], 'p');
      assert::equal(range[1], 'o');
      assert::equal(range[2], 'p');
      assert::equal(range[3], '-');
      assert::equal(range[4], 'f');
      assert::equal(range[5], 'r');
      assert::equal(range[6], 'o');
      assert::equal(range[7], 'n');
      assert::equal(range[8], 't');
    },

    task("pop-back") = [] {
      std::string value { "pop-back" };
      core::range<std::string::iterator> range { value };

      assert::equal(range[0], 'p');
      assert::equal(range[1], 'o');
      assert::equal(range[2], 'p');
      assert::equal(range[3], '-');
      assert::equal(range[4], 'b');
      assert::equal(range[5], 'a');
      assert::equal(range[6], 'c');
      assert::equal(range[7], 'k');

      range.pop_back();

      assert::equal(range.size(), 7);

      assert::equal(range[0], 'p');
      assert::equal(range[1], 'o');
      assert::equal(range[2], 'p');
      assert::equal(range[3], '-');
      assert::equal(range[4], 'b');
      assert::equal(range[5], 'a');
      assert::equal(range[6], 'c');

      range.pop_back(4);

      assert::equal(range.size(), 3);

      assert::equal(range[0], 'p');
      assert::equal(range[1], 'o');
      assert::equal(range[2], 'p');

      range.pop_back(-5);

      assert::equal(range.size(), value.size());

      assert::equal(range[0], 'p');
      assert::equal(range[1], 'o');
      assert::equal(range[2], 'p');
      assert::equal(range[3], '-');
      assert::equal(range[4], 'b');
      assert::equal(range[5], 'a');
      assert::equal(range[6], 'c');
      assert::equal(range[7], 'k');
    },

    task("pop-front-upto") = [] {
      std::string value { "pop-front-upto" };
      core::range<std::string::iterator> range { value };

      assert::equal(value.begin(), range.begin());
      assert::equal(value.end(), range.end());

      range.pop_front_upto(4);

      assert::not_equal(value.begin(), range.begin());
      assert::equal(value.end(), range.end());

      assert::equal(range[0], 'f');
      assert::equal(range[1], 'r');
      assert::equal(range[2], 'o');
      assert::equal(range[3], 'n');
      assert::equal(range[4], 't');
      assert::equal(range[5], '-');
      assert::equal(range[6], 'u');
      assert::equal(range[7], 'p');
      assert::equal(range[8], 't');
      assert::equal(range[9], 'o');

      /* Attempt to pop backwards and WAAAY past the beginning */
      range.pop_front_upto(-15);

      assert::equal(range.size(), 10);
    },

    task("pop-back-upto") = [] {
      std::string value { "pop-back-upto" };
      core::range<std::string::iterator> range { value };

      assert::equal(value.begin(), range.begin());
      assert::equal(value.end(), range.end());
      range.pop_back_upto(5);

      assert::not_equal(value.end(), range.end());
      assert::equal(value.begin(), range.begin());
      assert::equal(range.size(), 8);

      assert::equal(range[0], 'p');
      assert::equal(range[1], 'o');
      assert::equal(range[2], 'p');
      assert::equal(range[3], '-');
      assert::equal(range[4], 'b');
      assert::equal(range[5], 'a');
      assert::equal(range[6], 'c');
      assert::equal(range[7], 'k');

      /* Attempt to pop backwards and waaaay past the end */
      range.pop_back_upto(-15);

      assert::equal(range.size(), 8);
    },

    task("swap") = [] {
      using std::swap;
      std::string second { "second" };
      std::string first { "first" };

      core::range<std::string::iterator> lhs { first };
      core::range<std::string::iterator> rhs { second };

      assert::is_false(lhs.empty());
      assert::is_false(rhs.empty());

      assert::equal(lhs.size(), first.size());
      assert::equal(rhs.size(), second.size());

      assert::equal(lhs[0], 'f');
      assert::equal(lhs[1], 'i');
      assert::equal(lhs[2], 'r');
      assert::equal(lhs[3], 's');
      assert::equal(lhs[4], 't');

      assert::equal(rhs[0], 's');
      assert::equal(rhs[1], 'e');
      assert::equal(rhs[2], 'c');
      assert::equal(rhs[3], 'o');
      assert::equal(rhs[4], 'n');
      assert::equal(rhs[5], 'd');

      swap(lhs, rhs);

      assert::is_false(lhs.empty());
      assert::is_false(rhs.empty());

      assert::equal(lhs.size(), second.size());
      assert::equal(rhs.size(), first.size());

      assert::equal(rhs[0], 'f');
      assert::equal(rhs[1], 'i');
      assert::equal(rhs[2], 'r');
      assert::equal(rhs[3], 's');
      assert::equal(rhs[4], 't');

      assert::equal(lhs[0], 's');
      assert::equal(lhs[1], 'e');
      assert::equal(lhs[2], 'c');
      assert::equal(lhs[3], 'o');
      assert::equal(lhs[4], 'n');
      assert::equal(lhs[5], 'd');
    },

    task("make-range") = [] {
      std::string value { "make-range" };
      auto string_range = core::make_range(value);

      assert::equal(
        value,
        std::string { string_range.begin(), string_range.end() }
      );

      std::istringstream stream { value };
      auto stream_range = core::make_range<char>(stream);

      std::ostringstream ostream { };
      for (auto ch : stream_range) { ostream << ch; }

      assert::equal(
        value,
        ostream.str()
      );

      std::istringstream stream2 { value };
      auto buf_range = core::make_range(stream2.rdbuf());

      assert::equal(
        value,
        std::string { buf_range.begin(), buf_range.end() }
      );
    }
  };

  monitor::run();
}
