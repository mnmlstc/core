#ifndef CORE_FUNCTIONAL_HPP
#define CORE_FUNCTIONAL_HPP

#include <core/type_traits.hpp>
#include <core/utility.hpp>
#include <functional>
#include <tuple>
#include <array>

namespace core {
inline namespace v1 {

template <class F> struct function_traits;

template <class R, class... Args>
struct function_traits<R(*)(Args...)> : function_traits<R(Args...)> { };

template <class C, class R>
struct function_traits<R(C::*)> : function_traits<R(C&)> { };

template <class C, class R, class... Args>
struct function_traits<R(C::*)(Args...)> : function_traits<R(C&, Args...)> { };

template <class C, class R, class... Args>
struct function_traits<R(C::*)(Args...) const volatile> :
  function_traits<R(C volatile const&, Args...)>
{ };

template <class C, class R, class... Args>
struct function_traits<R(C::*)(Args...) volatile> :
  function_traits<R(C volatile&, Args...)>
{ };

template <class C, class R, class... Args>
struct function_traits<R(C::*)(Args...) const> :
  function_traits<R(C const&, Args...)>
{ };

template <class R, class... Args>
struct function_traits<R(Args...)> {
  using return_type = R;

  using pointer = return_type(*)(Args...);
  static constexpr ::std::size_t arity = sizeof...(Args);

  template < ::std::size_t N>
  using argument = typename ::std::tuple_element<
    N,
    ::std::tuple<Args...>
  >::type;
};

template <class F> struct function_traits {
  using functor_type = function_traits<decltype(&decay_t<F>::operator())>;
  using return_type = typename functor_type::return_type;
  using pointer = typename functor_type::pointer;
  static constexpr ::std::size_t arity = functor_type::arity - 1;
  template <::std::size_t N>
  using argument = typename functor_type::template argument<N>;
};

/* N3727 */
template <class Functor, class Object, class... Args>
constexpr auto invoke (Functor&& functor, Object&& object, Args&&... args) -> enable_if_t<
  invokable<Functor, Object, Args...>::value,
  decltype((object.*functor)(::core::forward<Args>(args)...))
> { return (object.*functor)(::core::forward<Args>(args)...); }

template <class Functor, class Object, class... Args>
constexpr auto invoke (Functor&& functor, Object&& object, Args&&... args) -> enable_if_t<
  invokable<Functor, Object, Args...>::value,
  decltype(
    ((*::core::forward<Object>(object)).*functor)(::core::forward<Args>(args)...)
  )
> {
  return (
    (*::core::forward<Object>(object)).*functor
  )(::core::forward<Args>(args)...);
}

template <class Functor, class Object>
constexpr auto invoke (Functor&& functor, Object&& object) -> enable_if_t<
  invokable<Functor, Object>::value,
  decltype(object.*functor)
> { return object.*functor; }

template <class Functor, class Object>
constexpr auto invoke (Functor&& functor, Object&& object) -> enable_if_t<
  invokable<Functor, Object>::value,
  decltype((*::core::forward<Object>(object)).*functor)
> { return (*::core::forward<Object>(object)).*functor; }

template <class Functor, class... Args>
constexpr auto invoke (Functor&& functor, Args&&... args) -> enable_if_t<
  invokable<Functor, Args...>::value,
  decltype(::core::forward<Functor>(functor)(::core::forward<Args>(args)...))
> { return ::core::forward<Functor>(functor)(::core::forward<Args>(args)...); }

namespace impl {

/* Used to provide lambda based 'pattern matching' for variant and optional
 * types.
 *
 * Based off of Dave Abrahams C++11 'generic lambda' example.
 */
template <class... Lambdas> struct overload;
template <class Lambda> struct overload<Lambda> : Lambda {
  using call_type = Lambda;
  using call_type::operator ();
};

template <class Lambda, class... Lambdas>
struct overload<Lambda, Lambdas...> :
  private Lambda,
  private overload<Lambdas...>::call_type
{
  using base_type = typename overload<Lambdas...>::call_type;

  using lambda_type = Lambda;
  using call_type = overload;

  overload (Lambda&& lambda, Lambdas&&... lambdas) :
    lambda_type(::core::forward<Lambda>(lambda)),
    base_type(::core::forward<Lambdas>(lambdas)...)
  { }

  using lambda_type::operator ();
  using base_type::operator ();
};

template <class... Lambdas>
auto make_overload(Lambdas&&... lambdas) -> overload<Lambdas...> {
  return overload<Lambdas...> { ::core::forward<Lambdas>(lambdas)... };
}

template <class Functor, class U, ::std::size_t... I>
auto unpack (
  Functor&& functor,
  U&& unpackable,
  index_sequence<I...>&&
) -> invoke_of_t<
  Functor,
  decltype(::std::get<I>(::std::forward<U>(unpackable)))...
> {
  return ::core::v1::invoke(::std::forward<Functor>(functor),
    ::std::get<I>(::std::forward<U>(unpackable))...
  );
}

template <class U, ::std::size_t... I>
auto unpack (U&& unpackable, index_sequence<I...>&&) -> invoke_of_t<
  decltype(::std::get<I>(::std::forward<U>(unpackable)))...
> {
  return ::core::v1::invoke(::std::get<I>(::std::forward<U>(unpackable))...);
}

template <class Functor, class U, ::std::size_t... I>
auto runpack (
  Functor&& functor,
  U&& runpackable,
  index_sequence<I...>&&
) -> invoke_of_t<Functor, decltype(::std::forward<U>(runpackable).at(I))...> {
  return ::core::v1::invoke(
    ::std::forward<Functor>(functor),
    ::std::forward<U>(runpackable).at(I)...);
}

} /* namespace impl */

struct unpack_t final { };
constexpr unpack_t unpack { };

struct runpack_t final { };
constexpr runpack_t runpack { };

template <class Functor, class Unpackable>
auto invoke (unpack_t, Functor&& functor, Unpackable&& unpackable) ->
enable_if_t<
  is_unpackable<decay_t<Unpackable>>::value,
  decltype(
    impl::unpack(
      ::std::forward<Functor>(functor),
      ::std::forward<Unpackable>(unpackable),
      make_index_sequence<::std::tuple_size<decay_t<Unpackable>>::value> { }
    )
  )
> {
  return impl::unpack(
    ::std::forward<Functor>(functor),
    ::std::forward<Unpackable>(unpackable),
    make_index_sequence<::std::tuple_size<decay_t<Unpackable>>::value> { }
  );
}

template <class Unpackable>
auto invoke (unpack_t, Unpackable&& unpackable) ->
enable_if_t<
  is_unpackable<decay_t<Unpackable>>::value,
  decltype(
    impl::unpack(
      ::std::forward<Unpackable>(unpackable),
      make_index_sequence<::std::tuple_size<decay_t<Unpackable>>::value> { }
    )
  )
> {
  return impl::unpack(
    ::std::forward<Unpackable>(unpackable),
    make_index_sequence<::std::tuple_size<decay_t<Unpackable>>::value> { }
  );
}

/* Modified to force clang to *not* select this function in a bizarre corner
 * case.
 */
template <
  class Functor,
  class Runpackable,
  class=enable_if_t<is_runpackable<decay_t<Runpackable>>::value>
> auto invoke (
  runpack_t,
  Functor&& functor,
  Runpackable&& unpackable
) -> decltype(
    impl::runpack(
      ::std::forward<Functor>(functor),
      ::std::forward<Runpackable>(unpackable),
      make_index_sequence<function_traits<Functor>::arity> { }
    )
  ) {
  return impl::runpack(
    ::std::forward<Functor>(functor),
    ::std::forward<Runpackable>(unpackable),
    make_index_sequence<function_traits<Functor>::arity> { }
  );
}

template <class Functor, class T>
auto apply (Functor&& f, T&& t) -> decltype(
  invoke(unpack, ::std::forward<Functor>(f), ::std::forward<T>(t))
) { return invoke(unpack, ::std::forward<Functor>(f), ::std::forward<T>(t)); }

/* function objects -- arithmetic */
template <class T=void>
struct plus {
  using second_argument_type = T;
  using first_argument_type = T;
  using result_type = T;
  constexpr T operator () (T const& l, T const& r) const { return l + r; }
};

template <class T=void>
struct minus {
  using second_argument_type = T;
  using first_argument_type = T;
  using result_type = T;

  constexpr T operator () (T const& l, T const& r) const { return l - r; }
};

template <class T=void>
struct multiplies {
  using second_argument_type = T;
  using first_argument_type = T;
  using result_type = T;

  constexpr T operator () (T const& l, T const& r) const { return l * r; }
};

template <class T=void>
struct divides {
  using second_argument_type = T;
  using first_argument_type = T;
  using result_type = T;

  constexpr T operator () (T const& l, T const& r) const { return l / r; }
};

template <class T=void>
struct modulus {
  using second_argument_type = T;
  using first_argument_type = T;
  using result_type = T;

  constexpr T operator () (T const& l, T const& r) const { return l % r; }
};

template <class T=void>
struct negate {
  using argument_type = T;
  using result_type = T;

  constexpr T operator () (T const& arg) const { return -arg; }
};

/* function objects -- comparisons */
template <class T=void>
struct equal_to {
  using second_argument_type = T;
  using first_argument_type = T;
  using result_type = bool;

  constexpr bool operator () (T const& l, T const& r) const { return l == r; }
};

template <class T=void>
struct not_equal_to {
  using second_argument_type = T;
  using first_argument_type = T;
  using result_type = bool;

  constexpr bool operator () (T const& l, T const& r) const { return l != r; }
};

template <class T=void>
struct greater_equal {
  using second_argument_type = T;
  using first_argument_type = T;
  using result_type = bool;

  constexpr bool operator () (T const& l, T const& r) const { return l >= r; }
};

template <class T=void>
struct less_equal {
  using second_argument_type = T;
  using first_argument_type = T;
  using result_type = bool;

  constexpr bool operator () (T const& l, T const& r) const { return l <= r; }
};

template <class T=void>
struct greater {
  using second_argument_type = T;
  using first_argument_type = T;
  using result_type = bool;

  constexpr bool operator () (T const& l, T const& r) const { return l > r; }
};

template <class T=void>
struct less {
  using second_argument_type = T;
  using first_argument_type = T;
  using result_type = bool;

  constexpr bool operator () (T const& l, T const& r) const { return l < r; }
};

/* function objects -- logical */
template <class T=void>
struct logical_and {
  using second_argument_type = T;
  using first_argument_type = T;
  using result_type = bool;

  constexpr bool operator () (T const& l, T const& r) const { return l and r; }
};

template <class T=void>
struct logical_or {
  using second_argument_type = T;
  using first_argument_type = T;
  using result_type = bool;

  constexpr bool operator () (T const& l, T const& r) const { return l or r; }
};

template <class T=void>
struct logical_not {
  using argument_type = T;
  using result_type = bool;

  constexpr bool operator () (T const& arg) const { return not arg; }
};

/* function objects -- bitwise */

template <class T=void>
struct bit_and {
  using second_argument_type = T;
  using first_argument_type = T;
  using result_type = T;

  constexpr bool operator () (T const& l, T const& r) const { return l & r; }
};

template <class T=void>
struct bit_or {
  using second_argument_type = T;
  using first_argument_type = T;
  using result_type = T;

  constexpr bool operator () (T const& l, T const& r) const { return l | r; }
};

template <class T=void>
struct bit_xor {
  using second_argument_type = T;
  using first_argument_type = T;
  using result_type = T;

  constexpr bool operator () (T const& l, T const& r) const { return l ^ r; }
};

template <class T=void>
struct bit_not {
  using argument_type = T;
  using result_type = T;

  constexpr bool operator () (T const& arg) const { return ~arg; }
};

/* function objects -- arithmetic specializations */
template <> struct plus<void> {
  using is_transparent = void;

  template <class T, class U>
  constexpr auto operator () (T&& t, U&& u) const -> decltype(
    forward<T>(t) + forward<U>(u)
  ) { return forward<T>(t) + forward<U>(u); }
};

template <> struct minus<void> {
  using is_transparent = void;

  template <class T, class U>
  constexpr auto operator () (T&& t, U&& u) const -> decltype(
    forward<T>(t) - forward<U>(u)
  ) { return forward<T>(t) - forward<U>(u); }
};

template <> struct multiplies<void> {
  using is_transparent = void;

  template <class T, class U>
  constexpr auto operator () (T&& t, U&& u) const -> decltype(
    forward<T>(t) * forward<U>(u)
  ) { return forward<T>(t) * forward<U>(u); }
};

template <> struct divides<void> {
  using is_transparent = void;

  template <class T, class U>
  constexpr auto operator () (T&& t, U&& u) const -> decltype(
    forward<T>(t) / forward<U>(u)
  ) { return forward<T>(t) / forward<U>(u); }
};

template <> struct modulus<void> {
  using is_transparent = void;

  template <class T, class U>
  constexpr auto operator () (T&& t, U&& u) const -> decltype(
    forward<T>(t) % forward<U>(u)
  ) { return forward<T>(t) % forward<U>(u); }
};

template <> struct negate<void> {
  using is_transparent = void;

  template <class T>
  constexpr auto operator () (T&& t) const -> decltype(forward<T>(t)) {
    return forward<T>(t);
  }
};

/* function objects -- comparison specialization */
template <> struct equal_to<void> {
  using is_transparent = void;

  template <class T, class U>
  constexpr auto operator () (T&& t, U&& u) const -> decltype(
    forward<T>(t) == forward<U>(u)
  ) { return forward<T>(t) == forward<U>(u); }
};

template <> struct not_equal_to<void> {
  using is_transparent = void;

  template <class T, class U>
  constexpr auto operator () (T&& t, U&& u) const -> decltype(
    forward<T>(t) != forward<U>(u)
  ) { return forward<T>(t) != forward<U>(u); }
};

template <> struct greater_equal<void> {
  using is_transparent = void;

  template <class T, class U>
  constexpr auto operator () (T&& t, U&& u) const -> decltype(
    forward<T>(t) >= forward<U>(u)
  ) { return forward<T>(t) >= forward<U>(u); }
};

template <> struct less_equal<void> {
  using is_transparent = void;

  template <class T, class U>
  constexpr auto operator () (T&& t, U&& u) const -> decltype(
    forward<T>(t) <= forward<U>(u)
  ) { return forward<T>(t) <= forward<U>(u); }
};

template <> struct greater<void> {
  using is_transparent = void;

  template <class T, class U>
  constexpr auto operator () (T&& t, U&& u) const -> decltype(
    forward<T>(t) > forward<U>(u)
  ) { return forward<T>(t) > forward<U>(u); }
};

template <> struct less<void> {
  using is_transparent = void;

  template <class T, class U>
  constexpr auto operator () (T&& t, U&& u) const -> decltype(
    forward<T>(t) < forward<U>(u)
  ) { return forward<T>(t) < forward<U>(u); }
};

/* function objects -- logical specializations */
template <> struct logical_and<void> {
  using is_transparent = void;

  template <class T, class U>
  constexpr auto operator () (T&& t, U&& u) const -> decltype(
    forward<T>(t) and forward<U>(u)
  ) { return forward<T>(t) and forward<U>(u); }
};

template <> struct logical_or<void> {
  using is_transparent = void;

  template <class T, class U>
  constexpr auto operator () (T&& t, U&& u) const -> decltype(
    forward<T>(t) or forward<U>(u)
  ) { return forward<T>(t) or forward<U>(u); }
};

template <> struct logical_not<void> {
  using is_transparent = void;

  template <class T>
  constexpr auto operator () (T&& t) const -> decltype(not forward<T>(t)) {
    return not forward<T>(t);
  }
};

/* function objects -- bitwise specializations */
template <> struct bit_and<void> {
  using is_transparent = void;

  template <class T, class U>
  constexpr auto operator () (T&& t, U&& u) const -> decltype(
    forward<T>(t) & forward<U>(u)
  ) { return forward<T>(t) & forward<U>(u); }
};

template <> struct bit_or<void> {
  using is_transparent = void;

  template <class T, class U>
  constexpr auto operator () (T&& t, U&& u) const -> decltype(
    forward<T>(t) | forward<U>(u)
  ) { return forward<T>(t) | forward<U>(u); }
};

template <> struct bit_xor<void> {
  using is_transparent = void;

  template <class T, class U>
  constexpr auto operator () (T&& t, U&& u) const -> decltype(
    forward<T>(t) ^ forward<U>(u)
  ) { return forward<T>(t) ^ forward<U>(u); }
};

template <> struct bit_not<void> {
  using is_transparent = void;

  template <class T, class U>
  constexpr auto operator () (T&& t) const -> decltype(~forward<T>(t)) {
    return ~forward<T>(t);
  }
};

}} /* namespace core::v1 */

#endif /* CORE_FUNCTIONAL_HPP */
