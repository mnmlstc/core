#ifndef CORE_ALGORITHM_HPP
#define CORE_ALGORITHM_HPP

#include <algorithm>

#include <core/range.hpp>

namespace core {
inline namespace v1 {

/* non-modifying sequence algorithms */
template <class Range, class UnaryPredicate>
auto all_of (Range&& rng, UnaryPredicate&& p) -> enable_if_t<
  is_range<Range>::value,
  bool
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "all_of requires InputIterators");
  return ::std::all_of(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<UnaryPredicate>(p)
  );
}

template <class Range, class UnaryPredicate>
auto any_of (Range&& rng, UnaryPredicate&& p) -> enable_if_t<
  is_range<Range>::value,
  bool
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "any_of requires InputIterators");
  return ::std::any_of(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<UnaryPredicate>(p)
  );
}

template <class Range, class UnaryPredicate>
auto none_of (Range&& rng, UnaryPredicate&& p) -> enable_if_t<
  is_range<Range>::value,
  bool
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "none_of requires InputIterators");
  return ::std::none_of(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<UnaryPredicate>(p)
  );
}

template <class Range, class UnaryFunction>
auto for_each (Range&& rng, UnaryFunction&& f) -> enable_if_t<
  is_range<Range>::value,
  decay_t<UnaryFunction>
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "for_each requires InputIterators");
  return ::std::for_each(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<UnaryFunction>(f)
  );
}

template <class Range, class T>
auto count (Range&& rng, T const& value) -> enable_if_t<
  is_range<Range>::value,
  decltype(
    ::std::count(
      ::std::begin(::std::forward<Range>(rng)),
      ::std::end(::std::forward<Range>(rng)),
      value
    )
  )
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "count requires InputIterators");
  return ::std::count(::std::begin(range), ::std::end(range), value);
}

template <class Range, class UnaryPredicate>
auto count_if (Range&& rng, UnaryPredicate&& p) -> enable_if_t<
  is_range<Range>::value,
  decltype(
    ::std::count_if(
      ::std::begin(::std::forward<Range>(rng)),
      ::std::end(::std::forward<Range>(rng)),
      ::std::forward<UnaryPredicate>(p)
    )
  )
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "count_if requires InputIterators");
  return ::std::count_if(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<UnaryPredicate>(p)
  );
}

template <class Range, class InputIt>
auto mismatch(Range&& rng, InputIt&& it) -> enable_if_t<
  is_range<Range>::value,
  std::pair<
    decltype(::std::begin(std::forward<Range>(rng))),
    decay_t<InputIt>
  >
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "mismatch requires InputIterators");
  return ::std::mismatch(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<InputIt>(it)
  );
}

template <class Range, class InputIt, class BinaryPredicate>
auto mismatch(Range&& rng, InputIt&& it, BinaryPredicate&& bp) -> enable_if_t<
  is_range<Range>::value,
  std::pair<
    decltype(::std::begin(std::forward<Range>(rng))),
    decay_t<InputIt>
  >
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "mismatch requires InputIterators");
  return ::std::mismatch(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<InputIt>(it),
    ::std::forward<BinaryPredicate>(bp)
  );
}

template <class Range, class InputIt>
auto equal (Range&& rng, InputIt&& it) -> enable_if_t<
  is_range<Range>::value,
  bool
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "equal requires InputIterators");
  return ::std::equal(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<InputIt>(it)
  );
}

template <class Range, class InputIt, class BinaryPredicate>
auto equal (Range&& rng, InputIt&& it, BinaryPredicate&& bp) -> enable_if_t<
  is_range<Range>::value,
  bool
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "equal requires InputIterators");
  return ::std::equal(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<InputIt>(it),
    ::std::forward<BinaryPredicate>(bp)
  );
}

template <class Range, class T>
auto find (Range&& rng, T const& value) -> enable_if_t<
  is_range<Range>::value,
  decltype(::std::begin(::std::forward<Range>(rng)))
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "find requires InputIterators");
  return ::std::find(::std::begin(range), ::std::end(range), value);
}

template <class Range, class UnaryPredicate>
auto find_if (Range&& rng, UnaryPredicate&& p) -> enable_if_t<
  is_range<Range>::value,
  decltype(::std::begin(::std::forward<Range>(rng)))
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "find_if requires InputIterators");
  return ::std::find_if(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<UnaryPredicate>(p)
  );
}

template <class Range, class UnaryPredicate>
auto find_if_not (Range&& rng, UnaryPredicate&& p) -> enable_if_t<
  is_range<Range>::value,
  decltype(::std::begin(::std::forward<Range>(rng)))
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "find_if_not requires InputIterators");
  return ::std::find_if_not(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<UnaryPredicate>(p)
  );
}

template <class Range1, class Range2>
auto find_end (Range1&& rng1, Range2&& rng2) -> enable_if_t<
  all_traits<is_range<Range1>, is_range<Range2>>::value,
  decltype(::std::begin(::std::forward<Range1>(rng1)))
> {
  auto range1 = make_range(::std::forward<Range1>(rng1));
  auto range2 = make_range(::std::forward<Range2>(rng2));
  constexpr auto is_forward1 = decltype(range1)::is_forward;
  constexpr auto is_forward2 = decltype(range2)::is_forward;
  static_assert(is_forward1, "find_end requires ForwardIterators");
  static_assert(is_forward2, "find_end requires ForwardIterators");
  return ::std::find_end(
    ::std::begin(range1),
    ::std::end(range1),
    ::std::begin(range2),
    ::std::end(range2)
  );
}

template <class Range1, class Range2, class BinaryPred>
auto find_end (Range1&& rng1, Range2&& rng2, BinaryPred& bp) -> enable_if_t<
  all_traits<is_range<Range1>, is_range<Range2>>::value,
  decltype(::std::begin(::std::forward<Range1>(rng1)))
> {
  auto range1 = make_range(::std::forward<Range1>(rng1));
  auto range2 = make_range(::std::forward<Range2>(rng2));
  constexpr auto is_forward1 = decltype(range1)::is_forward;
  constexpr auto is_forward2 = decltype(range2)::is_forward;
  static_assert(is_forward1, "find_end requires ForwardIterators");
  static_assert(is_forward2, "find_end requires ForwardIterators");
  return ::std::find_end(
    ::std::begin(range1),
    ::std::end(range1),
    ::std::begin(range2),
    ::std::end(range2),
    ::std::forward<BinaryPred>(bp)
  );
}

template <class IRange, class FRange>
auto find_first_of (IRange&& irng, FRange&& frng) -> enable_if_t<
  all_traits<is_range<IRange>, is_range<FRange>>::value,
  decltype(::std::begin(::std::forward<IRange>(irng)))
> {
  auto irange = make_range(::std::forward<IRange>(irng));
  auto frange = make_range(::std::forward<FRange>(frng));
  constexpr auto is_input = decltype(irange)::is_input;
  constexpr auto is_forward = decltype(frange)::is_forward;
  static_assert(is_input, "find_first_of requires InputIterators");
  static_assert(is_forward, "find_first_of requires ForwardIterators");
  return ::std::find_first_of(
    ::std::begin(irange),
    ::std::end(irange),
    ::std::begin(frange),
    ::std::end(frange)
  );
}

template <class IRange, class FRange, class BinaryPred>
auto find_first_of (
  IRange&& irng,
  FRange&& frng,
  BinaryPred&& bp
) -> enable_if_t<
  all_traits<is_range<IRange>, is_range<FRange>>::value,
  decltype(::std::begin(::std::forward<IRange>(irng)))
> {
  auto irange = make_range(::std::forward<IRange>(irng));
  auto frange = make_range(::std::forward<FRange>(frng));
  constexpr auto is_input = decltype(irange)::is_input;
  constexpr auto is_forward = decltype(frange)::is_forward;
  static_assert(is_input, "find_first_of requires InputIterators");
  static_assert(is_forward, "find_first_of requires ForwardIterators");
  return ::std::find_first_of(
    ::std::begin(irange),
    ::std::end(irange),
    ::std::begin(frange),
    ::std::end(frange),
    ::std::forward<BinaryPred>(bp)
  );
}

template <class Range>
auto adjacent_find (Range&& rng) -> enable_if_t<
  is_range<Range>::value,
  decltype(::std::begin(::std::forward<Range>(rng)))
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_forward = decltype(range)::is_forward;
  static_assert(is_forward, "adjacent_find requires ForwardIterators");
  return ::std::adjacent_find(::std::begin(range), ::std::end(range));
}

template <class Range, class BinaryPredicate>
auto adjacent_find (Range&& rng, BinaryPredicate&& bp) -> enable_if_t<
  is_range<Range>::value,
  decltype(::std::begin(::std::forward<Range>(rng)))
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_forward = decltype(range)::is_forward;
  static_assert(is_forward, "adjacent_find requires ForwardIterators");
  return ::std::adjacent_find(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<BinaryPredicate>(bp)
  );
}

template <class Range1, class Range2>
auto search (Range1&& rng1, Range2&& rng2) -> enable_if_t<
  all_traits<is_range<Range1>, is_range<Range2>>::value,
  decltype(::std::begin(::std::forward<Range1>(rng1)))
> {
  auto range1 = make_range(::std::forward<Range1>(rng1));
  auto range2 = make_range(::std::forward<Range2>(rng2));
  constexpr auto is_forward1 = decltype(range1)::is_forward;
  constexpr auto is_forward2 = decltype(range2)::is_forward;
  static_assert(is_forward1, "search requires ForwardIterators");
  static_assert(is_forward2, "search requires ForwardIterators");
  return ::std::search(
    ::std::begin(range1),
    ::std::end(range1),
    ::std::begin(range2),
    ::std::end(range2)
  );
}

template <class Range1, class Range2, class BinaryPred>
auto search (Range1&& rng1, Range2&& rng2, BinaryPred&& bp) -> enable_if_t<
  all_traits<is_range<Range1>, is_range<Range2>>::value,
  decltype(::std::begin(::std::forward<Range1>(rng1)))
> {
  auto range1 = make_range(::std::forward<Range1>(rng1));
  auto range2 = make_range(::std::forward<Range2>(rng2));
  constexpr auto is_forward1 = decltype(range1)::is_forward;
  constexpr auto is_forward2 = decltype(range2)::is_forward;
  static_assert(is_forward1, "search requires ForwardIterators");
  static_assert(is_forward2, "search requires ForwardIterators");
  return ::std::search(
    ::std::begin(range1),
    ::std::end(range1),
    ::std::begin(range2),
    ::std::end(range2),
    ::std::forward<BinaryPred>(bp)
  );
}

template <class Range, class Size, class T>
auto search_n (Range&& rng, Size&& count, T const& value) -> enable_if_t<
  is_range<Range>::value,
  decltype(::std::begin(::std::forward<Range>(rng)))
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_forward = decltype(range)::is_forward;
  static_assert(is_forward, "search_n requires ForwardIterators");
  return ::std::search_n(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<Size>(count),
    value
  );
}

template <class Range, class Size, class T, class BinaryPred>
auto search_n (
  Range&& rng,
  Size&& count,
  T const& value,
  BinaryPred&& bp
) -> enable_if_t<
  is_range<Range>::value,
  decltype(::std::begin(::std::forward<Range>(rng)))
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_forward = decltype(range)::is_forward;
  static_assert(is_forward, "search_n requires ForwardIterators");
  return ::std::search_n(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<Size>(count),
    value,
    ::std::forward<BinaryPred>(bp)
  );
}

/* modifying sequence algorithms */
template <class Range, class OutputIt>
auto copy (Range&& rng, OutputIt&& it) -> enable_if_t<
  is_range<Range>::value,
  decay_t<OutputIt>
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "copy requires InputIterators");
  return ::std::copy(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<OutputIt>(it)
  );
}

template <class Range, class OutputIt, class UnaryPredicate>
auto copy_if (Range&& rng, OutputIt&& it, UnaryPredicate&& up) -> enable_if_t<
  is_range<Range>::value,
  decay_t<OutputIt>
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "copy_if requires InputIterators");
  return ::std::copy_if(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<OutputIt>(it),
    ::std::forward<UnaryPredicate>(up)
  );
}

template <class Range, class BidirIt>
auto copy_backward (Range&& rng, BidirIt&& it) -> enable_if_t<
  is_range<Range>::value,
  decay_t<BidirIt>
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_bidir = decltype(range)::is_bidirectional;
  static_assert(is_bidir, "copy_backward requires BidirectionalIterators");
  return ::std::copy_backward(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<BidirIt>(it)
  );
}

template <class Range, class OutputIt>
auto move (Range&& rng, OutputIt&& it) -> enable_if_t<
  is_range<Range>::value,
  decay_t<OutputIt>
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "move requires InputIterators");
  return ::std::move(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<OutputIt>(it)
  );
}

template <class Range, class BidirIt>
auto move_backward (Range&& rng, BidirIt&& it) -> enable_if_t<
  is_range<Range>::value,
  decay_t<BidirIt>
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_bidir = decltype(range)::is_bidirectional;
  static_assert(is_bidir, "move_backward requires BidirectionalIterators");
  return ::std::move_backward(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<BidirIt>(it)
  );
}

template <class Range, class T>
auto fill (Range&& rng, T const& value) -> enable_if_t<
  is_range<Range>::value
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_forward = decltype(range)::is_forward;
  static_assert(is_forward, "fill requires ForwardIterators");
  return ::std::fill(::std::begin(range), ::std::end(range), value);
}

template <class Range, class OutputIt, class UnaryOperation>
auto transform (
  Range&& rng,
  OutputIt&& it,
  UnaryOperation&& op
) -> enable_if_t<
  is_range<Range>::value,
  decay_t<OutputIt>
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "transform requires InputIterators");
  return ::std::transform(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<OutputIt>(it),
    ::std::forward<UnaryOperation>(op)
  );
}

template <class Range1, class Range2, class OutputIt, class BinaryOperation>
auto transform (
  Range1&& rng1,
  Range2&& rng2,
  OutputIt&& it,
  BinaryOperation&& op
) -> enable_if_t<
  all_traits<is_range<Range1>, is_range<Range2>>::value,
  decay_t<OutputIt>
> {
  auto range1 = make_range(::std::forward<Range1>(rng1));
  auto range2 = make_range(::std::forward<Range2>(rng2));
  constexpr auto is_input1 = decltype(range1)::is_input;
  constexpr auto is_input2 = decltype(range2)::is_input;
  static_assert(is_input1, "transform requires InputIterators");
  static_assert(is_input2, "transform requires InputIterators");
  return ::std::transform(
    ::std::begin(range2),
    ::std::end(range2),
    ::std::begin(range2),
    ::std::forward<OutputIt>(it),
    ::std::forward<BinaryOperation>(op)
  );
}

template <class Range, class T>
auto remove (Range&& rng, T const& value) -> enable_if_t<
  is_range<Range>::value,
  decltype(::std::begin(::std::forward<Range>(rng)))
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_forward = decltype(range)::is_forward;
  static_assert(is_forward, "remove requires ForwardIterators");
  return ::std::remove(::std::begin(range), ::std::end(range), value);
}

template <class Range, class UnaryPredicate>
auto remove_if (Range&& rng, UnaryPredicate&& up) -> enable_if_t<
  is_range<Range>::value,
  decltype(::std::begin(::std::forward<Range>(rng)))
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_forward = decltype(range)::is_forward;
  static_assert(is_forward, "remove_if requires ForwardIterators");
  return ::std::remove_if(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<UnaryPredicate>(up)
  );
}

template <class Range, class OutputIt, class T>
auto remove_copy (Range&& rng, OutputIt&& it, T const& value) -> enable_if_t<
  is_range<Range>::value,
  decay_t<OutputIt>
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "remove_copy requires InputIterators");
  return ::std::remove_copy(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<OutputIt>(it),
    value
  );
}

template <class Range, class OutputIt, class UnaryPred>
auto remove_copy_if (Range&& rng, OutputIt&& it, UnaryPred&& up) -> enable_if_t<
  is_range<Range>::value,
  decay_t<OutputIt>
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "remove_copy_if requires InputIterators");
  return ::std::remove_copy_if(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<OutputIt>(it),
    ::std::forward<UnaryPred>(up)
  );
}

template <class Range, class T>
auto replace (Range&& rng, T const& old, T const& value) -> enable_if_t<
  is_range<Range>::value
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_forward = decltype(range)::is_input;
  static_assert(is_forward, "replace requires ForwardIterators");
  return ::std::replace(
    ::std::begin(range),
    ::std::end(range),
    old,
    value
  );
}

template <class Range, class UnaryPred, class T>
auto replace_if (Range&& rng, UnaryPred&& up, T const& value) -> enable_if_t<
  is_range<Range>::value
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_forward = decltype(range)::is_forward;
  static_assert(is_forward, "replace_if requires ForwardIterators");
  return ::std::replace_if(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<UnaryPred>(up),
    value
  );
}

template <class Range, class OutputIt, class T>
auto replace_copy (Range&& rng, OutputIt&& it, T const& value) -> enable_if_t<
  is_range<Range>::value,
  decay_t<OutputIt>
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "replace_copy requires InputIterators");
  return ::std::replace_copy(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<OutputIt>(it),
    value
  );
}

template <class Range, class OutputIt, class UnaryPred, class T>
auto replace_copy_if (
  Range&& rng,
  OutputIt&& it,
  UnaryPred&& up,
  T const& value
) -> enable_if_t<
  is_range<Range>::value,
  decay_t<OutputIt>
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "replace_copy_if requires InputIterators");
  return ::std::replace_copy_if(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<OutputIt>(it),
    ::std::forward<UnaryPred>(up),
    value
  );
}

template <class Range>
auto reverse (Range&& rng) -> enable_if_t<is_range<Range>::value> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_bidir = decltype(range)::is_bidirectional;
  static_assert(is_bidir, "reverse requires BidirectionalIterators");
  return ::std::reverse(::std::begin(range), ::std::end(range));
}

template <class Range, class OutputIt>
auto reverse_copy (Range&& rng, OutputIt&& it) -> enable_if_t<
  is_range<Range>::value,
  decay_t<OutputIt>
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_bidir = decltype(range)::is_bidirectional;
  static_assert(is_bidir, "reverse_copy requires BidirectionalIterators");
  return ::std::reverse_copy(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<OutputIt>(it)
  );
}

template <class Range, class ForwardIt>
auto rotate (Range&& rng, ForwardIt&& it) -> enable_if_t<
  is_range<Range>::value
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_forward = decltype(range)::is_forward;
  static_assert(is_forward, "rotate requires ForwardIterators");
  return ::std::rotate(
    ::std::begin(range),
    ::std::forward<ForwardIt>(it),
    ::std::end(range)
  );
}

template <class Range, class ForwardIt, class OutputIt>
auto rotate_copy (Range&& rng, ForwardIt&& it, OutputIt&& ot) -> enable_if_t<
  is_range<Range>::value
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_forward = decltype(range)::is_forward;
  static_assert(is_forward, "rotate_copy requires ForwardIterators");
  return ::std::rotate_copy(
    ::std::begin(range),
    ::std::forward<ForwardIt>(it),
    ::std::end(range),
    ::std::forward<OutputIt>(ot)
  );
}

template <class Range, class URNG>
auto shuffle (Range&& rng, URNG&& g) -> enable_if_t<
  is_range<Range>::value
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_random = decltype(range)::is_random;
  static_assert(is_random, "shuffle requires RandomIterators");
  return ::std::shuffle(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<URNG>(g)
  );
}

template <class Range>
auto unique (Range&& rng) -> enable_if_t<
  is_range<Range>::value,
  decltype(::std::begin(::std::forward<Range>(rng)))
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_forward = decltype(range)::is_forward;
  static_assert(is_forward, "unique requires ForwardIterators");
  return ::std::unique(::std::begin(range), ::std::end(range));
}

template <class Range, class BinaryPredicate>
auto unique (Range&& rng, BinaryPredicate&& bp) -> enable_if_t<
  is_range<Range>::value,
  decltype(::std::begin(::std::forward<Range>(rng)))
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_forward = decltype(range)::is_forward;
  static_assert(is_forward, "unique requires ForwardIterators");
  return ::std::unique(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<BinaryPredicate>(bp)
  );
}

template <class Range, class OutputIt>
auto unique_copy (Range&& rng, OutputIt&& it) -> enable_if_t<
  is_range<Range>::value,
  decay_t<OutputIt>
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "unique_copy requires InputIterators");
  return ::std::unique_copy(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<OutputIt>(it)
  );
}

template <class Range, class OutputIt, class BinaryPred>
auto unique_copy (Range&& rng, OutputIt&& it, BinaryPred&& bp) -> enable_if_t<
  is_range<Range>::value,
  decay_t<OutputIt>
> {
  auto range = make_range(::std::forward<Range>(rng));
  constexpr auto is_input = decltype(range)::is_input;
  static_assert(is_input, "unique_copy requires InputIterators");
  return ::std::unique_copy(
    ::std::begin(range),
    ::std::end(range),
    ::std::forward<OutputIt>(it)
  );
}



}} /* namespace core::v1 */

#endif /* CORE_ALGORITHM_HPP */
