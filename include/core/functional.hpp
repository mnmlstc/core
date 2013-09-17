#ifndef CORE_FUNCTIONAL_HPP
#define CORE_FUNCTIONAL_HPP

#include <core/type_traits.hpp>
#include <core/utility.hpp>
#include <tuple>

namespace core {
inline namespace v1 {
namespace impl {

template <class Functor, class Tuple, std::size_t... I>
auto unpack (Functor&& functor, Tuple&& tuple, index_sequence<I...>&&) ->
invoke_of_t<Functor, decltype(std::get<I>(std::forward<Tuple>(tuple)))...> {
  return std::forward<Functor>(functor)(
    std::get<I>(std::forward<Tuple>(tuple))...
  );
}

} /* namespace impl */

struct unpack_t final { };
constexpr unpack_t unpack { };

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
constexpr auto invoke(Functor&& functor, Args&&... args) -> invoke_of_t<
  Functor, Args...
> { return std::forward<Functor>(functor)(std::forward<Args>(args)...); }

template <class Functor, class Tuple>
constexpr auto invoke(unpack_t, Functor&& functor, Tuple&& tuple) ->
enable_if_t<
  core::is_specialization_of<decay_t<Tuple>, std::tuple>::value,
  decltype(
    impl::unpack(
      std::forward<Functor>(functor),
      std::forward<Tuple>(tuple),
      make_index_sequence<std::tuple_size<decay_t<Tuple>>::value> { }
    )
  )
> {
  return impl::unpack(
    std::forward<Functor>(functor),
    std::forward<Tuple>(tuple),
    make_index_sequence<std::tuple_size<decay_t<Tuple>>::value> { }
  );
}

}} /* namespace core::v1 */

//auto result = core::invoke(core::unpack

#endif /* CORE_FUNCTIONAL_HPP */
