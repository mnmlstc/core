#ifndef CORE_ITERATOR_HPP
#define CORE_ITERATOR_HPP

#include <functional>
#include <iterator>
#include <ostream>

namespace core {
inline namespace v1 {

/* capacity */
template <class Container>
constexpr auto size (Container const& container) noexcept -> decltype(
  container.size()
) { return container.size(); }

template <class T, ::std::size_t N>
constexpr ::std::size_t size (T const (&)[N]) noexcept { return N; }

template <class Container>
constexpr bool empty (Container const& container) noexcept {
  return container.empty();
}

template <class T, std::size_t N>
constexpr bool empty (T const (&)[N]) noexcept { return false; }

/* element access */
template <class Container>
constexpr auto front (Container const& container) -> decltype(
  container.front()
) { return container.front(); }

template <class Container>
constexpr auto front (Container& container) -> decltype(container.front()) {
  return container.front();
}

template <class T, ::std::size_t N>
constexpr T const& front (T const (&array)[N]) noexcept { return array[0]; }

template <class T, ::std::size_t N>
constexpr T& front (T (&array)[N]) noexcept { return array[0]; }

template <class Container>
constexpr auto back (Container const& container) -> decltype(
  container.back()
) { return container.back(); }

template <class Container>
constexpr auto back (Container& container) -> decltype(container.back()) {
  return container.back();
}

template <class T, ::std::size_t N>
constexpr T const& back (T const (&array)[N]) noexcept { return array[N - 1]; }

template <class T, ::std::size_t N>
constexpr T& back (T (&array)[N]) noexcept { return array[N - 1]; }

/* data access */
template <class Container>
constexpr auto data (Container const& container) noexcept -> decltype(
  container.data()
) { return container.data(); }

template <class Container>
constexpr auto data (Container& container) noexcept -> decltype(
  container.data()
) { return container.data(); }

template <class T, ::std::size_t N>
constexpr T const* data (T const (&array)[N]) noexcept { return array; }

template <class T, ::std::size_t N>
constexpr T* data (T (&array)[N]) noexcept { return array; }

/* iteration */
template <class Container>
auto cbegin (Container const& container) -> decltype(::std::begin(container)) {
  return ::std::begin(container);
}

template <class Container>
auto cend (Container const& container) -> decltype(::std::end(container)) {
  return ::std::end(container);
}

template <class Container>
auto rbegin (Container const& container) -> decltype(container.rbegin()) {
  return container.rbegin();
}

template <class Container>
auto rbegin (Container& container) -> decltype(container.rbegin()) {
  return container.rbegin();
}

template <class Container>
auto crbegin (Container const& container) -> decltype(rbegin(container)) {
  return rbegin(container);
}

template <class Container>
auto rend (Container const& container) -> decltype(container.rend()) {
  return container.rend();
}

template <class Container>
auto rend (Container& container) -> decltype(container.rend()) {
  return container.rend();
}

template <class Container>
auto crend (Container const& container) -> decltype(rend(container)) {
  return rend(container);
}

template <
  class T,
  class CharT=char,
  class Traits=::std::char_traits<CharT>
> struct infix_ostream_iterator final : ::std::iterator<
  ::std::output_iterator_tag,
  void,
  void,
  void,
  void
> {
  using ostream_type = ::std::basic_ostream<CharT, Traits>;
  using traits_type = Traits;
  using char_type = CharT;

  infix_ostream_iterator (ostream_type& os) :
    infix_ostream_iterator { os, nullptr }
  { }

  infix_ostream_iterator (ostream_type& os, char_type const* delimiter) :
    os { os },
    delimiter { delimiter },
    first { true }
  { }

  infix_ostream_iterator& operator = (T const& item) {
    if (not first and delimiter) { this->os.get() << delimiter; }
    os.get() << item;
    this->first = false;
    return *this;
  }

  infix_ostream_iterator& operator ++ (int) { return *this; }
  infix_ostream_iterator& operator ++ () { return *this; }
  infix_ostream_iterator& operator * () { return *this; }

private:
  ::std::reference_wrapper<ostream_type> os;
  char_type const* delimiter;
  bool first;

};

}} /* namespace core::v1 */

#endif /* CORE_ITERATOR_HPP */
