#include <core/string.hpp>

#include <unordered_map>
#include <sstream>
#include <cstring>

#include <unittest/unittest.hpp>

int main () {
  using namespace unittest;

  test("basic-string-view") = {
    task("default-constructor") = [] {
      constexpr core::string_view ref { };
      assert::equal(ref.length(), 0u);
      assert::equal(ref.size(), 0u);
      assert::is_true(ref.empty());
      assert::is_null(ref.data());
    },

    task("copy-constructor") = [] {
      constexpr core::string_view ref { "hello!", 6 };

      assert::equal(ref.length(), 6u);
      assert::equal(ref.size(), 6u);
      assert::is_false(ref.empty());
      assert::is_not_null(ref.data());

      constexpr core::string_view copy { ref };

      assert::equal(copy.length(), ref.length());
      assert::equal(copy.size(), ref.size());
      assert::equal(copy.empty(), ref.empty());
      assert::equal(copy.data(), ref.data());

      assert::equal(copy, ref);
    },

    task("string-constructor") = [] {
      std::string str { "string-ctor" };
      core::string_view ref { str };

      assert::is_not_null(ref.data());
      assert::is_false(ref.empty());
      assert::equal(ref.size(), 11u);
      assert::is(ref.data(), str.data());
    },

    task("pointer-constructor") = [] {
      auto str = "pointer-constructor";
      core::string_view ref { str };

      assert::is_not_null(ref.data());
      assert::is_false(ref.empty());

      assert::equal(ref.size(), std::strlen(str));
      assert::is(ref.data(), str);
    },

    task("pointer-len-constructor") = [] {
      constexpr core::string_view ref { "pointer-len", 11 };

      assert::is_not_null(ref.data());
      assert::is_false(ref.empty());
      assert::equal(ref.size(), 11u);
    },

    task("copy-assignment-operator") = [] {
      core::string_view ref { "copy-assign" };
      core::string_view copy { };
      copy = ref;

      assert::is_false(copy.empty());
      assert::is_false(ref.empty());
      assert::equal(copy.size(), ref.size());
      assert::is(copy.data(), ref.data());

      assert::equal(copy, ref);
    },

    task("explicit-string-cast") = [] {
      core::string_view ref { "explicit-string-cast" };
      auto str = static_cast<std::string>(ref);

      assert::is_true(std::is_same<decltype(str), std::string>::value);
      assert::equal(str, std::string { "explicit-string-cast" });
    },

    task("to-string") = [] {
      core::string_view ref { "to-string" };
      auto str = ref.to_string();

      assert::equal(str, "to-string");
    },

    task("max-size") = [] {
      constexpr core::string_view ref { };

      assert::equal(ref.max_size(), 0u);
    },

    task("size") = [] {
      constexpr core::string_view ref { "size", 4 };

      assert::equal(ref.size(), 4u);
    },

    task("empty") = [] {
      constexpr core::string_view empty_ref { };
      constexpr core::string_view valid_ref { "valid", 5 };

      assert::is_false(valid_ref.empty());
      assert::is_true(empty_ref.empty());
    },

    task("subscript-operator") = [] {
      core::string_view ref { "subscript" };

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
      core::string_view ref { "front" };
      assert::equal('f', ref.front());
    },

    task("back") = [] {
      core::string_view ref { "back" };
      assert::equal('k', ref.back());
    },

    task("data") = [] {
      auto literal = "data-test";
      core::string_view ref { literal };
      assert::is(ref.data(), literal);
    },

    task("remove-prefix") = [] {
      core::string_view ref { "remove-prefix" };
      ref.remove_prefix(7);
      assert::equal(ref, core::string_view { "prefix" });
      assert::equal(ref.size(), 6u);
    },

    task("remove-suffix") = [] {
      core::string_view ref { "remove-suffix" };
      ref.remove_suffix(7);
      assert::equal(ref, core::string_view { "remove" });
      assert::equal(ref.size(), 6u);
    },

    task("clear") = [] {
      core::string_view ref { "hello", 5 };

      assert::is_false(ref.empty());
      assert::equal(ref.size(), 5u);

      ref.clear();

      assert::is_true(ref.empty());
      assert::is_null(ref.data());
    },

    task("substr") = [] {
      using core::string_view;
      std::string str { "0123456789abcdefghij" };
      string_view ref { str };

      assert::equal(ref.substr(10), string_view { "abcdefghij" });
      assert::equal(ref.substr(5, 3), string_view { "567" });
      assert::equal(ref.substr(12, 100), string_view { "cdefghij" });
      assert::equal(ref.substr(ref.size() - 3, 50), string_view { "hij" });
    },

    task("starts-with") = [] {
      core::string_view ref { "starts-with" };
      assert::is_true(ref.starts_with("starts"));
    },

    task("ends-with") = [] {
      core::string_view ref { "ends-with" };
      assert::is_true(ref.ends_with("with"));
    },

    task("compare") = [] {
      core::string_view ref { "compare" };

      assert::greater(ref.compare("comparable"), 0);
      assert::greater(ref.compare("bompare"), 0);
      assert::greater(ref.compare("comp"), 0);
      assert::equal(ref.compare("compare"), 0);
      assert::less(ref.compare("comparu"), 0);
      assert::less(ref.compare("dompare"), 0);
    },

    task("at") = [] {
      core::string_view ref { "string-view-at" };

      assert::equal(ref.at(0), 's');
      assert::equal(ref.at(1), 't');
      assert::equal(ref.at(2), 'r');
      assert::equal(ref.at(3), 'i');
      assert::equal(ref.at(4), 'n');
      assert::equal(ref.at(5), 'g');
      assert::equal(ref.at(6), '-');
      assert::equal(ref.at(7), 'v');
      assert::equal(ref.at(8), 'i');
      assert::equal(ref.at(9), 'e');
      assert::equal(ref.at(10), 'w');
      assert::equal(ref.at(11), '-');
      assert::equal(ref.at(12), 'a');
      assert::equal(ref.at(13), 't');

      assert::throws<std::out_of_range>([ref] { std::ignore = ref.at(14); });
    },

    task("find-first-not-of") = [] {
      core::string_view ref { "find-first-not-of" };
      core::string_view search_pass { "findrsto" };
      core::string_view search_fail { "findrsto-" };
      auto npos = core::string_view::npos;

      /* char comparisons */
      assert::equal(ref.find_first_not_of('x'), 0u);
      assert::equal(ref.find_first_not_of('f'), 1u);

      /* string-view comparisons */
      assert::equal(ref.find_first_not_of(search_pass), 4u);
      assert::equal(ref.find_first_not_of(search_fail), npos);
    },

    task("find-last-not-of") = [] {
      core::string_view ref { "find-last-not-of" };
      core::string_view search_pass { "findlasto" };
      core::string_view search_fail { "findlasto-" };
      auto npos = core::string_view::npos;

      /* char comparisons */
      assert::equal(ref.find_last_not_of('x'), ref.size() - 1);
      assert::equal(ref.find_last_not_of('f'), ref.size() - 2);

      /* string-view comparisons */
      assert::equal(ref.find_last_not_of(search_pass), 13u);
      assert::equal(ref.find_last_not_of(search_fail), npos);
    },

    task("find-first-of") = [] {
      core::string_view ref { "find-first-of" };
      core::string_view search_pass { "pass" };
      core::string_view search_fail { "que?" };

      auto npos = core::string_view::npos; /* used to solve linker issue */

      /* char comparisons */
      assert::equal(ref.find_first_of('x'), npos);
      assert::equal(ref.find_first_of('f'), 0u);

      /* string-view comparisons */
      assert::equal(ref.find_first_of(search_fail), npos);
      assert::equal(ref.find_first_of(search_pass), 8u);
    },

    task("find-last-of") = [] {
      core::string_view ref { "find-last-of" };
      core::string_view pass { "pass" };
      core::string_view fail { "exqu" };

      auto npos = core::string_view::npos;

      assert::equal(ref.find_last_of('f'), ref.size() - 1);
      assert::equal(ref.find_last_of('x'), npos);

      assert::equal(ref.find_last_of(fail), npos);
      assert::equal(ref.find_last_of(pass), 7u);
    },

    task("rfind") = [] {
      core::string_view ref { "rfind" };
      auto npos = core::string_view::npos;

      assert::equal(ref.find("in"), 2u);
      assert::equal(ref.find('d'), ref.size() - 1);

      assert::equal(ref.find("string"), npos);
      assert::equal(ref.find('x'), npos);
    },

    task("find") = [] {
      core::string_view ref { "find" };
      auto npos = core::string_view::npos;

      assert::equal(ref.find("in"), 1u);
      assert::equal(ref.find('d'), 3u);

      assert::equal(ref.find("string"), npos);
      assert::equal(ref.find('x'), npos);
    },

    task("swap") = [] {
      using std::swap;
      core::string_view lhs { "lhs" };
      core::string_view rhs { "rhs" };

      assert::equal(lhs, core::string_view { "lhs" });
      assert::equal(rhs, core::string_view { "rhs" });

      swap(lhs, rhs);

      assert::equal(lhs, core::string_view { "rhs" });
      assert::equal(rhs, core::string_view { "lhs" });
    },

    task("operator-equal") = [] {
      core::string_view lhs { "operator-equal" };
      core::string_view rhs { "operator-equal" };

      assert::equal(lhs, rhs);
    },

    task("operator-not-equal") = [] {
      core::string_view lhs { "operator-not-equal" };
      core::string_view rhs { "rhs" };

      assert::not_equal(lhs, rhs);
    },

    task("operator-greater-equal") = [] {
      core::string_view lhs { "fghij" };
      core::string_view rhs { "abcde" };

      assert::greater_equal(lhs, rhs);
      assert::greater_equal(lhs, lhs);
    },

    task("operator-less-equal") = [] {
      core::string_view lhs { "abcde" };
      core::string_view rhs { "bcdef" };

      assert::less_equal(lhs, rhs);
      assert::less_equal(lhs, lhs);
    },

    task("operator-greater") = [] {
      core::string_view lhs { "bcdef" };
      core::string_view rhs { "abcde" };

      assert::greater(lhs, rhs);
    },

    task("operator-less") = [] {
      core::string_view lhs { "abcde" };
      core::string_view rhs { "fghij" };

      assert::less(lhs, rhs);
    },

    task("operator-stream-insert") = [] {
      std::string str { "operator-stream-insert" };
      core::string_view ref { str };

      std::ostringstream stream;
      stream << ref;

      assert::equal(stream.str(), str);
    },

    task("hash") = [] {
      std::unordered_map<core::string_view, int> values = {
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
