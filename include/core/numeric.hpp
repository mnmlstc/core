#ifndef CORE_NUMERIC_HPP
#define CORE_NUMERIC_HPP

#include <numeric>

#include <core/range.hpp>

namespace core {
inline namespace v1 {

template <class Range, class T>
auto iota (Range&& rng, T value) -> enable_if_t<is_range<Range>::value> {
  auto range = make_range(std::forward<Range>(rng));
  constexpr auto is_forward = decltype(range)::is_forward;
  static_assert(is_forward, "iota requires ForwardIterators");
  return ::std::iota(::std::begin(range), ::std::end(range), value);
}

template <class Range, class T>
auto accumulate (Range&& rng, T init) -> enable_if_t<
  is_range<Range>::value,
  T
> {
  auto range = make_range(std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "accumulate requires InputIterators");
  return ::std::accumulate(::std::begin(range), ::std::end(range), init);
}

/* TODO: Look into additional SFINAE checking for use with BinaryOp */
template <class Range, class T, class BinaryOp>
auto accumulate (Range&& rng, T init, BinaryOp op) -> enable_if_t<
  is_range<Range>::value,
  T
> {
  auto range = make_range(std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "accumulate requires InputIterators");
  return ::std::accumulate(::std::begin(range), ::std::end(range), init, op);
}

template <class Range, class InputIt, class T>
auto inner_product (Range&& rng, InputIt first, T value) -> enable_if_t<
  is_range<Range>::value,
  T
> {
  auto range = make_range(std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "inner_product requires InputIterators");
  return ::std::inner_product(
    ::std::begin(range),
    ::std::end(range),
    first,
    value
  );
}

template <class Range, class InputIt, class T, class BinaryOp, class BinaryOp2>
auto inner_product (
  Range&& rng,
  InputIt first,
  T value,
  BinaryOp op,
  BinaryOp2 op2
) -> enable_if_t<is_range<Range>::value, T> {
  auto range = make_range(std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "inner_product requires InputIterators");
  return ::std::inner_product(
    ::std::begin(range),
    ::std::end(range),
    first,
    value,
    op,
    op2
  );
}

template <class Range, class OutputIt>
auto adjacent_difference (Range&& rng, OutputIt it) -> enable_if_t<
  is_range<Range>::value,
  OutputIt
> {
  auto range = make_range(std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "adjacent_difference requires InputIterators");
  return ::std::adjacent_difference(::std::begin(range), ::std::end(range), it);
}

template <class Range, class OutputIt, class BinaryOp>
auto adjacent_difference (Range&& rng, OutputIt it, BinaryOp op) -> enable_if_t<
  is_range<Range>::value,
  OutputIt
> {
  auto range = make_range(std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "adjacent_difference requires InputIterators");
  return ::std::adjacent_difference(
    ::std::begin(range),
    ::std::end(range),
    it,
    op
  );
}

template <class Range, class OutputIt>
auto partial_sum (Range&& rng, OutputIt it) -> enable_if_t<
  is_range<Range>::value,
  OutputIt
> {
  auto range = make_range(std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "partial_sum requires InputIterators");
  return ::std::partial_sum(::std::begin(range), ::std::end(range), it);
}

template <class Range, class OutputIt, class BinaryOp>
auto partial_sum (Range&& rng, OutputIt it, BinaryOp op) -> enable_if_t<
  is_range<Range>::value,
  OutputIt
> {
  auto range = make_range(std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "partial_sum requires InputIterators");
  return ::std::partial_sum(::std::begin(range), ::std::end(range), it, op);
}

}} /* namespace core::v1 */

#endif /* CORE_NUMERIC_HPP */
