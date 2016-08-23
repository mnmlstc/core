#include <core/string_view.hpp>

#include <unordered_map>
#include <sstream>
#include <cstring>

#include "catch.hpp"

TEST_CASE("string-view-constructors", "[string-view][constructors]") {
  SECTION("default") {
    constexpr core::string_view ref { };
    CHECK(ref.length() == 0u);
    CHECK(ref.size() == 0u);
    CHECK(ref.empty());
    CHECK_FALSE(ref.data());
  }

  SECTION("copy") {
    constexpr core::string_view ref { "hello!", 6 };

    CHECK(ref.length() == 6u);
    CHECK(ref.size() == 6u);
    CHECK_FALSE(ref.empty());
    CHECK(ref.data());

    constexpr core::string_view copy { ref };

    CHECK(copy.length() == ref.length());
    CHECK(copy.size() == ref.size());
    CHECK(copy.empty() == ref.empty());
    CHECK(copy.data() == ref.data());

    CHECK(copy == ref);
  }

  SECTION("string") {
    std::string str { "string-ctor" };
    core::string_view ref { str };

    CHECK(ref.data());
    CHECK_FALSE(ref.empty());
    CHECK(ref.size() == 11u);
    CHECK(ref.data() == str.data());
  }

  SECTION("pointer") {
    auto str = "pointer-constructor";
    core::string_view ref { str };

    CHECK(ref.data());
    CHECK_FALSE(ref.empty());

    CHECK(ref.size() == std::strlen(str));
    CHECK(ref.data() == str);
  }

  SECTION("pointer-with-length") {
    constexpr core::string_view ref { "pointer-len", 11 };

    CHECK(ref.data());
    CHECK_FALSE(ref.empty());
    CHECK(ref.size() == 11u);
  }
}

TEST_CASE("string-view-assignment", "[string-view][assignment]") {
  SECTION("copy") {
    core::string_view ref { "copy-assign" };
    core::string_view copy { };
    copy = ref;

    CHECK_FALSE(copy.empty());
    CHECK_FALSE(ref.empty());
    CHECK(copy.size() == ref.size());
    CHECK(copy.data() == ref.data());

    CHECK(copy == ref);
  }
}

TEST_CASE("string-view-methods", "[string-view][methods]") {
  SECTION("explicit-string-cast") {
    core::string_view ref { "explicit-string-cast" };
    auto str = static_cast<std::string>(ref);

    CHECK((std::is_same<decltype(str), std::string>::value));
    CHECK(str == "explicit-string-cast");
  }

  SECTION("to-string") {
    core::string_view ref { "to-string" };
    auto str = ref.to_string();

    CHECK(str == "to-string");
  }

  SECTION("max-size") {
    constexpr core::string_view ref { };

    CHECK(ref.max_size() == ::std::numeric_limits<size_t>::max());
  }

  SECTION("size") {
    constexpr core::string_view ref { "size", 4 };

    CHECK(ref.size() == 4u);
  }

  SECTION("empty") {
    constexpr core::string_view empty_ref { };
    constexpr core::string_view valid_ref { "valid", 5 };

    CHECK_FALSE(valid_ref.empty());
    CHECK(empty_ref.empty());
  }

  SECTION("front") {
    core::string_view ref { "front" };
    CHECK('f' == ref.front());
  }

  SECTION("back") {
    core::string_view ref { "back" };
    CHECK('k' == ref.back());
  }

  SECTION("data") {
    auto literal = "data-TEST_CASE";
    core::string_view ref { literal };
    CHECK(ref.data() == literal);
  }

  SECTION("remove-prefix") {
    core::string_view ref { "remove-prefix" };
    ref.remove_prefix(7);
    CHECK(ref == core::string_view { "prefix" });
    CHECK(ref.size() == 6u);
  }

  SECTION("remove-suffix") {
    core::string_view ref { "remove-suffix" };
    ref.remove_suffix(7);
    CHECK(ref == core::string_view { "remove" });
    CHECK(ref.size() == 6u);
  }

  SECTION("clear") {
    core::string_view ref { "hello", 5 };

    CHECK_FALSE(ref.empty());
    CHECK(ref.size() == 5u);

    ref.clear();

    CHECK(ref.empty());
    CHECK_FALSE(ref.data());
  }

  SECTION("substr") {
    using core::string_view;
    std::string str { "0123456789abcdefghij" };
    string_view ref { str };

    CHECK(ref.substr(10) == string_view { "abcdefghij" });
    CHECK(ref.substr(5, 3) == string_view { "567" });
    CHECK(ref.substr(12, 100) == string_view { "cdefghij" });
    CHECK(ref.substr(ref.size() - 3, 50) == string_view { "hij" });
  }

  SECTION("starts-with") {
    core::string_view ref { "starts-with" };
    CHECK(ref.starts_with("starts"));
  }

  SECTION("ends-with") {
    core::string_view ref { "ends-with" };
    CHECK(ref.ends_with("with"));
  }

  SECTION("compare") {
    core::string_view ref { "compare" };

    CHECK(ref.compare("comparable") > 0);
    CHECK(ref.compare("bompare") > 0);
    CHECK(ref.compare("comp") > 0);
    CHECK(ref.compare("compare") == 0);
    CHECK(ref.compare("comparu") < 0);
    CHECK(ref.compare("dompare") < 0);
  }

  SECTION("at") {
    core::string_view ref { "string-view-at" };

    CHECK(ref.at(0) == 's');
    CHECK(ref.at(1) == 't');
    CHECK(ref.at(2) == 'r');
    CHECK(ref.at(3) == 'i');
    CHECK(ref.at(4) == 'n');
    CHECK(ref.at(5) == 'g');
    CHECK(ref.at(6) == '-');
    CHECK(ref.at(7) == 'v');
    CHECK(ref.at(8) == 'i');
    CHECK(ref.at(9) == 'e');
    CHECK(ref.at(10) == 'w');
    CHECK(ref.at(11) == '-');
    CHECK(ref.at(12) == 'a');
    CHECK(ref.at(13) == 't');

    CHECK_THROWS_AS(ref.at(14), std::out_of_range);
  }

  SECTION("find-first-not-of") {
    core::string_view ref { "find-first-not-of" };
    core::string_view search_pass { "findrsto" };
    core::string_view search_fail { "findrsto-" };
    auto npos = core::string_view::npos;

    // char comparisons
    CHECK(ref.find_first_not_of('x') == 0u);
    CHECK(ref.find_first_not_of('f') == 1u);

    // string-view comparisons
    CHECK(ref.find_first_not_of(search_pass) == 4u);
    CHECK(ref.find_first_not_of(search_fail) == npos);
  }

  SECTION("find-last-not-of") {
    core::string_view ref { "find-last-not-of" };
    core::string_view search_pass { "findlasto" };
    core::string_view search_fail { "findlasto-" };
    auto npos = core::string_view::npos;

    // char comparisons
    CHECK(ref.find_last_not_of('x') == ref.size() - 1);
    CHECK(ref.find_last_not_of('f') == ref.size() - 2);

    // string-view comparisons
    CHECK(ref.find_last_not_of(search_pass) == 13u);
    CHECK(ref.find_last_not_of(search_fail) == npos);
  }

  SECTION("find-first-of") {
    core::string_view ref { "find-first-of" };
    core::string_view search_pass { "pass" };
    core::string_view search_fail { "que?" };

    auto npos = core::string_view::npos; // used to solve linker issue

    // char comparisons
    CHECK(ref.find_first_of('x') == npos);
    CHECK(ref.find_first_of('f') == 0u);

    // string-view comparisons
    CHECK(ref.find_first_of(search_fail) == npos);
    CHECK(ref.find_first_of(search_pass) == 8u);
  }

  SECTION("find-last-of") {
    core::string_view ref { "find-last-of" };
    core::string_view pass { "pass" };
    core::string_view fail { "exqu" };

    auto npos = core::string_view::npos;

    CHECK(ref.find_last_of('f') == ref.size() - 1);
    CHECK(ref.find_last_of('x') == npos);

    CHECK(ref.find_last_of(fail) == npos);
    CHECK(ref.find_last_of(pass) == 7u);
  }

  SECTION("rfind") {
    core::string_view ref { "rfind" };
    auto npos = core::string_view::npos;

    CHECK(ref.find("in") == 2u);
    CHECK(ref.find('d') == ref.size() - 1);

    CHECK(ref.find("string") == npos);
    CHECK(ref.find('x') == npos);
  }

  SECTION("find") {
    core::string_view ref { "find" };
    auto npos = core::string_view::npos;

    CHECK(ref.find("in") == 1u);
    CHECK(ref.find('d') == 3u);

    CHECK(ref.find("string") == npos);
    CHECK(ref.find('x') == npos);
  }
}

TEST_CASE("string-view-operators", "[string-view][operators]") {
  SECTION("equal") {
    core::string_view lhs { "operator-equal" };
    core::string_view rhs { "operator-equal" };

    CHECK(lhs == rhs);
  }

  SECTION("not-equal") {
    core::string_view lhs { "operator-not-equal" };
    core::string_view rhs { "rhs" };

    CHECK(lhs != rhs);
  }

  SECTION("greater-equal") {
    core::string_view lhs { "fghij" };
    core::string_view rhs { "abcde" };

    CHECK(lhs >= rhs);
    CHECK(lhs >= lhs);
  }

  SECTION("less-equal") {
    core::string_view lhs { "abcde" };
    core::string_view rhs { "bcdef" };

    CHECK(lhs <= rhs);
    CHECK(lhs <= lhs);
  }

  SECTION("greater") {
    core::string_view lhs { "bcdef" };
    core::string_view rhs { "abcde" };

    CHECK(lhs > rhs);
  }

  SECTION("less") {
    core::string_view lhs { "abcde" };
    core::string_view rhs { "fghij" };

    CHECK(lhs < rhs);
  }

  SECTION("stream-insert") {
    std::string str { "operator-stream-insert" };
    core::string_view ref { str };

    std::ostringstream stream;
    stream << ref;

    CHECK(stream.str() == str);
  }

  SECTION("subscript") {
    core::string_view ref { "subscript" };

    CHECK(ref[0] == 's');
    CHECK(ref[1] == 'u');
    CHECK(ref[2] == 'b');
    CHECK(ref[3] == 's');
    CHECK(ref[4] == 'c');
    CHECK(ref[5] == 'r');
    CHECK(ref[6] == 'i');
    CHECK(ref[7] == 'p');
    CHECK(ref[8] == 't');
  }
}

TEST_CASE("string-view-functions", "[string-view][functions]") {
  SECTION("hash") {
    std::unordered_map<core::string_view, int> values = {
      { "one", 1 },
      { "two", 2 },
      { "three", 3 },
      { "eleven", 11 },
      { "fourteen", 14 }
    };

    CHECK(values["one"] == 1);
    CHECK(values["two"] == 2);
    CHECK(values["three"] == 3);
    CHECK(values["eleven"] == 11);
    CHECK(values["fourteen"] == 14);
  }

  SECTION("swap") {
    using std::swap;
    core::string_view lhs { "lhs" };
    core::string_view rhs { "rhs" };

    CHECK(lhs == core::string_view { "lhs" });
    CHECK(rhs == core::string_view { "rhs" });

    swap(lhs, rhs);

    CHECK(lhs == core::string_view { "rhs" });
    CHECK(rhs == core::string_view { "lhs" });
  }
}

TEST_CASE("string-view-issues", "[string-view][issues]") {
  SECTION("issue-20") {
    core::string_view const ref { "b" };
    std::string const str { "b" };

    // string comparisons
    CHECK(ref == str);
    CHECK(ref != std::string { "a" });
    CHECK(ref >= std::string { "a" });
    CHECK(ref >= str);
    CHECK(ref <= str);
    CHECK(ref <= std::string { "c" });
    CHECK(ref > std::string { "a" });
    CHECK(ref < std::string { "c" });

    CHECK(str == ref);
    CHECK(std::string { "a" } != ref);
    CHECK(std::string { "c" } >= ref);
    CHECK(str >= ref);
    CHECK(str <= ref);
    CHECK(std::string { "a" } <= ref);
    CHECK(std::string { "c" } > ref);
    CHECK(std::string { "a" } < ref);

    // char comparisons
    CHECK(ref == "b");
    CHECK(ref != "a");
    CHECK(ref >= "a");
    CHECK(ref >= "b");
    CHECK(ref <= "b");
    CHECK(ref <= "c");
    CHECK(ref > "a");
    CHECK(ref < "c");

    CHECK(str == ref);
    CHECK("a" != ref);
    CHECK("c" >= ref);
    CHECK(str >= ref);
    CHECK(str <= ref);
    CHECK("a" <= ref);
    CHECK("c" > ref);
    CHECK("a" < ref);
  }
}
