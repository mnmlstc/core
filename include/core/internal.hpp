#ifndef CORE_INTERNAL_HPP
#define CORE_INTERNAL_HPP

/* This is a header containing common implementation specific code, to
 * reduce the complexity of the other headers, especially those that are
 * closely intertwined, such as <core/functional.hpp> and <core/type_traits.hpp>
 *
 * Additionally, some of this code is duplicated elsewhere (such as class_of,
 * and meta::identity), but aliases are placed to lessen any impact that this might
 * have.
 */

#include <type_traits>
#include <functional>
#include <utility>

#include <core/meta.hpp>

namespace core {
inline namespace v1 {
namespace impl {

/* deduce_t workaround for gcc */
template <class...> struct deducer { using type = void; };

/* extremely useful custom type traits */
template <class T> struct class_of : meta::identity<T> { };
template <class Signature, class T>
struct class_of<Signature T::*> : meta::identity<T> { };

/* aliases */
template <class... Ts> using deduce_t = typename deducer<Ts...>::type;
template <class T> using class_of_t = typename class_of<T>::type;
template <class T> using decay_t = typename ::std::decay<T>::type;
template <class T>
using remove_reference_t = typename ::std::remove_reference<T>::type;
template <bool B, class T = void>
using enable_if_t = typename ::std::enable_if<B, T>::type;

/* is_nothrow_swappable plumbing */
using ::std::declval;
using ::std::swap;

template <class, class, class=void>
struct is_swappable : ::std::false_type { };

template <class T, class U>
struct is_swappable<
  T,
  U,
  deduce_t<
    decltype(swap(declval<T&>(), declval<U&>())),
    decltype(swap(declval<U&>(), declval<T&>()))
  >
> : ::std::true_type { };

template <class T, class U>
struct is_nothrow_swappable : meta::all<
  is_swappable<T, U>,
  meta::boolean<noexcept(swap(declval<T&>(), declval<U&>()))>,
  meta::boolean<noexcept(swap(declval<U&>(), declval<T&>()))>
> { };

/* used below but also for the functional objects defined later to cut down
 * on unnecessary code. Defined due to deprecation in C++11, and removal in
 * C++17. I remove the _function part of the name because it's an internal
 * API and never you mind.
 */
template <class T, class U, class R>
struct binary {
  using second_argument_type = U;
  using first_argument_type = T;
  using result_type = R;
};

template <class T, class R>
struct unary {
  using argument_type = T;
  using result_type = R;
};

/* constexpr mem_fn plumbing
 * names here are kept short because we *have* to give some of these types a
 * name, but the name doesn't really matter (nor are the attempts I've made
 * at giving them proper names truly descriptive). Comments are placed above
 * each one so that anyone reading won't wonder why the member function pointer
 * trait base is named the onomatopoeia for giving someone a raspberry :P and
 * so on and so forth.
 *
 * If I can't amuse myself when working with C++ templates, then life isn't
 * worth living. Bury me with my chevrons.
 */
template <class T>
constexpr T&& pass (remove_reference_t<T>& t) noexcept {
  return static_cast<T&&>(t);
}

template <class T>
constexpr T&& pass (remove_reference_t<T>&& t) noexcept {
  return static_cast<T&&>(t);
}

/* mfptb == member function pointer trait base
 * No, I don't feel like typing that out. I am mad that I have to do all
 * this in the first place.
 */
template <class R, class... Args> struct mfptb {
  constexpr mfptb () noexcept = default;
};
template <class R, class T, class U> struct mfptb<R, T, U> :
  binary<T, U, R>
{ };
template <class R, class T> struct mfptb<R, T> :
  unary<T, R>
{ };

template <class T> using ptr = typename ::std::add_pointer<T>::type;
template <class T> using cptr = ptr<typename ::std::add_const<T>::type>;
template <class T> using vptr = ptr<typename ::std::add_volatile<T>::type>;
template <class T> using cvptr = ptr<typename ::std::add_cv<T>::type>;

/* mfpt == member function pointer trait
 * we need a lot of overloads for this one
 */
template <class> struct mfpt;

template <class R, class C, class... Args>
struct mfpt<R(C::*)(Args...) volatile const> :
  mfptb<R, cvptr<C>, Args...>
{ };

template <class R, class C, class... Args>
struct mfpt<R(C::*)(Args...) volatile> :
  mfptb<R, vptr<C>, Args...>
{ };

template <class R, class C, class... Args>
struct mfpt<R(C::*)(Args...) const> :
  mfptb<R, cptr<C>, Args...>
{ };

template <class R, class C, class... Args>
struct mfpt<R(C::*)(Args...)> :
  mfptb<R, C, Args...>
{ };

template <class R, class C, class... Args>
struct mfpt<R(C::*)(Args...) volatile const &> :
  mfptb<R, cvptr<C>, Args...>
{ };

template <class R, class C, class... Args>
struct mfpt<R(C::*)(Args...) volatile &> :
  mfptb<R, vptr<C>, Args...>
{ };

template <class R, class C, class... Args>
struct mfpt<R(C::*)(Args...) const &> :
  mfptb<R, cptr<C>, Args...>
{ };

template <class R, class C, class... Args>
struct mfpt<R(C::*)(Args...) &> :
  mfptb<R, C, Args...>
{ };

template <class R, class C, class... Args>
struct mfpt<R(C::*)(Args...) volatile const &&> :
  mfptb<R, cvptr<C>, Args...>
{ };

template <class R, class C, class... Args>
struct mfpt<R(C::*)(Args...) volatile &&> :
  mfptb<R, vptr<C>, Args...>
{ };

template <class R, class C, class... Args>
struct mfpt<R(C::*)(Args...) const &&> :
  mfptb<R, cptr<C>, Args...>
{ };

template <class R, class C, class... Args>
struct mfpt<R(C::*)(Args...) &&> :
  mfptb<R, C, Args...>
{ };

/* type used for pointer to member function */
template <class T>
struct pmf final : mfpt<remove_reference_t<T>> {

  using member_type = T;
  using class_type = class_of_t<decay_t<member_type>>;

  template <class C> using reference = ::std::reference_wrapper<C>;
  template <class U>
  using is_derived = ::std::is_base_of<class_type, decay_t<U>>;

  template <class, class=void> struct has_object : ::std::false_type { };

  template <class U>
  struct has_object<U, deduce_t<decltype(*declval<U>())>> :
    ::std::is_base_of<class_type, decltype(*declval<U>())>
  { };

  constexpr pmf (member_type mp) noexcept : mp(mp) { }

  template <
    class Object,
    class... Args,
    class=enable_if_t<is_derived<Object>::value>
  > constexpr auto operator () (Object&& obj, Args&&... args) const noexcept(
    noexcept((pass<Object>(obj).*declval<member_type>())(pass<Args>(args)...))
  ) -> decltype((pass<Object>(obj).*declval<member_type>())(pass<Args>(args)...)) {
    return (pass<Object>(obj).*mp)(pass<Args>(args)...);
  }

  template <
    class Object,
    class... Args,
    class=enable_if_t<has_object<Object>::value>
  > constexpr auto operator () (Object&& obj, Args&&... args) const noexcept(
    noexcept(operator()(*pass<Object>(obj), pass<Args>(args)...))
  ) -> decltype(operator()(*pass<Object>(obj), pass<Args>(args)...)) {
    return operator()(*pass<Object>(obj), pass<Args>(args)...);
  }

  template <
    class Object,
    class... Args,
    class=enable_if_t<is_derived<Object>::value>
  > constexpr auto operator () (Object* ptr, Args&&... args) const noexcept(
    noexcept(operator ()(*ptr, pass<Args>(args)...))
  ) -> decltype(operator ()(*ptr, pass<Args>(args)...)) {
    return operator ()(*ptr, pass<Args>(args)...);
  }

  template <
    class Object,
    class... Args,
    class=enable_if_t<is_derived<Object>::value>
  > auto operator () (reference<Object> ref, Args&&... args) const noexcept(
    noexcept(operator ()(ref.get(), pass<Args>(args)...))
  ) -> decltype(operator ()(ref.get(), pass<Args>(args)...)) {
    return operator ()(ref.get(), pass<Args>(args)...);
  }

private:
  member_type mp;
};

/* type used for pointer to data member */
template <class T>
struct pdm final {
  using member_type = T;
  using class_type = class_of_t<decay_t<member_type>>;

  template <class C> using reference = ::std::reference_wrapper<C>;
  template <class U>
  using is_derived = ::std::is_base_of<class_type, decay_t<U>>;

  template <class, class=void> struct has_object : ::std::false_type { };

  template <class U>
  struct has_object<U, deduce_t<decltype(*declval<U>())>> :
    ::std::is_base_of<class_type, decltype(*declval<U>())>
  { };


  constexpr pdm (member_type mp) noexcept : mp(mp) { }

  template <
    class Object,
    class=enable_if_t<is_derived<Object>::value>
  > constexpr auto operator () (Object&& obj) const noexcept (
    noexcept(pass<Object>(obj).*declval<member_type>())
  ) -> decltype(pass<Object>(obj).*declval<member_type>()) {
    return pass<Object>(obj).*mp;
  }

  template <
    class Object,
    class=enable_if_t<has_object<Object>::value>
  > constexpr auto operator () (Object&& obj) const noexcept (
    noexcept(operator()(*pass<Object>(obj)))
  ) -> decltype(operator()(*pass<Object>(obj))) {
    return operator()(*pass<Object>(obj));
  }

  template <
    class Object,
    class=enable_if_t<is_derived<Object>::value>
  > constexpr auto operator () (Object* ptr) const noexcept (
    noexcept(operator()(*ptr))
  ) -> decltype(operator()(*ptr)) { return operator()(*ptr); }

  template <
    class Object,
    class=enable_if_t<is_derived<Object>::value>
  > auto operator () (reference<Object> ref) const noexcept (
    noexcept(operator()(ref.get()))
  ) -> decltype(operator()(ref.get())) { return operator()(ref.get()); }

private:
  member_type mp;
};

template <class T> using mem_fn = typename ::std::conditional<
  ::std::is_member_function_pointer<decay_t<T>>::value,
  pmf<T>,
  pdm<T>
>::type;

/* INVOKE pseudo-expression plumbing, *much* more simplified. */
struct undefined { constexpr undefined (...) noexcept { } };

/* We get some weird warnings under clang, so we actually give these functions
 * a body to get rid of it.
 */
template <class... Args>
constexpr undefined INVOKE (undefined, Args&&...) noexcept {
  return undefined { };
}

template <class Functor, class... Args>
constexpr auto INVOKE (Functor&& f, Args&&... args) -> enable_if_t<
  not ::std::is_member_pointer<decay_t<Functor>>::value,
  decltype(pass<Functor>(f)(pass<Args>(args)...))
> { return pass<Functor>(f)(pass<Args>(args)...); }

template <class Functor, class... Args>
constexpr auto INVOKE (Functor&& f, Args&&... args) -> enable_if_t<
  ::std::is_member_pointer<decay_t<Functor>>::value,
  decltype(mem_fn<Functor> { pass<Functor>(f) }(pass<Args>(args)...))
> { return mem_fn<Functor> { pass<Functor>(f) }(pass<Args>(args)...); }

template <bool, class...> struct invoke_of { };
template <class... Args> struct invoke_of<true, Args...> :
  meta::identity<decltype(INVOKE(declval<Args>()...))>
{ };

/* Used to provide lambda based 'pattern matching' for variant and optional
 * types.
 *
 * Based off of Dave Abrahams C++11 'generic lambda' example
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
    lambda_type(pass<Lambda>(lambda)),
    base_type(pass<Lambdas>(lambdas)...)
  { }

  using lambda_type::operator ();
  using base_type::operator ();
};

template <class... Lambdas>
auto make_overload(Lambdas&&... lambdas) -> overload<Lambdas...> {
  return overload<Lambdas...> { pass<Lambdas>(lambdas)... };
}

/* union used for variant<Ts...> and implementing aligned_union, which is
 * not provided by gcc 4.8.x, but is provided by clang. (aligned_union_t is
 * the only alias missing from <type_traits>
 */
template <class... Ts> union discriminate;
template <> union discriminate<> { };
template <class T, class... Ts>
union discriminate<T, Ts...> {
  T value;
  discriminate<Ts...> rest;
};

}}} /* namespace core::v1::impl */

#endif /* CORE_INTERNAL_HPP */
