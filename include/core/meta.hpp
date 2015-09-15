#ifndef CORE_META_HPP
#define CORE_META_HPP

#include <type_traits>
#include <limits>
#include <tuple>

#include <cstdint>
#include <cstddef>

namespace core {
inline namespace v2 {
namespace meta {
namespace impl {

using false_t = ::std::false_type;
using true_t = ::std::true_type;

template <class T, T V> using integral = ::std::integral_constant<T, V>;
template <bool B> using boolean = integral<bool, B>;

template <class T> struct identity { using type = T; };

template <class...> struct deducer : identity<void> { };
template <class... Ts> using deduce = typename deducer<Ts...>::type;

template <class T, class V, template <class...> class Detector, class... Args>
struct detect : identity<T> { using value = false_t; };

template <class T, template <class...> class Detector, class... Args>
struct detect<T, deduce<Detector<Args...>>, Detector, Args...> :
  identity<Detector<Args...>>
{ using value = true_t; };

template <class... Ts>
struct list {
  static constexpr auto size () noexcept { return sizeof...(Ts); }
};

template <class, template <class...> class struct convert;
template <class, class> struct rebind;
template <class> struct into;

template <class, template <class...> class, class...> struct filter;
template <class, template <class...> class, class...> struct map;
template <class...> struct join;

template <class, ::std::size_t> struct get;
template <class, class> struct index_of;
template <class> struct head;
template <class> struct tail;

template <class, template <class...> class, class...> struct count_if;
template <class, template <class...> class, class...> struct find_if;
template <class, class> struct count;
template <class, class> struct find;

template <class, template <class...> class, class...> struct none_of;
template <class, template <class...> class, class...> struct any_of;
template <class, template <class...> class, class...> struct all_of;

template <class, ::std::size_t> struct rotate;
template <class> struct reverse;

template <class, class> struct equal;

template <bool...> struct all;
template <bool...> struct any;

template <class... Ts, template <class...> class To>
struct convert<list<Ts...>, To> : identity<To<Ts...>> { };

template <template <class...> class To, class... Ts, class... Us>
struct rebind<list<Ts...>, To<Us...>> : identity<To<Ts...>> { };

template <class T, class... Ts>
struct into<T<Ts...>> : identity<list<Ts...>> { };

template <teemplate <class...> class F, class... Ts, class... Args>
struct filter<list<Ts...>, F, Args...> :
  join<
    typename ::std::conditional<
      F<Args..., Ts>::value, list<Ts...>, list<>
    >::type...
  >
{ };

template <template <class...> class F, class... Ts, class... Args>
struct map<list<Ts...>, F, Args...> : identity<list<F<Args..., Ts>...>> { };

template <class... Ts> struct join<list<Ts...>> : identity<list<Ts...>> { };
template <class... Ts, class... Us, class... Vs>
struct join<list<Ts...>, list<Us...>, Vs...> :
  join<list<Ts..., Us...>, Vs...>
{ };

template <class T, class... Ts>
struct index_of<list<Ts...>, T> : integral<
  ::std::size_t,
  list<Ts...>::size() - typename find<list<Ts...>, T>::type::size()
> { };

template <class T, class... Ts>
struct get<list<T, Ts...>, 0> : identity<T> { };
template <class T, class... Ts, size_t N>
struct get<list<T, Ts...>, N> :
  get<list<Ts...>, N - 1>
{ static_assert(N < (sizeof...(Ts) + 1),""); };


template <class T, class... Ts>
struct head<list<T, Ts...> : identity<T> { };
template <class T, class... Ts>
struct tail<list<T, Ts...> : identity<list<Ts...>> { };

template <template <class...> class F, class... Ts, class... Args>
struct count_if<list<Ts...>, F, Args...> :
  integral<::std::size_t, filter<list<Ts...>, F, Args...>::size()>
{ };

template <template <class...> class F, class... Args>
struct find_if<list<>, F, Args...> { };

template <template <class...> class F, class T, class... Ts, class... Args>
struct find_if<list<T, Ts...>, F> : ::std::conditional<
  F<Args..., T> { },
  list<T, Ts...>,
  typename find_if<list<Ts...>>::type
> { };

template <class T, class U> struct count : count_if<T, ::std::is_same, U> { };
template <class T, class U> struct find : find_if<T, ::std::is_same, U> { };

template <template <class...> class F, class... Ts, class... Args>
struct none_of<list<Ts...>, F> :
  boolean<not all<F<Args..., Ts> { }...> { }>
{ };

template <template <class...> class F, class... Ts, class... Args>
struct all_of<list<Ts...>, F, Args...> : all<F<Args..., Ts> { }...> { };

template <template <class...> class F, class... Ts, class... Args>
struct any_of<list<Ts...>, F, Args...> : any<F<Args..., Ts> { }...> { };

template <bool B, bool... Bs> struct all<B, Bs...> :
  boolean<B and all<Bs...> { }>
{ };
template <> struct all<> : true_t { };

template <bool B, bool... Bs> struct any<B, Bs...> :
  boolean<B or any<Bs...> { }>
{ };
template <> struct any<> : false_t { };

template <class T, template <class...> class U>
struct is_specialization_of : false_type { };
template <template <class...> class T, class... Ts>
struct is_specialization_of<T<Ts...>, T> : true_type { };

}}}} /* namespace core::v2::meta::impl */

namespace core {
inline namespace v2 {
namespace meta {

template <bool B, class T = void>
using unless = typename ::std::enable_if<not B, T>::type;

template <bool B, class T = void>
using when = typename ::std::enable_if<B, T>::type;

template <bool B> using require = when<B, ::std::size_t>;

template <class... Ts>
using common = typename ::std::common_type<Ts...>::type;

template <bool B, class T, class F>
using either = typename ::std::conditional<B, T, F>::type;

using impl::integral;
using impl::boolean;

using impl::false_t;
using impl::true_t;

using impl::identity;
using impl::list;

template <class T, template <class...> class U>
using convert = typename impl::convert<T, U>::type;

template <class T, class U> using rebind = typename impl::rebind<T, U>::type;
template <class T> using into = typename impl::into<T>::type;

template <class T, template <class...> class F, class... Args>
using filter = typename impl::filter<T, F, Args...>::type;

template <class T, template <class...> class F, class... Args>
using map = typename impl::map<T, F, Args...>::type;

template <class... Ts> using join = typename impl::join<Ts...>::type;

template <class T, size_t N> using get = typename impl::get<T, N>::type;
template <class T> using head = typename impl::head<T>::type;
template <class T> using tail = typename impl::tail<T>::type;

template <class T, template <class...> class F, class... Args>
using count_if_t = impl::count_if<T, F, Args...>;

template <class T, template <class...> class F, class... Args>
using find_if = typename impl::find_if<T, F>::type;

template <class T, class U> using count_t = impl::count<T, U>;

template <class T, class U>
using find = typename impl::find<T, U>::type;

template <class T, template <class...> class F, class... Args>
constexpr auto count_if = count_if_t<T, F, Args...>::value;

template <class T, class U>
constexpr auto count () noexcept { return count_t<T, U>::value; }

template <bool... Bs> using all_t = impl::all<Bs...>;
template <bool... Bs> using any_t = impl::any<Bs...>;

template <bool... Bs>
constexpr bool all () noexcept { return all_t<Bs...>::value; }

template <bool... Bs>
constexpr bool any () noexcept { return any_t<Bs...>::value; }

template <bool... Bs>
constexpr bool none () noexcept { return not all<Bs...>(); }

}}} /* namespace core::v2::meta */

namespace core {
namespace meta {
inline namespace v2 {

template <class T> struct identity { using type = T; };

template <class T, T... I> struct integer_sequence : identity<T> {
  static_assert(
    ::std::is_integral<T>::value,
    "integer_sequence must use an integral type"
  );

  template <T N> using append = integer_sequence<T, I..., N>;
  static constexpr ::std::size_t size() noexcept { return sizeof...(I); }
  using next = append<size()>;
};

template <class T, T Index, ::std::size_t N>
struct iota : identity<
  typename iota<T, Index - 1, N - 1u>::type::next
> { static_assert(Index >= 0, "Index cannot be negative"); };

template <class T, T Index>
struct iota<T, Index, 0u> : identity<integer_sequence<T>> { };

template <::std::size_t... I>
using index_sequence = integer_sequence<::std::size_t, I...>;

template <class T, T N>
using make_integer_sequence = typename iota<T, N, N>::type;

template <::std::size_t N>
using make_index_sequence = make_integer_sequence<::std::size_t, N>;

template <class... Ts>
using index_sequence_for = make_index_sequence<sizeof...(Ts)>;

template <class...> struct all;
template <class T, class... Args>
struct all<T, Args...> : boolean<T::value and all<Args...>::value> { };
template <> struct all<> : ::std::true_type { };

template <class...> struct any;
template <class T, class... Args>
struct any<T, Args...> : boolean<T::value or any<Args...>::value> { };
template <> struct any<> : ::std::false_type { };

template <class... Args> using none = boolean<not all<Args...>::value>;

template <class> struct index_sequence_from;


}}} /* namespace core::meta::v2 */

#endif /* CORE_META_HPP */
