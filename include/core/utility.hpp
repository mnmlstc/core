#ifndef CORE_UTILITY_HPP
#define CORE_UTILITY_HPP

#include <type_traits>
#include <utility>

#include <cstddef>

namespace core {
inline namespace v1 {
namespace impl {

template <class T, T... I> struct integer_sequence {
  static_assert(
    std::is_integral<T>::value,
    "integer_sequence must use an integral type"
  );

  template <T N> using append = integer_sequence<T, I..., N>;
  static constexpr std::size_t size () noexcept { return sizeof...(I); }
  using next = append<size()>;
  using type = T;
};

template <class T, T Index, std::size_t N>
struct sequence_generator {
  static_assert(Index >= 0, "Index cannot be negative");
  using type = typename sequence_generator<T, Index - 1, N - 1>::type::next;
};

template <class T, T Index>
struct sequence_generator<T, Index, 0ul> { using type = integer_sequence<T>; };

template <std::size_t Index, class T, class U, class... Types>
struct typelist_index {
  using type = typename typelist_index<Index + 1, T, Types...>::type;
  static constexpr std::size_t value = Index;
};

template <std::size_t Index, class T, class... Types>
struct typelist_index<Index, T, T, Types...> {
  using type = typelist_index;
  static constexpr std::size_t value = Index;
};

} /* namespace impl */

template <class T, T... I>
using integer_sequence = impl::integer_sequence<T, I...>;

template <std::size_t... I>
using index_sequence = integer_sequence<std::size_t, I...>;

template <class T, T N>
using make_integer_sequence = typename impl::sequence_generator<T, N, N>::type;

template <std::size_t N>
using make_index_sequence = make_integer_sequence<std::size_t, N>;

/* Allows us to properly get values from a constexpr
 * std::pair at compile time. Follows the new get<type> convention
 */
template <class T, class U>
constexpr auto get (std::pair<T, U> const& pair) noexcept -> T const& {
  return pair.first;
}

template <class T, class U>
constexpr auto get (std::pair<U, T> const& pair) noexcept -> T const& {
  return pair.second;
}

template <class T, class U>
constexpr auto get (std::pair<T, U>&& pair) noexcept -> T&& {
  return std::move(pair.first);
}

template <class T, class U>
constexpr auto get (std::pair<U, T>&& pair) noexcept -> T&& {
  return std::move(pair.second);
}

template <class T, class U>
constexpr auto get (std::pair<T, U>& pair) noexcept -> T& {
  return pair.first;
}

template <class T, class U>
constexpr auto get (std::pair<U, T>& pair) noexcept -> T& {
  return pair.second;
}

}} /* namespace core::v1 */

#endif /* CORE_UTILITY_HPP */
