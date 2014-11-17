#ifndef CORE_TYPE_TRAITS_HPP
#define CORE_TYPE_TRAITS_HPP

#include <type_traits>
#include <utility>
#include <tuple>

#include <core/meta.hpp>

namespace core {
inline namespace v1 {
namespace impl {

/* workaround for gcc */
template <class...> struct deducer { using type = void; };

using ::std::swap;

template <class T, class U, class=void>
struct is_swappable : ::std::false_type { };

template <class T, class U>
struct is_swappable<
  T,
  U,
  typename deducer<
    decltype(swap(::std::declval<T&>(), ::std::declval<U&>())),
    decltype(swap(::std::declval<U&>(), ::std::declval<T&>()))
  >::type
> : ::std::true_type { };

template <class T, class U>
struct is_nothrow_swappable : meta::all<
  is_swappable<T, U>,
  meta::boolean<noexcept(swap(::std::declval<T&>(), ::std::declval<U&>()))>,
  meta::boolean<noexcept(swap(::std::declval<U&>(), ::std::declval<T&>()))>
> { };

} /* namespace impl */

/* custom type traits and types */
template <class T> struct identity { using type = T; };

/* This is equivalent to the Boost.TypeTraits dont_care type */
template <::std::size_t I, class T>
using tuple_element_t = typename ::std::tuple_element<I, T>::type;
template <class T> using tuple_size_t = typename ::std::tuple_size<T>::type;
using ignore_t = decltype(::std::ignore);

/* a 'better named' (personal opinion!) form of the void_t type transformation
 * alias trait by Walter E. Brown.
 */
template <class... Ts> using deduce_t = typename impl::deducer<Ts...>::type;

/* tuple_size is used by unpack, so we expect it to be available.
 * We also expect ::std::get<N> to be available for the give type T
 */
template <class T, class=void> struct is_unpackable : ::std::false_type { };
template <class T>
struct is_unpackable<T, deduce_t<tuple_size_t<T>>> :
  ::std::true_type
{ };

/* Used for types that have a .at(size_type) member function. Used by
 * invoke for 'runpacking'
 */
template <class T, class=void> struct is_runpackable : ::std::false_type { };
template <class T>
struct is_runpackable<T, deduce_t<decltype(std::declval<T>().at(0ul))>> :
  ::std::true_type
{ };

/* extracts the class of a member function ponter */
template <class T> struct class_of : identity<T> { };
template <class Signature, class Type>
struct class_of<Signature Type::*> : identity<Type> { };

/* forward declaration */
template <class... Args> struct invokable;
template <class... Args> struct invoke_of;
template <class T> struct result_of; /* SFINAE result_of */

/* C++14 style aliases for standard traits */
template <class T>
using remove_volatile_t = typename ::std::remove_volatile<T>::type;

template <class T>
using remove_const_t = typename ::std::remove_const<T>::type;
template <class T> using remove_cv_t = typename ::std::remove_cv<T>::type;

template <class T>
using add_volatile_t = typename ::std::add_volatile<T>::type;
template <class T> using add_const_t = typename ::std::add_const<T>::type;
template <class T> using add_cv_t = typename ::std::add_cv<T>::type;

template <class T>
using add_lvalue_reference_t = typename ::std::add_lvalue_reference<T>::type;

template <class T>
using add_rvalue_reference_t = typename ::std::add_rvalue_reference<T>::type;

template <class T>
using remove_reference_t = typename ::std::remove_reference<T>::type;

template <class T>
using remove_pointer_t = typename ::std::remove_pointer<T>::type;

template <class T> using add_pointer_t = typename ::std::add_pointer<T>::type;

template <class T>
using make_unsigned_t = typename ::std::make_unsigned<T>::type;
template <class T> using make_signed_t = typename ::std::make_signed<T>::type;

template <class T>
using remove_extent_t = typename ::std::remove_extent<T>::type;

template <class T>
using remove_all_extents_t = typename ::std::remove_all_extents<T>::type;

template <::std::size_t Len, ::std::size_t Align>
using aligned_storage_t = typename ::std::aligned_storage<Len, Align>::type;

template <class T> using decay_t = typename ::std::decay<T>::type;

template <bool B, class T = void>
using enable_if_t = typename ::std::enable_if<B, T>::type;

template <bool B, class T, class F>
using conditional_t = typename ::std::conditional<B, T, F>::type;

template <class T>
using underlying_type_t = typename ::std::underlying_type<T>::type;

/* custom type trait specializations */
template <class... Args> using invoke_of_t = typename invoke_of<Args...>::type;
template <class T> using identity_t = typename identity<T>::type;
template <class T> using class_of_t = typename class_of<T>::type;

namespace impl {

struct undefined { undefined (...); };

/* Get the result of an attempt at the INVOKE expression */
/* fallback */
template <class... Args> auto invoke_expr (undefined, Args&&...) -> undefined;

template <class Functor, class Object, class... Args>
auto invoke_expr (Functor&& fun, Object&& obj, Args&&... args) -> enable_if_t<
  ::std::is_member_function_pointer<remove_reference_t<Functor>>::value and
  ::std::is_base_of<
    class_of_t<remove_reference_t<Functor>>,
    remove_reference_t<Object>
  >::value,
  decltype((::std::forward<Object>(obj).*fun)(::std::forward<Args>(args)...))
>;

template <class Functor, class Object, class... Args>
auto invoke_expr (Functor&& fun, Object&& obj, Args&&... args) -> enable_if_t<
  meta::all<
    ::std::is_member_function_pointer<remove_reference_t<Functor>>,
    meta::boolean<
      not ::std::is_base_of<
        class_of_t<remove_reference_t<Functor>>,
        remove_reference_t<Object>
      >::value
    >
  >::value,
  decltype(
    ((*::std::forward<Object>(obj)).*fun)(::std::forward<Args>(args)...)
  )
>;

template <class Functor, class Object>
auto invoke_expr (Functor&& functor, Object&& object) -> enable_if_t<
  ::std::is_member_object_pointer<remove_reference_t<Functor>>::value and
  ::std::is_base_of<
    class_of_t<remove_reference_t<Functor>>,
    remove_reference_t<Object>
  >::value,
  decltype(::std::forward<Object>(object).*functor)
>;

template <class Functor, class Object>
auto invoke_expr (Functor&& functor, Object&& object) -> enable_if_t<
  ::std::is_member_object_pointer<remove_reference_t<Functor>>::value and
  not ::std::is_base_of<
    class_of_t<remove_reference_t<Functor>>,
    remove_reference_t<Object>
  >::value,
  decltype((*::std::forward<Object>(object)).*functor)
>;

template <class Functor, class... Args>
auto invoke_expr (Functor&& functor, Args&&... args) -> enable_if_t<
  not ::std::is_member_pointer<decay_t<Functor>>::value,
  decltype(
    ::std::forward<Functor>(functor)(::std::forward<Args>(args)...)
  )
>;

template <bool, class... Args> struct invoke_of { };
template <class... Args>
struct invoke_of<true, Args...> :
  identity<decltype(invoke_expr(::std::declval<Args>()...))>
{ };

} /* namespace impl */

template <class... Args>
struct invokable : meta::none<
  std::is_same<
    decltype(impl::invoke_expr(::std::declval<Args>()...)),
    impl::undefined
  >
> { };

template <class... Args> struct invoke_of :
  impl::invoke_of<invokable<Args...>::value, Args...>
{ };

template <class F, class... Args>
struct result_of<F(Args...)> : invoke_of<F, Args...> { };

template <class T> using result_of_t = typename result_of<T>::type;

template <class... Ts> struct common_type;

template <class T> struct common_type<T> : identity<decay_t<T>> { };
template <class T, class U>
struct common_type<T, U> : identity<
  decay_t<decltype(true ? ::std::declval<T>() : ::std::declval<U>())>
> { };

template <class T, class U, class... Ts>
struct common_type<T, U, Ts...> : identity<
  typename common_type<
    typename common_type<T, U>::type,
    Ts...
  >::type
> { };

template <class... T> using common_type_t = typename common_type<T...>::type;

/* is_null_pointer */
template <class T> struct is_null_pointer : ::std::false_type { };

template <>
struct is_null_pointer<add_cv_t<::std::nullptr_t>> : ::std::true_type { };
template <>
struct is_null_pointer<::std::nullptr_t volatile> : ::std::true_type { };
template <>
struct is_null_pointer<::std::nullptr_t const> : ::std::true_type { };
template <>
struct is_null_pointer<::std::nullptr_t> : ::std::true_type { };

/* is_swappable */
template <class T, class U=T>
using is_swappable = impl::is_swappable<T, U>;

/* is_nothrow_swappable */
template <class T, class U=T>
using is_nothrow_swappable = impl::is_nothrow_swappable<T, U>;

/* These are now deprecated. Use what they alias to instead */
template <class... Args> using all_traits = meta::all<Args...>;
template <class... Args> using any_traits = meta::any<Args...>;
template <class... Args> using no_traits = meta::none<Args...>;


namespace trait {
namespace impl {

/* comparison */
template <class, class, class, class=void> struct eq : ::std::false_type { };
template <class, class, class, class=void> struct ne : ::std::false_type { };
template <class, class, class, class=void> struct ge : ::std::false_type { };
template <class, class, class, class=void> struct le : ::std::false_type { };
template <class, class, class, class=void> struct gt : ::std::false_type { };
template <class, class, class, class=void> struct lt : ::std::false_type { };

/* arithmetic */
template <class, class, class, class=void> struct mul : ::std::false_type { };
template <class, class, class, class=void> struct div : ::std::false_type { };
template <class, class, class, class=void> struct mod : ::std::false_type { };
template <class, class, class, class=void> struct add : ::std::false_type { };
template <class, class, class, class=void> struct sub : ::std::false_type { };

/* bitwise */
template <class, class, class, class=void> struct band : ::std::false_type { };
template <class, class, class, class=void> struct bxor : ::std::false_type { };
template <class, class, class, class=void> struct bor : ::std::false_type { };
template <class, class, class, class=void> struct bsl : ::std::false_type { };
template <class, class, class, class=void> struct bsr : ::std::false_type { };

/* logical */
template <class, class, class, class=void> struct land : ::std::false_type { };
template <class, class, class, class=void> struct lor : ::std::false_type { };

/* disgusting */
template <class, class, class, class=void> struct comma : ::std::false_type { };

/* prefix */
template <class, class, class=void> struct negate : ::std::false_type { };
template <class, class, class=void> struct plus : ::std::false_type { };

template <class, class, class=void> struct inc : ::std::false_type { };
template <class, class, class=void> struct dec : ::std::false_type { };

template <class, class, class=void> struct lnot : ::std::false_type { };
template <class, class, class=void> struct bnot : ::std::false_type { };

/* postfix */
template <class, class, class=void> struct postinc : ::std::false_type { };
template <class, class, class=void> struct postdec : ::std::false_type { };

/* special operator overloads */
template <class, class, class, class=void> struct subscript :
  ::std::false_type
{ };

template <class, class, class=void> struct dereference : ::std::false_type { };
template <class, class, class=void> struct address : ::std::false_type { };
template <class, class, class=void> struct arrow : ::std::false_type { };

/* comparison - ignore */
template <class T, class U>
struct eq<
  T, U, ignore_t,
  deduce_t<decltype(::std::declval<T>() == ::std::declval<U>())>
> : ::std::true_type { };

template <class T, class U>
struct ne<
  T, U, ignore_t,
  deduce_t<decltype(::std::declval<T>() != ::std::declval<U>())>
> : ::std::true_type { };

template <class T, class U>
struct ge<
  T, U, ignore_t,
  deduce_t<decltype(::std::declval<T>() >= ::std::declval<U>())>
> : ::std::true_type { };

template <class T, class U>
struct le<
  T, U, ignore_t,
  deduce_t<decltype(::std::declval<T>() <= ::std::declval<U>())>
> : ::std::true_type { };

template <class T, class U>
struct gt<
  T, U, ignore_t,
  deduce_t<decltype(::std::declval<T>() > ::std::declval<U>())>
> : ::std::true_type { };

template <class T, class U>
struct lt<
  T, U, ignore_t,
  deduce_t<decltype(::std::declval<T>() < ::std::declval<U>())>
> : ::std::true_type { };

/* arithmetic - ignore */
template <class T, class U>
struct mul<
  T, U, ignore_t,
  deduce_t<decltype(::std::declval<T>() * ::std::declval<T>())>
> : ::std::true_type { };

template <class T, class U>
struct div<
  T, U, ignore_t,
  deduce_t<decltype(::std::declval<T>() / ::std::declval<U>())>
> : ::std::true_type { };

template <class T, class U>
struct mod<
  T, U, ignore_t,
  deduce_t<decltype(::std::declval<T>() % ::std::declval<U>())>
> : ::std::true_type { };

template <class T, class U>
struct add<
  T, U, ignore_t,
  deduce_t<decltype(::std::declval<T>() + ::std::declval<U>())>
> : ::std::true_type { };

template <class T, class U>
struct sub<
  T, U, ignore_t,
  deduce_t<decltype(::std::declval<T>() - ::std::declval<U>())>
> : ::std::true_type { };

/* bitwise - ignore */
template <class T, class U>
struct band<
  T, U, ignore_t,
  deduce_t<decltype(::std::declval<T>() & ::std::declval<U>())>
> : ::std::true_type { };

template <class T, class U>
struct bxor<
  T, U, ignore_t,
  deduce_t<decltype(::std::declval<T>() ^ ::std::declval<U>())>
> : ::std::true_type { };

template <class T, class U>
struct bor<
  T, U, ignore_t,
  deduce_t<decltype(::std::declval<T>() | ::std::declval<U>())>
> : ::std::true_type { };

template <class T, class U>
struct bsl<
  T, U, ignore_t,
  deduce_t<decltype(::std::declval<T>() << ::std::declval<U>())>
> : ::std::true_type { };

template <class T, class U>
struct bsr<
  T, U, ignore_t,
  deduce_t<decltype(::std::declval<T>() >> ::std::declval<U>())>
> : ::std::true_type { };

template <class T>
struct bnot<T, ignore_t, deduce_t<decltype(~::std::declval<T>())>> :
  ::std::true_type
{ };

/* logic - ignore */
template <class T, class U>
struct land<
  T, U, ignore_t,
  deduce_t<decltype(::std::declval<T>() and ::std::declval<U>())>
> : ::std::true_type { };

template <class T, class U>
struct lor<
  T, U, ignore_t,
  deduce_t<decltype(::std::declval<T>() or ::std::declval<U>())>
> : ::std::true_type { };

template <class T>
struct lnot<T, ignore_t, deduce_t<decltype(not ::std::declval<T>())>> :
  ::std::true_type
{ };

/* special operators - ignore */
template <class T, class I>
struct subscript<
  T, I, ignore_t,
  deduce_t<decltype(::std::declval<T>()[::std::declval<I>()])>
> : ::std::true_type { };

template <class T>
struct dereference<
  T, ignore_t,
  deduce_t<decltype(::std::declval<T>().operator*())>
> : ::std::true_type { };

template <class T>
struct address<
  T, ignore_t,
  deduce_t<decltype(::std::declval<T>().operator&())>
> : ::std::true_type { };

template <class T>
struct arrow<
  T, ignore_t,
  deduce_t<decltype(::std::declval<T>().operator->())>
> : ::std::true_type { };

/* comparison */
template <class T, class U, class R>
struct eq<
  T, U, R,
  deduce_t<decltype(::std::declval<T>() == ::std::declval<U>())>
> : ::std::is_convertible<
  decltype(::std::declval<T>() == ::std::declval<U>()),
  R
> { };

template <class T, class U, class R>
struct ne<
  T, U, R,
  deduce_t<decltype(::std::declval<T>() != ::std::declval<U>())>
> : ::std::is_convertible<
  decltype(::std::declval<T>() == ::std::declval<U>()),
  R
> { };

template <class T, class U, class R>
struct ge<
  T, U, R,
  deduce_t<decltype(::std::declval<T>() >= ::std::declval<U>())>
> : ::std::is_convertible<
  decltype(::std::declval<T>() >= ::std::declval<U>()),
  R
> { };

template <class T, class U, class R>
struct le<
  T, U, R,
  deduce_t<decltype(::std::declval<T>() <= ::std::declval<U>())>
> : ::std::is_convertible<
  decltype(::std::declval<T>() <= ::std::declval<U>()),
  R
> { };

template <class T, class U, class R>
struct gt<
  T, U, R,
  deduce_t<decltype(::std::declval<T>() > ::std::declval<U>())>
> : ::std::is_convertible<
  decltype(::std::declval<T>() > ::std::declval<U>()),
  R
> { };

template <class T, class U, class R>
struct lt<
  T, U, R,
  deduce_t<decltype(::std::declval<T>() < ::std::declval<U>())>
> : ::std::is_convertible<
  decltype(::std::declval<T>() < ::std::declval<U>()),
  R
> { };

/* arithmetic */
template <class T, class U, class R>
struct mul<
  T, U, R,
  deduce_t<decltype(::std::declval<T>() * ::std::declval<U>())>
> : ::std::is_convertible<
  decltype(::std::declval<T>() * ::std::declval<U>()),
  R
> { };

template <class T, class U, class R>
struct div<
  T, U, R,
  deduce_t<decltype(::std::declval<T>() / ::std::declval<U>())>
> : ::std::is_convertible<
  decltype(::std::declval<T>() / ::std::declval<U>()),
  R
> { };

template <class T, class U, class R>
struct mod<
  T, U, R,
  deduce_t<decltype(::std::declval<T>() % ::std::declval<U>())>
> : ::std::is_convertible<
  decltype(::std::declval<T>() % ::std::declval<U>()),
  R
> { };

template <class T, class U, class R>
struct add<
  T, U, R,
  deduce_t<decltype(::std::declval<T>() + ::std::declval<U>())>
> : ::std::is_convertible<
  decltype(::std::declval<T>() + ::std::declval<U>()),
  R
> { };

template <class T, class U, class R>
struct sub<
  T, U, R,
  deduce_t<decltype(::std::declval<T>() - ::std::declval<U>())>
> : ::std::is_convertible<
  decltype(::std::declval<T>() - ::std::declval<U>()),
  R
> { };

/* bitwise */
template <class T, class U, class R>
struct band<
  T, U, R,
  deduce_t<decltype(::std::declval<T>() & ::std::declval<U>())>
> : ::std::is_convertible<
  decltype(::std::declval<T>() & ::std::declval<U>()),
  R
> { };

template <class T, class U, class R>
struct bxor<
  T, U, R,
  deduce_t<decltype(::std::declval<T>() ^ ::std::declval<U>())>
> : ::std::is_convertible<
  decltype(::std::declval<T>() & ::std::declval<U>()),
  R
> { };

template <class T, class U, class R>
struct bor<
  T, U, R,
  deduce_t<decltype(::std::declval<T>() | ::std::declval<U>())>
> : ::std::is_convertible<
  decltype(::std::declval<T>() | ::std::declval<U>()),
  R
> { };

template <class T, class U, class R>
struct bsl<
  T, U, R,
  deduce_t<decltype(::std::declval<T>() << ::std::declval<U>())>
> : ::std::is_convertible<
  decltype(::std::declval<T>() << ::std::declval<U>()),
  R
> { };

template <class T, class U, class R>
struct bsr<
  T, U, R,
  deduce_t<decltype(::std::declval<T>() >> ::std::declval<U>())>
> : ::std::is_convertible<
  decltype(::std::declval<T>() >> ::std::declval<U>()),
  R
> { };

template <class T, class R>
struct bnot<T, R, deduce_t<decltype(~::std::declval<T>())>> :
  ::std::is_convertible<decltype(~::std::declval<T>()), R>
{ };

/* logical */
template <class T, class U, class R>
struct land<
  T, U, R,
  deduce_t<decltype(::std::declval<T>() and ::std::declval<U>())>
> : ::std::is_convertible<
  decltype(::std::declval<T>() and ::std::declval<U>()),
  R
> { };

template <class T, class U, class R>
struct lor<
  T, U, R,
  deduce_t<decltype(::std::declval<T>() or ::std::declval<U>())>
> : ::std::is_convertible<
  decltype(::std::declval<T>() or ::std::declval<U>()),
  R
> { };

template <class T, class R>
struct lnot<T, R, deduce_t<decltype(not ::std::declval<T>())>> :
  ::std::is_convertible<decltype(not ::std::declval<T>()), R>
{ };

/* disgusting */
template <class T, class U, class R>
struct comma<
  T, U, R,
  deduce_t<decltype((::std::declval<T>(), ::std::declval<U>()))>
> : ::std::is_convertible<
  decltype((::std::declval<T>(), ::std::declval<U>())),
  R
> { };

/* unary */
template <class T, class R>
struct negate<
  T, R, deduce_t<decltype(-::std::declval<T>())>
> : ::std::is_convertible<decltype(-::std::declval<T>()), R> { };

template <class T, class R>
struct plus<
  T, R, deduce_t<decltype(+::std::declval<T>())>
> : ::std::is_convertible<decltype(+::std::declval<T>()), R> { };

template <class T, class R>
struct inc<
  T, R, deduce_t<decltype(++::std::declval<T>())>
> : ::std::is_convertible<decltype(++::std::declval<T>()), R> { };

template <class T, class R>
struct dec<
  T, R, deduce_t<decltype(--::std::declval<T>())>
> : ::std::is_convertible<decltype(--::std::declval<T>()), R> { };

template <class T, class R>
struct postinc<
  T, R, deduce_t<decltype(::std::declval<T>()++)>
> : ::std::is_convertible<decltype(::std::declval<T>()++), R> { };

template <class T, class R>
struct postdec<
  T, R, deduce_t<decltype(::std::declval<T>()--)>
> : ::std::is_convertible<decltype(::std::declval<T>()--), R> { };

/* special operator overloads */
template <class T, class I, class R>
struct subscript<
  T, I, R,
  deduce_t<decltype(::std::declval<T>()[std::declval<I>()])>
> : ::std::is_convertible<
  decltype(::std::declval<T>()[::std::declval<I>()]),
  R
> { };

template <class T, class R>
struct address<T, R, deduce_t<decltype(::std::declval<T>().operator&())>> :
  std::is_convertible<decltype(::std::declval<T>().operator&()), R>
{ };

template <class T, class R>
struct arrow<T, R, deduce_t<decltype(::std::declval<T>().operator->())>> :
  std::is_convertible<decltype(::std::declval<T>().operator->()), R>
{ };

} /* namespace impl */

/* comparison */
template <class T, class U=T, class R=ignore_t> using eq = impl::eq<T, U, R>;
template <class T, class U=T, class R=ignore_t> using ne = impl::ne<T, U, R>;
template <class T, class U=T, class R=ignore_t> using ge = impl::ge<T, U, R>;
template <class T, class U=T, class R=ignore_t> using le = impl::le<T, U, R>;
template <class T, class U=T, class R=ignore_t> using gt = impl::gt<T, U, R>;
template <class T, class U=T, class R=ignore_t> using lt = impl::lt<T, U, R>;

/* arithmetic */
template <class T, class U=T, class R=ignore_t> using mul = impl::mul<T, U, R>;
template <class T, class U=T, class R=ignore_t> using div = impl::div<T, U, R>;
template <class T, class U=T, class R=ignore_t> using mod = impl::mod<T, U, R>;
template <class T, class U=T, class R=ignore_t> using add = impl::add<T, U, R>;
template <class T, class U=T, class R=ignore_t> using sub = impl::sub<T, U, R>;

/* bitwise */
template <class T, class U=T, class R=ignore_t>
using band = impl::band<T, U, R>;

template <class T, class U=T, class R=ignore_t>
using bxor = impl::bxor<T, U, R>;

template <class T, class U=T, class R=ignore_t> using bor = impl::bor<T, U, R>;
template <class T, class U=T, class R=ignore_t> using bsl = impl::bsl<T, U, R>;
template <class T, class U=T, class R=ignore_t> using bsr = impl::bsr<T, U, R>;
template <class T, class R=ignore_t> using bnot = impl::bnot<T, R>;

/* logical */
template <class T, class U=T, class R=ignore_t>
using land = impl::land<T, U, R>;

template <class T, class U=T, class R=ignore_t> using lor = impl::lor<T, U, R>;
template <class T, class R=ignore_t> using lnot = impl::lnot<T, R>;

/* disgusting (we do not allow ignoring the return type) */
template <class T, class U, class R> using comma = impl::comma<T, U, R>;

/* unary (prefix) */
template <class T, class R=ignore_t> using negate = impl::negate<T, R>;
template <class T, class R=ignore_t> using plus = impl::plus<T, R>;

template <class T, class R=ignore_t> using inc = impl::inc<T, R>;
template <class T, class R=ignore_t> using dec = impl::dec<T, R>;

/* unary (postfix) */
template <class T, class R=ignore_t> using postinc = impl::postinc<T, R>;
template <class T, class R=ignore_t> using postdec = impl::postdec<T, R>;

/* special operators */
template <class T, class I, class R=ignore_t>
using subscript = impl::subscript<T, I, R>;

template <class T, class R=ignore_t>
using dereference = impl::dereference<T, R>;

template <class T, class R=ignore_t> using address = impl::address<T, R>;
template <class T, class R> using arrow = impl::arrow<T, R>;

}}} /* namespace core::v1::trait */

#endif /* CORE_TYPE_TRAITS_HPP */
