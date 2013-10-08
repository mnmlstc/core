#include <core/string.hpp>

#include <unordered_map>
#include <sstream>
#include <cstring>

#include <unittest/unittest.hpp>

int main () {
  using namespace unittest;

  test("basic-string-ref") = {
    task("default-constructor") = [] {
      constexpr core::string_ref ref { };
      assert::equal(ref.length(), 0);
      assert::equal(ref.size(), 0);
      assert::is_true(ref.empty());
      assert::is_null(ref.data());
    },

    task("copy-constructor") = [] {
      constexpr core::string_ref ref { "hello!", 6 };

      assert::equal(ref.length(), 6);
      assert::equal(ref.size(), 6);
      assert::is_false(ref.empty());
      assert::is_not_null(ref.data());

      constexpr core::string_ref copy { ref };

      assert::equal(copy.length(), ref.length());
      assert::equal(copy.size(), ref.size());
      assert::equal(copy.empty(), ref.empty());
      assert::equal(copy.data(), ref.data());

      assert::equal(copy, ref);
    },

    task("string-constructor") = [] {
      std::string str { "string-ctor" };
      core::string_ref ref { str };

      assert::is_not_null(ref.data());
      assert::is_false(ref.empty());
      assert::equal(ref.size(), 11);
      assert::is(ref.data(), str.data());
    },

    task("pointer-constructor") = [] {
      auto str = "pointer-constructor";
      core::string_ref ref { str };

      assert::is_not_null(ref.data());
      assert::is_false(ref.empty());

      assert::equal(ref.size(), std::strlen(str));
      assert::is(ref.data(), str);
    },

    task("pointer-len-constructor") = [] {
      constexpr core::string_ref ref { "pointer-len", 11 };

      assert::is_not_null(ref.data());
      assert::is_false(ref.empty());
      assert::equal(ref.size(), 11);
    },

    task("copy-assignment-operator") = [] {
      core::string_ref ref { "copy-assign" };
      core::string_ref copy { };
      copy = ref;

      assert::is_false(copy.empty());
      assert::is_false(ref.empty());
      assert::equal(copy.size(), ref.size());
      assert::is(copy.data(), ref.data());

      assert::equal(copy, ref);
    },

    task("explicit-string-cast") = [] {
      core::string_ref ref { "explicit-string-cast" };
      auto str = static_cast<std::string>(ref);

      assert::is_true(std::is_same<decltype(str), std::string>::value);
      assert::equal(str, std::string { "explicit-string-cast" });
    },

    task("max-size") = [] {
      constexpr core::string_ref ref { };

      assert::equal(ref.max_size(), 0);
    },

    task("size") = [] {
      constexpr core::string_ref ref { "size", 4 };

      assert::equal(ref.size(), 4);
    },

    task("empty") = [] {
      constexpr core::string_ref empty_ref { };
      constexpr core::string_ref valid_ref { "valid", 5 };

      assert::is_false(valid_ref.empty());
      assert::is_true(empty_ref.empty());
    },

    task("subscript-operator") = [] {
      core::string_ref ref { "subscript" };

      assert::equal(ref[0], 's');
      assert::equal(ref[1], 'u');
      assert::equal(ref[2], 'b');
      assert::equal(ref[3], 's');
      assert::equal(ref[4], 'c');
      assert::equal(ref[5], 'r');
      assert::equal(ref[6], 'i');
      assert::equal(ref[7], 'p');
      assert::equal(ref[8], 't');
    },

    task("front") = [] {
      core::string_ref ref { "front" };
      assert::equal('f', ref.front());
    },

    task("back") = [] {
      core::string_ref ref { "back" };
      assert::equal('k', ref.back());
    },

    task("data") = [] {
      auto literal = "data-test";
      core::string_ref ref { literal };
      assert::is(ref.data(), literal);
    },

    task("remove-prefix") = [] {
      core::string_ref ref { "remove-prefix" };
      ref.remove_prefix(7);
      assert::equal(ref, core::string_ref { "prefix" });
      assert::equal(ref.size(), 6);
    },

    task("remove-suffix") = [] {
      core::string_ref ref { "remove-suffix" };
      ref.remove_suffix(7);
      assert::equal(ref, core::string_ref { "remove" });
      assert::equal(ref.size(), 6);
    },

    task("clear") = [] {
      core::string_ref ref { "hello", 5 };

      assert::is_false(ref.empty());
      assert::equal(ref.size(), 5);

      ref.clear();

      assert::is_true(ref.empty());
      assert::is_null(ref.data());
    },

    task("substr") = [] {
      using core::string_ref;
      std::string str { "0123456789abcdefghij" };
      string_ref ref { str };

      assert::equal(ref.substr(10), string_ref { "abcdefghij" });
      assert::equal(ref.substr(5, 3), string_ref { "567" });
      assert::equal(ref.substr(12, 100), string_ref { "cdefghij" });
      assert::equal(ref.substr(ref.size() - 3, 50), string_ref { "hij" });
    },

    task("starts-with") = [] {
      core::string_ref ref { "starts-with" };
      assert::is_true(ref.starts_with("starts"));
    },

    task("ends-with") = [] {
      core::string_ref ref { "ends-with" };
      assert::is_true(ref.ends_with("with"));
    },

    task("compare") = [] { assert::fail(); },

    task("at") = [] {
      core::string_ref ref { "string-ref-at" };

      assert::equal(ref.at(0), 's');
      assert::equal(ref.at(1), 't');
      assert::equal(ref.at(2), 'r');
      assert::equal(ref.at(3), 'i');
      assert::equal(ref.at(4), 'n');
      assert::equal(ref.at(5), 'g');
      assert::equal(ref.at(6), '-');
      assert::equal(ref.at(7), 'r');
      assert::equal(ref.at(8), 'e');
      assert::equal(ref.at(9), 'f');
      assert::equal(ref.at(10), '-');
      assert::equal(ref.at(11), 'a');
      assert::equal(ref.at(12), 't');

      assert::throws<std::out_of_range>([ref] { std::ignore = ref.at(13); });
    },

    task("find-first-not-of") = [] {
      core::string_ref ref { "find-first-not-of" };
      core::string_ref search_pass { "findrsto" };
      core::string_ref search_fail { "findrsto-" };
      auto npos = core::string_ref::npos;

      /* char comparisons */
      assert::equal(ref.find_first_not_of('x'), 0);
      assert::equal(ref.find_first_not_of('f'), 1);

      /* string-ref comparisons */
      assert::equal(ref.find_first_not_of(search_pass), 4);
      assert::equal(ref.find_first_not_of(search_fail), npos);
    },

    task("find-last-not-of") = [] {
      core::string_ref ref { "find-last-not-of" };
      core::string_ref search_pass { "findlasto" };
      core::string_ref search_fail { "findlasto-" };
      auto npos = core::string_ref::npos;

      /* char comparisons */
      assert::equal(ref.find_last_not_of('x'), ref.size() - 1);
      assert::equal(ref.find_last_not_of('f'), ref.size() - 2);

      /* string-ref comparisons */
      assert::equal(ref.find_last_not_of(search_pass), 13);
      assert::equal(ref.find_last_not_of(search_fail), npos);
    },

    task("find-first-of") = [] {
      core::string_ref ref { "find-first-of" };
      core::string_ref search_pass { "pass" };
      core::string_ref search_fail { "que?" };

      auto npos = core::string_ref::npos; /* used to solve linker issue */

      /* char comparisons */
      assert::equal(ref.find_first_of('x'), npos);
      assert::equal(ref.find_first_of('f'), 0);

      /* string-ref comparisons */
      assert::equal(ref.find_first_of(search_fail), npos);
      assert::equal(ref.find_first_of(search_pass), 8);
    },

    task("find-last-of") = [] {
      core::string_ref ref { "find-last-of" };
      core::string_ref pass { "pass" };
      core::string_ref fail { "exqu" };

      auto npos = core::string_ref::npos;

      assert::equal(ref.find_last_of('f'), ref.size() - 1);
      assert::equal(ref.find_last_of('x'), npos);

      assert::equal(ref.find_last_of(fail), npos);
      assert::equal(ref.find_last_of(pass), 7);
    },

    task("rfind") = [] {
      core::string_ref ref { "rfind" };
      auto npos = core::string_ref::npos;

      assert::equal(ref.find("in"), 2);
      assert::equal(ref.find('d'), ref.size() - 1);

      assert::equal(ref.find("string"), npos);
      assert::equal(ref.find('x'), npos);
    },

    task("find") = [] {
      core::string_ref ref { "find" };
      auto npos = core::string_ref::npos;

      assert::equal(ref.find("in"), 1);
      assert::equal(ref.find('d'), 3);

      assert::equal(ref.find("string"), npos);
      assert::equal(ref.find('x'), npos);
    },

    task("swap") = [] {
      core::string_ref lhs { "lhs" };
      core::string_ref rhs { "rhs" };

      assert::equal(lhs, core::string_ref { "lhs" });
      assert::equal(rhs, core::string_ref { "rhs" });

      std::swap(lhs, rhs);

      assert::equal(lhs, core::string_ref { "rhs" });
      assert::equal(rhs, core::string_ref { "lhs" });
    },

    task("operator-equal") = [] {
      core::string_ref lhs { "operator-equal" };
      core::string_ref rhs { "operator-equal" };

      assert::equal(lhs, rhs);
    },

    task("operator-not-equal") = [] {
      core::string_ref lhs { "operator-not-equal" };
      core::string_ref rhs { "rhs" };

      assert::not_equal(lhs, rhs);
    },

    task("operator-greater-equal") = [] {
      core::string_ref lhs { "fghij" };
      core::string_ref rhs { "abcde" };

      assert::greater_equal(lhs, rhs);
      assert::greater_equal(lhs, lhs);
    },

    task("operator-less-equal") = [] {
      core::string_ref lhs { "abcde" };
      core::string_ref rhs { "bcdef" };

      assert::less_equal(lhs, rhs);
      assert::less_equal(lhs, lhs);
    },

    task("operator-greater") = [] {
      core::string_ref lhs { "bcdef" };
      core::string_ref rhs { "abcde" };

      assert::greater(lhs, rhs);
    },

    task("operator-less") = [] {
      core::string_ref lhs { "abcde" };
      core::string_ref rhs { "fghij" };

      assert::less(lhs, rhs);
    },

    task("operator-stream-insert") = [] {
      std::string str { "operator-stream-insert" };
      core::string_ref ref { str };

      std::ostringstream stream;
      stream << ref;

      assert::equal(stream.str(), str);
    },

    task("hash") = [] {
      std::unordered_map<core::string_ref, int> values = {
        { "one", 1 },
        { "two", 2 },
        { "thr", 3 }
      };

      assert::equal(values["one"], 1);
      assert::equal(values["two"], 2);
      assert::equal(values["thr"], 3);
    }
  };

  monitor::run();
}
