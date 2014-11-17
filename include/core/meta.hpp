#ifndef CORE_META_HPP
#define CORE_META_HPP

#include <type_traits>
#include <cstdint>
#include <cstddef>

namespace core {
namespace meta {
inline namespace v1 {

template <::std::size_t I>
using size = ::std::integral_constant<::std::size_t, I>;

template <bool B> using boolean = ::std::integral_constant<bool, B>;
template <int I> using integer = ::std::integral_constant<int, I>;

template <class...> struct all;
template <class T, class... Args>
struct all<T, Args...> : boolean<T::value and all<Args...>::value> { };
template <> struct all<> : ::std::true_type { };

template <class...> struct any;
template <class T, class... Args>
struct any<T, Args...> : boolean<T::value or any<Args...>::value> { };
template <> struct any<> : ::std::false_type { };

template <class... Args> using none = boolean<not all<Args...>::value>;

}}} /* namespace core::meta::v1 */

#endif /* CORE_META_HPP */
