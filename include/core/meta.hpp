#ifndef CORE_META_HPP
#define CORE_META_HPP

namespace core {
namespace meta {
inline namespace v1 {

template <class...> struct all;
template <class T, class... Args>
struct all<T, Args...> : ::std::integral_constant<bool,
  T::value and all<Args...>::value
> { };
template <> struct all<> : ::std::true_type { };

template <class...> struct any;
template <class T, class... Args>
struct any<T, Args...> : ::std::integral_constant<bool,
  T::value or any<Args...>::value
> { };
template <> struct any<> : ::std::false_type { };

template <class... Args>
using none = ::std::integral_constant<bool,
  not all<Args...>::value
>;

}}} /* namespace core::meta::v1 */

#endif /* CORE_META_HPP */
