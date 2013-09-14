#include <core/range.hpp>

#include <vector>
#include <string>

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
      assert::equal(range.size(), value.size());

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
      assert::equal(value.size(), range.size());
    },

    task("slice") = [] { assert::fail(); },
    task("split") = [] { assert::fail(); },
    task("pop-front") = [] { assert::fail(); },
    task("pop-back") = [] { assert::fail(); },
    task("pop-front-upto") = [] { assert::fail(); },

    task("swap") = [] {
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

      std::swap(lhs, rhs);

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
    }
  };

  monitor::run();
}
