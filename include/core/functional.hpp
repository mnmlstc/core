#ifndef CORE_FUNCTIONAL_HPP
#define CORE_FUNCTIONAL_HPP

#include <core/type_traits.hpp>
#include <core/utility.hpp>
#include <tuple>
#include <array>

namespace core {
inline namespace v1 {

template <class Functor, class Object, class... Args>
constexpr auto invoke (Functor&& functor, Object&& object, Args&&... args) ->
enable_if_t<
  invokable<Functor, Object, Args...>::value,
  decltype((object.*functor)(std::forward<Args>(args)...))
> { return (object.*functor)(std::forward<Args>(args)...); }

template <class Functor, class Object, class... Args>
constexpr auto invoke (Functor&& functor, Object&& object, Args&&... args) ->
enable_if_t<
  invokable<Functor, Object, Args...>::value,
  decltype(
    ((*std::forward<Object>(object)).*functor)(std::forward<Args>(args)...)
  )
> {
  return (
    (*std::forward<Object>(object)).*functor
  )(std::forward<Args>(args)...);
}

template <class Functor, class Object>
constexpr auto invoke (Functor&& functor, Object&& object) -> enable_if_t<
  invokable<Functor, Object>::value,
  decltype(object.*functor)
> { return object.*functor; }

template <class Functor, class Object>
constexpr auto invoke (Functor&& functor, Object&& object) -> enable_if_t<
  invokable<Functor, Object>::value,
  decltype((*std::forward<Object>(object)).*functor)
> { return (*std::forward<Object>(object)).*functor; }

template <class Functor, class... Args>
constexpr auto invoke (Functor&& functor, Args&&... args) -> invoke_of_t<
  Functor, Args...
> { return std::forward<Functor>(functor)(std::forward<Args>(args)...); }

namespace impl {

template <class Functor, class U, std::size_t... I>
auto unpack (Functor&& functor, U&& unpackable, index_sequence<I...>&&) ->
invoke_of_t<Functor, decltype(std::get<I>(std::forward<U>(unpackable)))...> {
  return ::core::v1::invoke(std::forward<Functor>(functor),
    std::get<I>(std::forward<U>(unpackable))...
  );
}

template <class U, std::size_t... I>
auto unpack (U&& unpackable, index_sequence<I...>&&) -> invoke_of_t<
  decltype(std::get<I>(std::forward<U>(unpackable)))...
> { return ::core::v1::invoke(std::get<I>(std::forward<U>(unpackable))...); }

} /* namespace impl */

struct unpack_t final { };
constexpr unpack_t unpack { };

template <class Functor, class Unpackable>
constexpr auto invoke (unpack_t, Functor&& functor, Unpackable&& unpackable) ->
enable_if_t<
  is_unpackable<decay_t<Unpackable>>::value,
  decltype(
    impl::unpack(
      std::forward<Functor>(functor),
      std::forward<Unpackable>(unpackable),
      make_index_sequence<std::tuple_size<decay_t<Unpackable>>::value> { }
    )
  )
> {
  return impl::unpack(
    std::forward<Functor>(functor),
    std::forward<Unpackable>(unpackable),
    make_index_sequence<std::tuple_size<decay_t<Unpackable>>::value> { }
  );
}

template <class Unpackable>
constexpr auto invoke (unpack_t, Unpackable&& unpackable) ->
enable_if_t<
  is_unpackable<decay_t<Unpackable>>::value,
  decltype(
    impl::unpack(
      std::forward<Unpackable>(unpackable),
      make_index_sequence<std::tuple_size<decay_t<Unpackable>>::value> { }
    )
  )
> {
  return impl::unpack(
    std::forward<Unpackable>(unpackable),
    make_index_sequence<std::tuple_size<decay_t<Unpackable>>::value> { }
  );
}


}} /* namespace core::v1 */

#endif /* CORE_FUNCTIONAL_HPP */
