#include <core/algorithm.hpp>
#include <initializer_list>
#include <random>
#include <vector>
#include <set>

#include "catch.hpp"

TEST_CASE("predicates", "[non-modifying-sequence]") {
  std::vector<int> values { 1, 2, 3, 4, 5 };

  static_assert(core::is_range<std::vector<int>>::value, "UH OH");

  SECTION("all-of") {
    auto result = core::all_of(values, [](int v) { return v > 0; });
    CHECK(result);
  }

  SECTION("any-of") {
    auto result = core::any_of(values, [](int v) { return v % 2 == 0; });
    CHECK(result);
  }

  SECTION("none-of") {
    auto result = core::none_of(values, [](int v) { return v <= 0; });
    CHECK(result);
  }
}

TEST_CASE("for-each", "[non-modifying-sequence]") {
  SECTION("for-each") {
    std::vector<int> values { 1, 2, 3, 4, 5 };
    core::for_each(values, [](int v) { CHECK(v > 0); });
  }

  SECTION("for-each-while") {
    std::vector<int> values { 1, 2, 3, 4, 5, 6 };
    auto const function = [] (int v) { CHECK(v < 6); };
    auto const predicate = [] (int v) { return v != 6; };
    auto it = core::for_each_while(values, function, predicate);
    CHECK(it != end(values));
  }

  SECTION("for-each-until") {
    std::vector<int> values { 1, 2, 3, 4, 5, 6 };
    auto const function = [] (int v) { CHECK(v < 6); };
    auto it = core::for_each_until(values, function, 6);
    CHECK(it != end(values));
  }

}


TEST_CASE("count", "[non-modifying-sequence]") {
  SECTION("count") {
    std::vector<int> values { 1, 2, 1, 1 };
    auto result = core::count(values, 1);
    CHECK(result == 3);
  }

  SECTION("count-if") {
    std::vector<int> values { 1, 2, 1 };
    auto result = core::count_if(values, [](int v) { return v % 2; });
    CHECK(result == 2);
  }
}

TEST_CASE("mismatch", "[non-modifying-sequence]") {
  auto value = std::string { "abcXYZcba" };
  auto expected = std::string { "abc" };
  auto result = core::mismatch(value, value.rbegin());
  auto compare = std::string { value.begin(), result.first };
  CHECK(compare == expected);
}

TEST_CASE("equal", "[non-modifying-sequence]") {
  auto value = std::string { "hello" };
  auto result = core::equal(value, begin(value));
  CHECK(result);
}

TEST_CASE("find", "[non-modifying-sequence]") {
  SECTION("find") {
    auto value = std::string { "find" };
    auto result = core::find(value, 'n');
    CHECK(result != end(value));
  }

  SECTION("find-if") {
    auto value = std::string { "find-if" };
    auto result = core::find_if(value, [](int ch) {
      return not ::std::isalpha(ch);
    });
    CHECK(result != end(value));
  }

  SECTION("find-if-not") {
    auto value = std::string { "find-if-not" };
    auto result = core::find_if_not(value,
      [] (int ch) { return ::std::isalpha(ch);
    });
    CHECK(result != end(value));
  }

  SECTION("find-end") {
    auto value = std::vector<int> { 1, 2, 3, 4, 6, 7, 9, 8, 1, 2, 3 };
    auto subset = std::vector<int> { 1, 2, 3 };

    auto result = core::find_end(value, subset);
    auto distance = ::std::distance(begin(value), result);
    CHECK(result != end(value));
    CHECK(distance == 8);
  }

  SECTION("find-first-of") {
    auto value = std::string { "find-first-of" };
    auto subset = std::string { "o" };
    auto result = core::find_first_of(value, subset);
    auto distance = ::std::distance(begin(value), result);
    CHECK(result != end(value));
    CHECK(distance == 11);
  }

  SECTION("adjacent-find") {
    auto value = std::string { "adjacent-find" };
    auto result = core::adjacent_find(value);
    CHECK(result == end(value));
  }
}

TEST_CASE("search", "[non-modifying-sequence]") {
  SECTION("search") {
    auto value = std::string { "searching with the search function" };
    auto result = core::search(value, std::string { "the" });
    CHECK(result != end(value));
  }

  SECTION("search-n") {
    auto value = std::string { "searching for consecutive letters" };
    auto result = core::search_n(value, 2, 't');
    CHECK(result != end(value));
  }
}

TEST_CASE("copy", "[modifying-sequence]") {
  SECTION("copy") {
    std::vector<int> from { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::vector<int> to { };
    std::ignore = core::copy(from, std::back_inserter(to));

    CHECK(to[0] == 0);
    CHECK(to[1] == 1);
    CHECK(to[2] == 2);
    CHECK(to[3] == 3);
    CHECK(to[4] == 4);
    CHECK(to[5] == 5);
    CHECK(to[6] == 6);
    CHECK(to[7] == 7);
    CHECK(to[8] == 8);
    CHECK(to[9] == 9);
  }

  SECTION("copy-if") {
    auto value = std::string { "copy-if" };
    std::string output { };
    std::ignore = core::copy_if(
      value,
      ::std::back_inserter(output),
      [] (char ch) { return ::std::isalpha(ch); }
    );
    CHECK(output == "copyif");
  }

  SECTION("copy-backward") {
    auto value = std::string { "copy-backward" };
    std::string output(value.size(), '\0');
    std::ignore = core::copy_backward(value, ::std::end(output));
    CHECK(output == value);
  }
}

TEST_CASE("move", "[modifying-sequence]") {
  SECTION("move") {
    auto value = std::vector<std::string> { "first", "second", "third" };
    std::vector<std::string> output { 3 };
    std::ignore = core::move(value, ::std::begin(output));

    CHECK(value[0].empty());
    CHECK(value[1].empty());
    CHECK(value[2].empty());

    CHECK(output[0] == "first");
    CHECK(output[1] == "second");
    CHECK(output[2] == "third");
  }

  SECTION("move-backward") {
    auto value = std::vector<std::string> { "first", "second", "third" };
    std::vector<std::string> output { 3 };
    std::ignore = core::move_backward(value, ::std::end(output));

    CHECK(value[0].empty());
    CHECK(value[1].empty());
    CHECK(value[2].empty());

    CHECK(output[2] == "third");
    CHECK(output[1] == "second");
    CHECK(output[0] == "first");
  }
}

TEST_CASE("fill", "[modifying-sequence]") {
  std::vector<int> value { 1, 2, 3, 4, 5, 6, 7 };
  auto predicate = [](int v) { return v == -1; };
  CHECK(core::none_of(value, predicate));
  core::fill(value, -1);
  CHECK(core::all_of(value, predicate));
}

TEST_CASE("transform", "[modifying-sequence]") {
  SECTION("transform-unary") {
    std::vector<int> value { 1, 2, 3 };
    std::vector<std::string> output { 3 };
    core::transform(value, ::std::begin(output), [] (int v) -> std::string {
      return std::to_string(v);
    });
    CHECK(output[0] == "1");
    CHECK(output[1] == "2");
    CHECK(output[2] == "3");
  }

  SECTION("transform-binary-ranges") {
    std::vector<int> keys { 1, 2, 3 };
    std::vector<std::string> values { "1", "2", "3" };
    std::vector<std::pair<int, std::string>> output;
    auto const converter = [] (int l, std::string& s) {
      return std::make_pair(l, s);
    };
    core::transform(keys, values, std::back_inserter(output), converter);
    CHECK(output[0].second == "1");
    CHECK(output[1].second == "2");
    CHECK(output[2].second == "3");
    CHECK(output[0].first == 1);
    CHECK(output[1].first == 2);
    CHECK(output[2].first == 3);
  }

  SECTION("transform-binary-iterator") {
    std::vector<int> key { 1, 2, 3 };
    std::vector<std::string> value { "1", "2", "3" };
    std::vector<std::pair<int, std::string>> output;
    auto const converter = [] (int l, std::string& s) {
      return std::make_pair(l, s);
    };
    core::transform(key, value.data(), std::back_inserter(output), converter);
    CHECK(output[0].second == "1");
    CHECK(output[1].second == "2");
    CHECK(output[2].second == "3");
    CHECK(output[0].first == 1);
    CHECK(output[1].first == 2);
    CHECK(output[2].first == 3);
  }

  SECTION("transform-if") {
    std::vector<int> value { 1, 2, 3 };
    std::vector<std::string> output { };
    std::vector<std::string> output2 { };

    core::transform_if(
      value,
      ::std::back_inserter(output),
      [] (int v) { return std::to_string(v); },
      [] (int v) { return v % 2 != 0; }
    );

    CHECK(output.size() == 2u);
    CHECK(output[0] == "1");
    CHECK(output[1] == "3");

    value = { 2, 3 };

    core::transform_if(
      value,
      output,
      ::std::back_inserter(output2),
      [] (int v, std::string const& str) { return std::to_string(v) + str; },
      [] (int v, std::string const&) { return v % 2 != 0 and v > 1; }
    );

    CHECK(output2.size() == 1u);
    CHECK(output2[0] == "33");
  }
}

TEST_CASE("remove", "[modifying-sequence]") {
  SECTION("remove") {
    std::string text { "words words words" };
    text.erase(core::remove(text, ' '), ::std::end(text));
    CHECK(text == "wordswordswords");
  }

  SECTION("remove-if") {
    std::string text { "remove-if" };
    text.erase(
      core::remove_if(text, [](char ch) { return not ::std::isalpha(ch); }),
      ::std::end(text));
    CHECK(text == "removeif");
  }

  SECTION("remove-copy") {
    std::string text { "remove-copy" };
    std::string output { };
    std::ignore = core::remove_copy(text, ::std::back_inserter(output), '-');
    CHECK(output == "removecopy");
  }

  SECTION("remove-copy-if") {
    std::string text { "remove-copy-if" };
    std::string output { };
    std::ignore = core::remove_copy_if(
      text,
      ::std::back_inserter(output),
      [] (char ch) { return ::std::isalpha(ch); }
    );
    CHECK(output == "--");
  }

  SECTION("remove-erase") {
    std::vector<int> values { 1, 2, 3, 4, 5 };
    core::remove_erase(values, 3);
    CHECK(values.size() == 4u);
    CHECK(values[0] == 1);
    CHECK(values[1] == 2);
    CHECK(values[2] == 4);
    CHECK(values[3] == 5);
  }

  SECTION("remove-erase-if") {
    std::vector<int> values { 1, 2, 3, 4, 5 };
    core::remove_erase_if(values, [] (int v) { return v % 2; });
    CHECK(values.size() == 2u);
    CHECK(values[0] == 2);
    CHECK(values[1] == 4);
  }
}

TEST_CASE("replace", "[modifying-sequence]") {
  SECTION("replace") {
    std::string text { "replace" };
    core::replace(text, 'e', 'f');
    CHECK(text == "rfplacf");
  }

  SECTION("replace-if") {
    std::string text { "replace-if" };
    core::replace_if(text, [] (char c) { return not std::isalpha(c); }, 'f');
    CHECK(text == "replacefif");
  }

  SECTION("replace-copy") {
    std::string text { "replace-copy" };
    std::string output { };
    core::replace_copy(text, ::std::back_inserter(output), '-', 'f');
    CHECK(output == "replacefcopy");
  }

  SECTION("replace-copy-if") {
    std::string text { "replace-copy-if" };
    std::string output { };
    core::replace_copy_if(
      text,
      ::std::back_inserter(output),
      [] (char ch) { return not std::isalpha(ch); },
      'f'
    );
    CHECK(output == "replacefcopyfif");
  }
}

TEST_CASE("swap-ranges", "[modifying-sequence]") {
  std::string hello { "hello" };
  std::vector<char> world { 'w', 'o', 'r', 'l', 'd' };
  core::swap_ranges(hello, ::std::begin(world));
  CHECK(hello == "world");
}

TEST_CASE("reverse", "[modifying-sequence]") {
  SECTION("reverse") {
    std::string text { "reverse" };
    core::reverse(text);
    CHECK(text == "esrever");
  }

  SECTION("reverse-copy") {
    std::string text { "reverse-copy" };
    std::string output { };
    core::reverse_copy(text, ::std::back_inserter(output));
    CHECK(output == "ypoc-esrever");
  }
}

TEST_CASE("rotate", "[modifying-sequence]") {
  SECTION("rotate") {
    std::vector<int> value { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    core::rotate(value, ::std::begin(value) + 5);
    CHECK(value[0] == 6);
    CHECK(value[1] == 7);
    CHECK(value[2] == 8);
    CHECK(value[3] == 9);
    CHECK(value[4] == 10);
    CHECK(value[5] == 1);
    CHECK(value[6] == 2);
    CHECK(value[7] == 3);
    CHECK(value[8] == 4);
    CHECK(value[9] == 5);
  }

  SECTION("rotate-copy") {
    std::vector<int> value { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::vector<int> output { };
    auto rotator = ::std::begin(value) + 5;
    core::rotate_copy(value, rotator, ::std::back_inserter(output));
    CHECK(output[0] == 5);
    CHECK(output[1] == 6);
    CHECK(output[2] == 7);
    CHECK(output[3] == 8);
    CHECK(output[4] == 9);
    CHECK(output[5] == 0);
    CHECK(output[6] == 1);
    CHECK(output[7] == 2);
    CHECK(output[8] == 3);
    CHECK(output[9] == 4);

  }
}

TEST_CASE("shuffle", "[modifying-sequence]") {
  std::vector<int> values { 1, 2, 3, 4, 5 };
  std::random_device rd { };
  core::shuffle(values, std::mt19937 { rd() });
}

TEST_CASE("unique", "[modifying-sequence]") {
  SECTION("unique") {
    std::vector<int> values { 1, 1, 2, 3, 4, 5 };
    auto result = core::unique(values);
    CHECK(result != end(values));
  }

  SECTION("unique-copy") {
    std::vector<int> values { 1, 1, 2, 3, 4, 5 };
    std::vector<int> output { };
    std::ignore = core::unique_copy(values, ::std::back_inserter(output));
    CHECK(output[0] == 1);
    CHECK(output[1] == 2);
    CHECK(output[2] == 3);
    CHECK(output[3] == 4);
    CHECK(output[4] == 5);
  }
}

TEST_CASE("is-partitioned", "[partitioning]") {
  auto is_even = [](int v) { return v % 2 == 0; };
  std::vector<int> values { 2, 4, 1, 3, 5 };
  CHECK(core::is_partitioned(values, is_even));
}

TEST_CASE("partition", "[partitioning]") {
  auto is_odd = [](int v) { return v % 2; };
  std::vector<int> values { 1, 2, 3, 4, 5, 6 };
  auto second_group = core::partition(values, is_odd);
  auto distance = std::distance(second_group, end(values));
  CHECK(distance == 3);
}

TEST_CASE("partition-copy", "[partitioning]") {
  auto is_odd = [](int v) { return v % 2; };
  std::vector<int> values { 1, 2, 3, 4, 5, 6 };
  std::vector<int> even { };
  std::vector<int> odds { };
  std::ignore = core::partition_copy(
    values,
    ::std::back_inserter(odds),
    ::std::back_inserter(even),
    is_odd
  );
  CHECK(core::none_of(even, is_odd));
  CHECK(core::all_of(odds, is_odd));
}

TEST_CASE("stable-partition", "[partitioning]") {
  auto is_odd = [](int v) { return v % 2; };
  std::vector<int> values { 1, 2, 3, 4, 5, 6 };
  std::ignore = core::stable_partition(values, is_odd);
  CHECK(values[0] == 1);
  CHECK(values[1] == 3);
  CHECK(values[2] == 5);
  CHECK(values[3] == 2);
  CHECK(values[4] == 4);
  CHECK(values[5] == 6);
}

TEST_CASE("partition-point", "[partitioning]") {
  auto is_odd = [](int v) { return v % 2; };
  std::vector<int> values { 1, 2, 3, 4, 5, 6 };
  auto result = core::stable_partition(values, is_odd);
  auto point = core::partition_point(values, is_odd);
  CHECK(point == result);
}

TEST_CASE("is-sorted", "[sorting]") {
  std::vector<int> values { 1, 2, 3, 4, 5 };
  CHECK(core::is_sorted(values));
}

TEST_CASE("is-sorted-until", "[sorting]") {
  std::vector<int> values { 1, 2, 3, 4, 5, 4, 3 };
  CHECK(end(values) != core::is_sorted_until(values));
}

TEST_CASE("sort", "[sorting]") {
  std::vector<int> values { 5, 4, 3, 2, 1 };
  core::sort(values);
  CHECK(values[0] == 1);
  CHECK(values[1] == 2);
  CHECK(values[2] == 3);
  CHECK(values[3] == 4);
  CHECK(values[4] == 5);
}

TEST_CASE("partial-sort", "[sorting]") {
  std::vector<int> values { 1, 4, 5, 6, 3, 2 };
  core::partial_sort(values, std::begin(values) + 3);
  CHECK(values[0] == 1);
  CHECK(values[1] == 2);
  CHECK(values[2] == 3);
}

TEST_CASE("partial-sort-copy", "[sorting]") {
  std::vector<int> values { 1, 4, 5, 6, 2, 3 };
  std::vector<int> output(3);
  core::partial_sort_copy(values, output);
  CHECK(output[0] == 1);
  CHECK(output[1] == 2);
  CHECK(output[2] == 3);
}

TEST_CASE("stable-sort", "[sorting]") {
  struct employee {
    bool operator == (employee const& that) const noexcept {
      return this->age == that.age and this->name == that.name;
    }

    bool operator < (employee const& that) const noexcept {
      return this->age < that.age;
    }

    std::string name;
    std::int64_t age;
  };

  std::vector<employee> values = {
    { "jane smith", 32 },
    { "joe shmoe", 55 },
    { "jocoocoo puhwenis", 39 },
    { "ira glass", 55 },
    { "bubbles", 43 }
  };
  core::stable_sort(values);
  auto ira = employee { "ira glass", 55 };
  CHECK(values.back() == ira);
}

TEST_CASE("nth-element", "[sorting]") {
  std::vector<int> values { 5, 6, 4, 3, 2, 6, 7, 9, 3 };
  core::nth_element(values, ::std::begin(values) + values.size() / 2);
  CHECK(values[values.size() / 2] == 5);
}

TEST_CASE("lower-bound", "[binary-search]") {
  std::vector<int> values { 1, 2, 3, 4, 5, 6, 7 };
  auto result = core::lower_bound(values, 4);
  CHECK(result != end(values));
  CHECK(*result == 4);
}

TEST_CASE("upper-bound", "[binary-search]") {
  std::vector<int> values { 1, 2, 3, 4, 5, 6, 7 };
  auto result = core::upper_bound(values, 4);
  CHECK(result != end(values));
  CHECK(*result == 5);
}

TEST_CASE("binary-search", "[binary-search]") {
  std::vector<int> values { 1, 2, 3, 4, 5, 6, 7 };
  CHECK(core::binary_search(values, 3));
}

TEST_CASE("equal_range", "[binary-search]") {
  std::vector<int> values { 1, 2, 3, 3, 4, 4, 5, 5 };
  using iterator = std::vector<int>::iterator;
  core::range<iterator> range { core::equal_range(values, 3) };
  CHECK((begin(values) + 2) == begin(range));
  CHECK((begin(values) + 4) == end(range));
}

TEST_CASE("merge", "[set]") {
  std::vector<int> even { 2, 4, 6, 8, 2, 4 };
  std::vector<int> odds { 1, 3, 5, 7, 1, 3 };
  std::set<int> integers { };
  core::merge(even, odds, ::std::inserter(integers, integers.begin()));
  CHECK(integers.size() == 8u);
}

TEST_CASE("inplace-merge", "[set]") {
  std::vector<int> values { 1, 2, 3, 4, 5, 6, 7, 8 };
  core::inplace_merge(values, values.begin() + 4);
  CHECK(core::is_sorted(values));
}

TEST_CASE("includes", "[set]") {
  std::vector<int> values { 1, 2, 3, 4, 5, 6, 7 };
  std::vector<int> subgroup { 4, 5, 6 };
  CHECK(core::includes(values, subgroup));
}

TEST_CASE("set-difference", "[set]") {
  std::vector<int> values { 1, 2, 3, 5, 6, 7 };
  std::vector<int> diff { 1, 2, 3, 4, 5, 6, 7 };
  std::vector<int> output { };
  core::set_difference(diff, values, ::std::back_inserter(output));
  CHECK(output.size() == 1u);
  CHECK(output.front() == 4);
}

TEST_CASE("set-intersection", "[set]") {
  std::vector<int> first { 1, 2, 3, 4, 5 };
  std::vector<int> second { 5, 6, 7, 8, 9 };
  std::vector<int> output { };
  core::set_intersection(first, second, ::std::back_inserter(output));
  CHECK(output.size() == 1u);
  CHECK(output.front() == 5);
}

TEST_CASE("set-symmetric-difference", "[set]") {
  std::vector<int> first { 1, 2, 3, 4, 5, 6 };
  std::vector<int> second { 1, 2, 3, 4, 5 };
  std::vector<int> output { };
  auto iter = ::std::back_inserter(output);
  core::set_symmetric_difference(first, second, iter);
  CHECK(output.size() == 1u);
  CHECK(output.front() == 6);
}

TEST_CASE("set-union", "[set]") {
  std::vector<int> first { 1, 2, 3, 4, 5 };
  std::vector<int> second { 2, 3, 4, 5, 6 };
  std::vector<int> output { };
  auto iter = ::std::back_inserter(output);
  core::set_union(first, second, iter);
  CHECK(output.size() == 6u);
  CHECK(output[0] == 1);
  CHECK(output[1] == 2);
  CHECK(output[2] == 3);
  CHECK(output[3] == 4);
  CHECK(output[4] == 5);
  CHECK(output[5] == 6);
}

TEST_CASE("is-heap", "[heap]") {
  std::vector<int> values { 9, 5, 4, 1, 1, 3 };
  CHECK(core::is_heap(values));
}

TEST_CASE("is-heap-until", "[heap]") {
  std::vector<int> values { 9, 5, 4, 1, 1, 3, 2, 6 };
  auto result = core::is_heap_until(values);
  CHECK(result != ::std::end(values));
}

TEST_CASE("make-heap", "[heap]") {
  std::vector<int> values { 3, 1, 4, 1, 5, 9 };
  core::make_heap(values);
  CHECK(core::is_heap(values));
}

TEST_CASE("push-heap", "[heap]") {
  std::vector<int> values { 9, 5, 4, 1, 1, 3 };
  values.push_back(6);
  core::push_heap(values);
  CHECK(values[0] == 9);
  CHECK(values[1] == 5);
  CHECK(values[2] == 6);
  CHECK(values[3] == 1);
  CHECK(values[4] == 1);
  CHECK(values[5] == 3);
  CHECK(values[6] == 4);
}

TEST_CASE("pop-heap", "[heap]") {
  std::vector<int> values { 9, 5, 4, 1, 1, 3 };
  core::pop_heap(values);
  values.pop_back();
  CHECK(values[0] == 5);
  CHECK(values[1] == 3);
  CHECK(values[2] == 4);
  CHECK(values[3] == 1);
  CHECK(values[4] == 1);
}

TEST_CASE("sort-heap", "[heap]") {
  std::vector<int> values { 9, 5, 4, 1, 1, 3 };
  core::sort_heap(values);
  CHECK(values[0] == 1);
  CHECK(values[1] == 1);
  CHECK(values[2] == 3);
  CHECK(values[3] == 4);
  CHECK(values[4] == 5);
  CHECK(values[5] == 9);
}

TEST_CASE("max-element", "[min-max]") {
  std::vector<int> values { 9, 5, 4, 1, 1, 3 };
  auto result = core::max_element(values);
  CHECK(result == ::std::begin(values));
  CHECK(*result == 9);
}

TEST_CASE("min-element", "[min-max]") {
  std::vector<int> values { 1, 1, 3, 4, 5, 6 };
  auto result = core::min_element(values);
  CHECK(result == ::std::begin(values));
  CHECK(*result == 1);
}

TEST_CASE("minmax-element", "[min-max]") {
  std::vector<int> values { 9, 4, 5, 6, 1 };
  auto result = core::minmax_element(values);
  CHECK(::std::get<0>(result) == (values.end() - 1));
  CHECK(::std::get<1>(result) == values.begin());
  CHECK(*::std::get<0>(result) == 1);
  CHECK(*::std::get<1>(result) == 9);
}

TEST_CASE("lexicographical-compare", "[min-max]") {
  std::string lhs { "abcd" };
  std::vector<char> rhs { 'b', 'c', 'd', 'e' };
  CHECK(core::lexicographical_compare(lhs, rhs));
}

TEST_CASE("is-permutation", "[min-max]") {
  std::vector<int> lhs { 1, 2, 3, 4, 5 };
  std::vector<int> rhs { 5, 4, 3, 2, 1 };
  CHECK(core::is_permutation(lhs, rhs));
}

TEST_CASE("next_permutation", "[min-max]") {
  std::string text { "abc" };
  CHECK(core::next_permutation(text));
}

TEST_CASE("prev-permutation", "[min-max]") {
  std::string text { "cba" };
  CHECK(core::prev_permutation(text));
}
