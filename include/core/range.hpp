#ifndef CORE_RANGE_HPP
#define CORE_RANGE_HPP

#include <iterator>
#include <istream>
#include <utility>
#include <memory>

#include <cstdlib>

namespace core {
inline namespace v1 {
namespace impl {

template <typename T>
class begin {
  template <typename U>
  static auto check (U&& u) noexcept -> decltype(std::begin(u));
  static void check (...) noexcept(false);
public:
  using type = decltype(check(std::declval<T>()));
  static constexpr bool value = noexcept(check(std::declval<T>()));
};

template <typename T>
class end {
  template <typename U>
  static auto check (U&& u) noexcept -> decltype(std::end(u));
  static void check (...) noexcept(false);
public:
  using type = decltype(check(std::declval<T>()));
  static constexpr bool value = noexcept(check(std::declval<T>()));
};

} /* namespace impl */

template <typename R>
struct is_range : std::integral_constant<bool,
  impl::begin<R>::value and impl::end<R>::value
> { };

template <typename Iterator>
struct range {
  using traits = std::iterator_traits<Iterator>;

  using iterator_category = typename traits::iterator_category;

  using difference_type = typename traits::difference_type;
  using value_type = typename traits::value_type;

  using reference = typename traits::reference;
  using pointer = typename traits::pointer;

  using iterator = Iterator;

  static constexpr bool is_input = std::is_convertible<
    iterator_category,
    std::input_iterator_tag
  >::value;

  static constexpr bool is_output = std::is_convertible<
    iterator_category,
    std::output_iterator_tag
  >::value;

  static constexpr bool is_forward = std::is_convertible<
    iterator_category,
    std::forward_iterator_tag
  >::value;

  static constexpr bool is_bidirectional = std::is_convertible<
    iterator_category,
    std::bidirectional_iterator_tag
  >::value;

  static constexpr bool is_random_access = std::is_convertible<
    iterator_category,
    std::random_access_iterator_tag
  >::value;

  template <
    typename Range,
    typename=typename std::enable_if<
      not std::is_pointer<iterator>::value and
      is_range<Range>::value and
      std::is_convertible<typename impl::begin<Range>::type, iterator>::value
    >::type
  > explicit range (Range&& r) noexcept :
    range { std::begin(r), std::end(r) }
  { }

  range (std::pair<iterator, iterator> pair) noexcept :
    range { std::get<0>(pair), std::get<1>(pair) }
  { }

  range (iterator begin_, iterator end_) noexcept :
    begin_ { begin_ },
    end_ { end_ }
  { }

  range (range const& that) :
    range { that.begin_, that.end_ }
  { }

  range (range&& that) noexcept :
    range { std::move(that.begin_), std::move(that.end_) }
  { }

  range () = default;
  ~range () = default;

  iterator begin () const { return this->begin_; }
  iterator end () const { return this->end_; }

  reference front () const { return *this->begin(); }
  reference back () const {
    static_assert(is_bidirectional, "can only get back of bidirectional");
    return *std::prev(this->end());
  }

  reference operator [](difference_type idx) const {
    static_assert(is_random_access, "can only subscript into random-access");
    return idx < 0 ? this->end()[idx] : this->begin()[idx];
  }

  bool empty () const { return this->begin() == this->end(); }

  difference_type size () const {
    static_assert(is_forward, "can only get size of forward-range");
    return std::distance(this->begin(), this->end());
  }

  /* Creates an open-ended range of [start, stop) */
  range slice (difference_type start, difference_type stop) const {
    static_assert(is_forward, "can only slice forward-range");
    return range { };
  }

  /* Creates an open-ended range of [start, end()) */
  range slice (difference_type start) const {
    static_assert(is_forward, "can only slice forward-range");
    return range { split(start).second };
  }

  std::pair<range, range> split (difference_type idx) const {
    static_assert(is_forward,"can only split a forward-range");
    if (idx >= 0) {
      range second { *this };
      second.pop_front_upto(idx);
      return std::make_pair(range { this->begin(), second.begin() }, second);
    }

    range first { *this };
    first.pop_back_upto(-idx);
    return std::make_pair(first, range { first.end(), this->end() });
  }

  /* mutates range */
  void pop_front (difference_type n) { std::advance(this->begin_, n); }
  void pop_front () { ++this->begin_; }

  void pop_back (difference_type n) {
    static_assert(is_bidirectional, "can only pop-back bidirectional-range");
    std::advance(this->end_, -n);
  }

  void pop_back () {
    static_assert(is_bidirectional, "can only pop-back bidirectional-range");
    --this->end_;
  }

  /* Negative argument causes no change */
  void pop_front_upto (difference_type n) {
    std::advance(
      this->begin_,
      std::min(std::max<difference_type>(0, n), this->size())
    );
  }

  /* Negative argument causes no change */
  void pop_back_upto (difference_type n) {
    static_assert(is_bidirectional, "can only pop-back-upto bidirectional");
    std::advance(
      this->end_,
      -std::min(std::max<difference_type>(0, n), this->size())
    );
  }

  void swap (range& that) noexcept {
    std::swap(this->begin_, that.begin_);
    std::swap(this->end_, that.end_);
  }

private:
  iterator begin_;
  iterator end_;
};

template <typename Iterator>
auto make_range (Iterator begin, Iterator end) -> range<Iterator>;

template <typename Range>
auto make_range (Range&& value) -> range<decltype(std::begin(value))> {
  return make_range(std::begin(value), std::end(value));
}

template <typename Range>
auto make_ptr_range (Range&& value) -> range<
  decltype(std::addressof(*std::begin(value)))
>;

/* Used like: core::make_range<char>(std::cin) */
template <
  typename T,
  typename CharT,
  typename Traits=std::char_traits<CharT>
> auto make_range (std::basic_istream<CharT, Traits>& stream) -> range<
  std::istream_iterator<T, CharT, Traits>
> {
  using iterator = std::istream_iterator<T, CharT, Traits>;
  return make_range(iterator { stream }, iterator { });
}

template <typename CharT, typename Traits=std::char_traits<CharT>>
auto make_range (std::basic_streambuf<CharT, Traits>* buffer) -> range<
  std::istreambuf_iterator<CharT, Traits>
> {
  using iterator = std::istreambuf_iterator<CharT, Traits>;
  return make_range(iterator { buffer }, iterator { });
}

template <typename CharT, typename Traits=std::char_traits<CharT>>
auto make_range (std::basic_istream<CharT, Traits>& stream) -> range<
  std::istreambuf_iterator<CharT, Traits>
> { return make_range(stream.rdbuf()); }

}} /* namespace core::v1 */

namespace std {

template <typename Iterator>
void swap (
  core::v1::range<Iterator>& lhs,
  core::v1::range<Iterator>& rhs
) noexcept { lhs.swap(rhs); }

} /* namespace std */

#endif /* CORE_RANGE_HPP */
