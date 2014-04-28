#include <core/algorithm.hpp>
#include <initializer_list>
#include <random>
#include <vector>
#include <set>

#include <unittest/unittest.hpp>

int main () {
  using namespace unittest;

  test("algorithm") = {
    task("all-of") = [] {
      std::vector<int> values { 1, 2, 3, 4, 5 };
      auto result = core::all_of(values, [](int v) { return v > 0; });
      assert::is_true(result);
    },

    task("any-of") = [] {
      std::vector<int> values { 1, 2, 3, 4, 5 };
      auto result = core::any_of(values, [](int v) { return v % 2 == 0; });
      assert::is_true(result);
    },

    task("none-of") = [] {
      std::vector<int> values { 1, 2, 3, 4, 5 };
      auto result = core::none_of(values, [](int v) { return v <= 0; });
      assert::is_true(result);
    },

    task("for-each") = [] {
      std::vector<int> values { 1, 2, 3, 4, 5 };
      core::for_each(values, [](int v) { assert::greater(v, 0); });
    },

    task("count") = [] {
      std::vector<int> values { 1, 2, 1, 1 };
      assert::equal(core::count(values, 1), 3);
    },

    task("count-if") = [] {
      std::vector<int> values { 1, 2, 1 };
      auto result = core::count_if(values, [](int v) { return v % 2; });
      assert::equal(result, 2);
    },

    task("mismatch") = [] {
      auto value = std::string { "abcXYZcba" };
      auto expected = std::string { "abc" };
      auto result = core::mismatch(value, value.rbegin());
      assert::equal(std::string { value.begin(), result.first }, expected);
    },

    task("equal") = [] {
      auto value = std::string { "hello" };
      auto result = core::equal(value, ::std::begin(value));
      assert::is_true(result);
    },

    task("find") = [] {
      auto value = std::string { "find" };
      auto result = core::find(value, 'n');
      assert::not_equal(result, ::std::end(value));
    },

    task("find-if") = [] {
      auto value = std::string { "find-if" };
      auto res = core::find_if(value, [](int ch) {
        return not ::std::isalpha(ch);
      });
      assert::not_equal(res, ::std::end(value));
    },

    task("find-if-not") = [] {
      auto value = std::string { "find-if-not" };
      auto res = core::find_if_not(value,
        [] (int ch) { return ::std::isalpha(ch);
      });
      assert::not_equal(res, ::std::end(value));
    },

    task("find-end") = [] {
      auto value = std::vector<int> { 1, 2, 3, 4, 6, 7, 9, 8, 1, 2, 3 };
      auto subset = std::vector<int> { 1, 2, 3 };

      auto result = core::find_end(value, subset);
      assert::not_equal(result, ::std::end(value));
      assert::equal(::std::distance(::std::begin(value), result), 8);
    },

    task("find-first-of") = [] {
      auto value = std::string { "find-first-of" };
      auto subset = std::string { "o" };
      auto result = core::find_first_of(value, subset);
      assert::not_equal(result, ::std::end(value));
      assert::equal(::std::distance(::std::begin(value), result), 11);
    },

    task("adjacent-find") = [] {
      auto value = std::string { "adjacent-find" };
      auto result = core::adjacent_find(value);
      assert::equal(result, ::std::end(value));
    },

    task("search") = [] {
      auto value = std::string { "searching with the search function" };
      auto result = core::search(value, std::string { "the" });
      assert::not_equal(result, ::std::end(value));
    },

    task("search-n") = [] {
      auto value = std::string { "searching for consecutive letters" };
      auto result = core::search_n(value, 2, 't');
      assert::not_equal(result, ::std::end(value));
    },

    task("copy") = [] {
      std::vector<int> from { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::vector<int> to { };
      std::ignore = core::copy(from, std::back_inserter(to));

      assert::equal(to[0], 0);
      assert::equal(to[1], 1);
      assert::equal(to[2], 2);
      assert::equal(to[3], 3);
      assert::equal(to[4], 4);
      assert::equal(to[5], 5);
      assert::equal(to[6], 6);
      assert::equal(to[7], 7);
      assert::equal(to[8], 8);
      assert::equal(to[9], 9);
    },

    task("copy-if") = [] {
      auto value = std::string { "copy-if" };
      std::string output { };
      std::ignore = core::copy_if(
        value,
        ::std::back_inserter(output),
        [] (char ch) { return ::std::isalpha(ch); }
      );
      assert::equal(output, std::string { "copyif" });
    },

    task("copy-backward") = [] {
      auto value = std::string { "copy-backward" };
      std::string output(value.size(), '\0');
      std::ignore = core::copy_backward(value, ::std::end(output));
      assert::equal(output, value);
    },

    task("move") = [] {
      auto value = std::vector<std::string> { "first", "second", "third" };
      std::vector<std::string> output { 3 };
      std::ignore = core::move(value, ::std::begin(output));

      assert::is_true(value[0].empty());
      assert::is_true(value[1].empty());
      assert::is_true(value[2].empty());

      assert::equal(output[0], std::string { "first" });
      assert::equal(output[1], std::string { "second" });
      assert::equal(output[2], std::string { "third" });
    },

    task("move-backward") = [] {
      auto value = std::vector<std::string> { "first", "second", "third" };
      std::vector<std::string> output { 3 };
      std::ignore = core::move_backward(value, ::std::end(output));

      assert::is_true(value[0].empty());
      assert::is_true(value[1].empty());
      assert::is_true(value[2].empty());

      assert::equal(output[2], std::string { "third" });
      assert::equal(output[1], std::string { "second" });
      assert::equal(output[0], std::string { "first" });
    },

    task("fill") = [] {
      std::vector<int> value { 1, 2, 3, 4, 5, 6, 7 };
      auto predicate = [](int v) { return v == -1; };
      assert::is_true(core::none_of(value, predicate));
      core::fill(value, -1);
      assert::is_true(core::all_of(value, predicate));
    },

    task("transform") = [] {
      std::vector<int> value { 1, 2, 3 };
      std::vector<std::string> output { 3 };
      core::transform(value, ::std::begin(output), [] (int v) -> std::string {
        return std::to_string(v);
      });
      assert::equal(output[0], std::string { "1" });
      assert::equal(output[1], std::string { "2" });
      assert::equal(output[2], std::string { "3" });
    },

    task("transform-if") = [] {
      std::vector<int> value { 1, 2, 3 };
      std::vector<std::string> output { };
      std::vector<std::string> output2 { };

      core::transform_if(
        value,
        ::std::back_inserter(output),
        [] (int v) { return std::to_string(v); },
        [] (int v) { return v % 2 != 0; }
      );

      assert::equal(output.size(), 2u);
      assert::equal(output[0], "1");
      assert::equal(output[1], "3");

      value = { 2, 3 };

      core::transform_if(
        value,
        output,
        ::std::back_inserter(output2),
        [] (int v, std::string const& str) { return std::to_string(v) + str; },
        [] (int v, std::string const&) { return v % 2 != 0 and v > 1; }
      );

      assert::equal(output2.size(), 1u);
      assert::equal(output2[0], "33");
    },

    task("remove") = [] {
      std::string text { "words words words" };
      text.erase(core::remove(text, ' '), ::std::end(text));
      assert::equal(text, std::string { "wordswordswords" });
    },

    task("remove-if") = [] {
      std::string text { "remove-if" };
      text.erase(
        core::remove_if(text, [](char ch) { return not ::std::isalpha(ch); }),
        ::std::end(text)
      );
      assert::equal(text, std::string { "removeif" });
    },

    task("remove-copy") = [] {
      std::string text { "remove-copy" };
      std::string output { };
      std::ignore = core::remove_copy(text, ::std::back_inserter(output), '-');
      assert::equal(output, std::string { "removecopy" });
    },

    task("remove-copy-if") = [] {
      std::string text { "remove-copy-if" };
      std::string output { };
      std::ignore = core::remove_copy_if(
        text,
        ::std::back_inserter(output),
        [] (char ch) { return ::std::isalpha(ch); }
      );
      assert::equal(output, std::string { "--" });
    },

    task("remove-erase") = [] {
      std::vector<int> values { 1, 2, 3, 4, 5 };
      core::remove_erase(values, 3);
      assert::equal(values.size(), 4u);
      assert::equal(values[0], 1);
      assert::equal(values[1], 2);
      assert::equal(values[2], 4);
      assert::equal(values[3], 5);
    },

    task("remove-erase-if") = [] {
      std::vector<int> values { 1, 2, 3, 4, 5 };
      core::remove_erase_if(values, [] (int v) { return v % 2; });
      assert::equal(values.size(), 2u);
      assert::equal(values[0], 2);
      assert::equal(values[1], 4);
    },

    task("replace") = [] {
      std::string text { "replace" };
      core::replace(text, 'e', 'f');
      assert::equal(text, std::string { "rfplacf" });
    },

    task("replace-if") = [] {
      std::string text { "replace-if" };
      core::replace_if(text, [] (char c) { return not std::isalpha(c); }, 'f');
      assert::equal(text, std::string { "replacefif" });
    },

    task("replace-copy") = [] {
      std::string text { "replace-copy" };
      std::string output { };
      core::replace_copy(text, ::std::back_inserter(output), '-', 'f');
      assert::equal(output, std::string { "replacefcopy" });
    },

    task("replace-copy-if") = [] {
      std::string text { "replace-copy-if" };
      std::string output { };
      core::replace_copy_if(
        text,
        ::std::back_inserter(output),
        [] (char ch) { return not std::isalpha(ch); },
        'f'
      );
      assert::equal(output, std::string { "replacefcopyfif" });
    },

    task("swap-ranges") = [] {
      std::string hello { "hello" };
      std::vector<char> world { 'w', 'o', 'r', 'l', 'd' };
      core::swap_ranges(hello, ::std::begin(world));
      assert::equal(hello, std::string { "world" });
    },

    task("reverse") = [] {
      std::string text { "reverse" };
      core::reverse(text);
      assert::equal(text, std::string { "esrever" });
    },

    task("reverse-copy") = [] {
      std::string text { "reverse-copy" };
      std::string output { };
      core::reverse_copy(text, ::std::back_inserter(output));
      assert::equal(output, std::string { "ypoc-esrever" });
    },

    task("rotate") = [] {
      std::vector<int> value { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
      core::rotate(value, ::std::begin(value) + 5);
      assert::equal(value[0], 6);
      assert::equal(value[1], 7);
      assert::equal(value[2], 8);
      assert::equal(value[3], 9);
      assert::equal(value[4], 10);
      assert::equal(value[5], 1);
      assert::equal(value[6], 2);
      assert::equal(value[7], 3);
      assert::equal(value[8], 4);
      assert::equal(value[9], 5);
    },

    task("rotate-copy") = [] {
      std::vector<int> value { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::vector<int> output { };
      auto rotator = ::std::begin(value) + 5;
      core::rotate_copy(value, rotator, ::std::back_inserter(output));
      assert::equal(output[0], 5);
      assert::equal(output[1], 6);
      assert::equal(output[2], 7);
      assert::equal(output[3], 8);
      assert::equal(output[4], 9);
      assert::equal(output[5], 0);
      assert::equal(output[6], 1);
      assert::equal(output[7], 2);
      assert::equal(output[8], 3);
      assert::equal(output[9], 4);
    },

    task("shuffle") = [] {
      std::vector<int> values { 1, 2, 3, 4, 5 };
      std::random_device rd { };
      core::shuffle(values, std::mt19937 { rd() });
    },

    task("unique") = [] {
      std::vector<int> values { 1, 1, 2, 3, 4, 5 };
      auto result = core::unique(values);
      assert::not_equal(result, ::std::end(values));
    },

    task("unique-copy") = [] {
      std::vector<int> values { 1, 1, 2, 3, 4, 5 };
      std::vector<int> output { };
      std::ignore = core::unique_copy(values, ::std::back_inserter(output));
      assert::equal(output[0], 1);
      assert::equal(output[1], 2);
      assert::equal(output[2], 3);
      assert::equal(output[3], 4);
      assert::equal(output[4], 5);
    },

    task("is-partitioned") = [] {
      auto is_even = [] (int v) { return v % 2 == 0; };
      std::vector<int> values { 2, 4, 1, 3, 5 };
      assert::is_true(core::is_partitioned(values, is_even));
    },

    task("partition") = [] {
      auto is_odd = [] (int v) { return v % 2; };
      std::vector<int> values { 1, 2, 3, 4, 5, 6 };
      auto second_group = core::partition(values, is_odd);
      assert::equal(std::distance(second_group, ::std::end(values)), 3);
    },

    task("partition-copy") = [] {
      auto is_odd = [] (int v) { return v % 2; };
      std::vector<int> values { 1, 2, 3, 4, 5, 6 };
      std::vector<int> even { };
      std::vector<int> odds { };
      std::ignore = core::partition_copy(
        values,
        ::std::back_inserter(odds),
        ::std::back_inserter(even),
        is_odd
      );
      assert::is_true(core::none_of(even, is_odd));
      assert::is_true(core::all_of(odds, is_odd));
    },

    task("stable-partition") = [] {
      auto is_odd = [] (int v) { return v % 2; };
      std::vector<int> values { 1, 2, 3, 4, 5, 6 };
      std::ignore = core::stable_partition(values, is_odd);
      assert::equal(values[0], 1);
      assert::equal(values[1], 3);
      assert::equal(values[2], 5);
      assert::equal(values[3], 2);
      assert::equal(values[4], 4);
      assert::equal(values[5], 6);
    },

    task("partition-point") = [] {
      auto is_odd = [] (int v) { return v % 2; };
      std::vector<int> values { 1, 2, 3, 4, 5, 6 };
      auto result = core::stable_partition(values, is_odd);
      auto point = core::partition_point(values, is_odd);
      assert::equal(point, result);
    },

    task("is-sorted") = [] {
      std::vector<int> values { 1, 2, 3, 4, 5 };
      assert::is_true(core::is_sorted(values));
    },

    task("is-sorted-until") = [] {
      std::vector<int> values { 1, 2, 3, 4, 5, 4, 3 };
      assert::not_equal(::std::end(values), core::is_sorted_until(values));
    },

    task("sort") = [] {
      std::vector<int> values { 5, 4, 3, 2, 1 };
      core::sort(values);
      assert::equal(values[0], 1);
      assert::equal(values[1], 2);
      assert::equal(values[2], 3);
      assert::equal(values[3], 4);
      assert::equal(values[4], 5);
    },

    task("partial-sort") = [] {
      std::vector<int> values { 1, 4, 5, 6, 3, 2 };
      core::partial_sort(values, std::begin(values) + 3);
      assert::equal(values[0], 1);
      assert::equal(values[1], 2);
      assert::equal(values[2], 3);
    },

    task("partial-sort-copy") = [] {
      std::vector<int> values { 1, 4, 5, 6, 2, 3 };
      std::vector<int> output(3);
      core::partial_sort_copy(values, output);
      assert::equal(output[0], 1);
      assert::equal(output[1], 2);
      assert::equal(output[2], 3);
    },

    task("stable-sort") = [] {
      struct employee {
        bool operator == (employee const& that) const noexcept {
          return this->age == that.age and this->name == that.name;
        }

        bool operator < (employee const& that) const noexcept {
          return this->age < that.age;
        }

        std::string name;
        std::int32_t age;
      };

      std::vector<employee> values = {
        { "jane smith", 32 },
        { "joe shmoe", 55 },
        { "jocoocoo puhwenis", 39 },
        { "ira glass", 55 },
        { "bubbles", 43 }
      };
      core::stable_sort(values);
      assert::equal(values.back(), employee { "ira glass", 55 });
    },

    task("nth-element") = [] {
      std::vector<int> values { 5, 6, 4, 3, 2, 6, 7, 9, 3 };
      core::nth_element(values, ::std::begin(values) + values.size() / 2);
      assert::equal(values[values.size() / 2], 5);
    },

    task("lower-bound") = [] {
      std::vector<int> values { 1, 2, 3, 4, 5, 6, 7 };
      auto result = core::lower_bound(values, 4);
      assert::not_equal(result, ::std::end(values));
      assert::equal(*result, 4);
    },

    task("upper-bound") = [] {
      std::vector<int> values { 1, 2, 3, 4, 5, 6, 7 };
      auto result = core::upper_bound(values, 4);
      assert::not_equal(result, ::std::end(values));
      assert::equal(*result, 5);
    },

    task("binary-search") = [] {
      std::vector<int> values { 1, 2, 3, 4, 5, 6, 7 };
      assert::is_true(core::binary_search(values, 3));
    },

    task("equal_range") = [] {
      std::vector<int> values { 1, 2, 3, 3, 4, 4, 5, 5 };
      using iterator = std::vector<int>::iterator;
      core::range<iterator> range { core::equal_range(values, 3) };
      assert::equal(::std::begin(values) + 2, ::std::begin(range));
      assert::equal(::std::begin(values) + 4, ::std::end(range));
    },

    task("merge") = [] {
      std::vector<int> even { 2, 4, 6, 8, 2, 4 };
      std::vector<int> odds { 1, 3, 5, 7, 1, 3 };
      std::set<int> integers { };
      core::merge(even, odds, ::std::inserter(integers, integers.begin()));
      assert::equal(integers.size(), 8u);
    },

    task("inplace-merge") = [] {
      std::vector<int> values { 1, 2, 3, 4, 5, 6, 7, 8 };
      core::inplace_merge(values, values.begin() + 4);
      assert::is_true(core::is_sorted(values));
    },

    task("includes") = [] {
      std::vector<int> values { 1, 2, 3, 4, 5, 6, 7 };
      std::vector<int> subgroup { 4, 5, 6 };
      assert::is_true(core::includes(values, subgroup));
    },

    task("set-difference") = [] {
      std::vector<int> values { 1, 2, 3, 5, 6, 7 };
      std::vector<int> diff { 1, 2, 3, 4, 5, 6, 7 };
      std::vector<int> output { };
      core::set_difference(diff, values, ::std::back_inserter(output));
      assert::equal(output.size(), 1u);
      assert::equal(output.front(), 4);
    },

    task("set-intersection") = [] {
      std::vector<int> first { 1, 2, 3, 4, 5 };
      std::vector<int> second { 5, 6, 7, 8, 9 };
      std::vector<int> output { };
      core::set_intersection(first, second, ::std::back_inserter(output));
      assert::equal(output.size(), 1u);
      assert::equal(output.front(), 5);
    },

    task("set-symmetric-difference") = [] {
      std::vector<int> first { 1, 2, 3, 4, 5, 6 };
      std::vector<int> second { 1, 2, 3, 4, 5 };
      std::vector<int> output { };
      auto iter = ::std::back_inserter(output);
      core::set_symmetric_difference(first, second, iter);
      assert::equal(output.size(), 1u);
      assert::equal(output.front(), 6);
    },

    task("set-union") = [] {
      std::vector<int> first { 1, 2, 3, 4, 5 };
      std::vector<int> second { 2, 3, 4, 5, 6 };
      std::vector<int> output { };
      auto iter = ::std::back_inserter(output);
      core::set_union(first, second, iter);
      assert::equal(output.size(), 6u);
      assert::equal(output[0], 1);
      assert::equal(output[1], 2);
      assert::equal(output[2], 3);
      assert::equal(output[3], 4);
      assert::equal(output[4], 5);
      assert::equal(output[5], 6);
    },

    task("is-heap") = [] {
      std::vector<int> values { 9, 5, 4, 1, 1, 3 };
      assert::is_true(core::is_heap(values));
    },

    task("is-heap-until") = [] {
      std::vector<int> values { 9, 5, 4, 1, 1, 3, 2, 6 };
      auto result = core::is_heap_until(values);
      assert::not_equal(result, ::std::end(values));
    },

    task("make-heap") = [] {
      std::vector<int> values { 3, 1, 4, 1, 5, 9 };
      core::make_heap(values);
      assert::is_true(core::is_heap(values));
    },

    task("push-heap") = [] {
      std::vector<int> values { 9, 5, 4, 1, 1, 3 };
      values.push_back(6);
      core::push_heap(values);
      assert::equal(values[0], 9);
      assert::equal(values[1], 5);
      assert::equal(values[2], 6);
      assert::equal(values[3], 1);
      assert::equal(values[4], 1);
      assert::equal(values[5], 3);
      assert::equal(values[6], 4);
    },

    task("pop-heap") = [] {
      std::vector<int> values { 9, 5, 4, 1, 1, 3 };
      core::pop_heap(values);
      values.pop_back();
      assert::equal(values[0], 5);
      assert::equal(values[1], 3);
      assert::equal(values[2], 4);
      assert::equal(values[3], 1);
      assert::equal(values[4], 1);
    },

    task("sort-heap") = [] {
      std::vector<int> values { 9, 5, 4, 1, 1, 3 };
      core::sort_heap(values);
      assert::equal(values[0], 1);
      assert::equal(values[1], 1);
      assert::equal(values[2], 3);
      assert::equal(values[3], 4);
      assert::equal(values[4], 5);
      assert::equal(values[5], 9);
    },

    task("max-element") = [] {
      std::vector<int> values { 9, 5, 4, 1, 1, 3 };
      auto result = core::max_element(values);
      assert::equal(result, ::std::begin(values));
      assert::equal(*result, 9);
    },

    task("min-element") = [] {
      std::vector<int> values { 1, 1, 3, 4, 5, 6 };
      auto result = core::min_element(values);
      assert::equal(result, ::std::begin(values));
      assert::equal(*result, 1);
    },

    task("minmax-element") = [] {
      std::vector<int> values { 9, 4, 5, 6, 1 };
      auto result = core::minmax_element(values);
      assert::equal(::std::get<0>(result), values.end() - 1);
      assert::equal(::std::get<1>(result), values.begin());
      assert::equal(*::std::get<0>(result), 1);
      assert::equal(*::std::get<1>(result), 9);
    },

    task("lexicographical-compare") = [] {
      std::string lhs { "abcd" };
      std::vector<char> rhs { 'b', 'c', 'd', 'e' };
      assert::is_true(core::lexicographical_compare(lhs, rhs));
    },

    task("is-permutation") = [] {
      std::vector<int> lhs { 1, 2, 3, 4, 5 };
      std::vector<int> rhs { 5, 4, 3, 2, 1 };
      assert::is_true(core::is_permutation(lhs, rhs));
    },

    task("next_permutation") = [] {
      std::string text { "abc" };
      assert::is_true(core::next_permutation(text));
    },

    task("prev-permutation") = [] {
      std::string text { "cba" };
      assert::is_true(core::prev_permutation(text));
    }
  };

  monitor::run();
}
