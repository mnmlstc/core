#ifndef CORE_FUNCTIONAL_HPP
#define CORE_FUNCTIONAL_HPP

#include <core/type_traits.hpp>

namespace core {
inline namespace v1 {
namespace impl {

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

template <class Functor, class Object, class... Args>
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
constexpr auto invoke(Functor&& functor, Args&&... args) -> enable_if_t<
  invokable<Functor, Args...>::value,
  decltype(std::forward<Functor>(functor)(std::forward<Args>(args)...))
> { return std::forward<Functor>(functor)(std::forward<Args>(args)...); }

}} /* namespace core::v1 */

//auto result = core::invoke(core::unpack

#endif /* CORE_FUNCTIONAL_HPP */
