#ifndef CORE_OPTIONAL_HPP
#define CORE_OPTIONAL_HPP

#include <initializer_list>
#include <functional>
#include <stdexcept>
#include <memory>

#include <cstdint>

#include <core/type_traits.hpp>
#include <core/utility.hpp>

namespace core {
inline namespace v1 {

struct in_place_t { };
struct nullopt_t { };

constexpr in_place_t in_place { };
constexpr nullopt_t nullopt { };

struct bad_optional_access final : public std::logic_error {
  using std::logic_error::logic_error;
};

namespace impl {

/* this is the default 'false' case */
template <class T, bool = ::std::is_trivially_destructible<T>::value>
struct storage {

  using value_type = T;

  static constexpr bool nothrow_mv_ctor = ::std::is_nothrow_move_constructible<
    value_type
  >::value;

  union {
    ::std::uint8_t dummy;
    value_type val;
  };
  bool engaged { false };

  constexpr storage () noexcept : dummy { '\0' } { }
  storage (storage const& that) :
    engaged { that.engaged }
  {
    if (not this->engaged) { return; }
    ::new(::std::addressof(this->val)) value_type { that.val };
  }

  storage (storage&& that) noexcept(nothrow_mv_ctor) :
    engaged { that.engaged }
  {
    if (not this->engaged) { return; }
    ::new(::std::addressof(this->val)) value_type { ::core::move(that.val) };
  }

  constexpr storage (value_type const& value) :
    val { value },
    engaged { true }
  { }

  constexpr storage (value_type&& value) noexcept(nothrow_mv_ctor) :
    val { ::core::move(value) },
    engaged { true }
  { }

  template <class... Args>
  constexpr explicit storage (in_place_t, Args&&... args) :
    val { ::core::forward<Args>(args)... },
    engaged { true }
  { }

  ~storage () noexcept { if (this->engaged) { this->val.~value_type(); } }
};

template <class T>
struct storage<T, true> {
  using value_type = T;
  static constexpr bool nothrow_mv_ctor = ::std::is_nothrow_move_constructible<
    value_type
  >::value;
  union {
    ::std::uint8_t dummy;
    value_type val;
  };
  bool engaged { false };

  constexpr storage () noexcept : dummy { '\0' } { }
  storage (storage const& that) :
    engaged { that.engaged }
  {
    if (not this->engaged) { return; }
    ::new(::std::addressof(this->val)) value_type { that.val };
  }

  storage (storage&& that) noexcept(nothrow_mv_ctor) :
    engaged { that.engaged }
  {
    if (not this->engaged) { return; }
    ::new(::std::addressof(this->val)) value_type {
      ::core::move(that.val)
    };
  }

  constexpr storage (value_type const& value) :
    val { value },
    engaged { true }
  { }

  constexpr storage (value_type&& value) noexcept(nothrow_mv_ctor) :
    val { ::core::move(value) },
    engaged { true }
  { }

  template <class... Args>
  constexpr explicit storage (in_place_t, Args&&... args) :
    val { ::core::forward<Args>(args)... },
    engaged { true }
  { }
};

} /* namespace impl */

template <class Type>
struct optional final : private impl::storage<Type> {
  using base = impl::storage<Type>;
  using value_type = typename impl::storage<Type>::value_type;

  /* compiler enforcement */
  static_assert(
    not ::std::is_reference<value_type>::value,
    "Cannot have optional reference (ill-formed)"
  );

  static_assert(
    not ::std::is_same<decay_t<value_type>, nullopt_t>::value,
    "Cannot have optional<nullopt_t> (ill-formed)"
  );

  static_assert(
    not ::std::is_same<decay_t<value_type>, in_place_t>::value,
    "Cannot have optional<in_place_t> (ill-formed)"
  );

  static_assert(
    not ::std::is_same<decay_t<value_type>, ::std::nullptr_t>::value,
    "Cannot have optional nullptr (tautological)"
  );

  static_assert(
    not ::std::is_same<decay_t<value_type>, void>::value,
    "Cannot have optional<void> (ill-formed)"
  );

  static_assert(
    ::std::is_object<value_type>::value,
    "Cannot have optional with a non-object type (undefined behavior)"
  );

  static_assert(
    ::std::is_nothrow_destructible<value_type>::value,
    "Cannot have optional with non-noexcept destructible (undefined behavior)"
  );

  constexpr optional () noexcept { }
  optional (optional const&) = default;
  optional (optional&&) = default;
  ~optional () = default;

  constexpr optional (nullopt_t) noexcept { }

  constexpr optional (value_type const& value) :
    base { value }
  { }

  constexpr optional (value_type&& value) noexcept(base::nothrow_mv_ctor) :
    base { ::core::move(value) }
  { }

  template <
    class... Args,
    class=enable_if_t< ::std::is_constructible<value_type, Args...>::value>
  > constexpr explicit optional (in_place_t, Args&&... args) :
    base { in_place, ::core::forward<Args>(args)... }
  { }

  template <
    class T,
    class... Args,
    class=enable_if_t<
      ::std::is_constructible<
        value_type,
        ::std::initializer_list<T>&,
        Args...
      >::value
    >
  > constexpr explicit optional (
    in_place_t,
    ::std::initializer_list<T> il,
    Args&&... args
  ) : base { in_place, il, ::core::forward<Args>(args)... } { }

  optional& operator = (optional const& that) {
    optional { that }.swap(*this);
    return *this;
  }

  optional& operator = (optional&& that) noexcept (
    all_traits<
      ::std::is_nothrow_move_assignable<value_type>,
      ::std::is_nothrow_move_constructible<value_type>
    >::value
  ) {
    optional { ::core::move(that) }.swap(*this);
    return *this;
  }

  template <
    class T,
    class=enable_if_t<
      not ::std::is_same<decay_t<T>, optional>::value and
      ::std::is_constructible<value_type, T>::value and
      ::std::is_assignable<value_type, T>::value
    >
  > optional& operator = (T&& value) {
    if (not this->engaged) { this->emplace(::core::forward<T>(value)); }
    else { **this = ::core::forward<T>(value); }
    return *this;
  }

  optional& operator = (nullopt_t) noexcept {
    if (this->engaged) {
      this->val.~value_type();
      this->engaged = false;
    }
    return *this;
  }

  void swap (optional& that) noexcept(is_nothrow_swappable<value_type>::value) {
    using std::swap;
    if (not *this and not that) { return; }
    if (*this and that) {
      swap(**this, *that);
      return;
    }

    auto& to_disengage = *this ? *this : that;
    auto& to_engage = *this ? that : *this;

    to_engage.emplace(::core::move(*to_disengage));
    to_disengage = nullopt;
  }

  constexpr value_type const* operator -> () const noexcept {
    return ::std::addressof(this->val);
  }

  value_type* operator -> () noexcept { return ::std::addressof(this->val); }

  constexpr value_type const& operator * () const noexcept {
    return this->val;
  }

  value_type& operator * () noexcept { return this->val; }

  constexpr explicit operator bool () const { return this->engaged; }

  template <class T, class... Args>
  void emplace (::std::initializer_list<T> il, Args&&... args) {
    *this = nullopt;
    ::new(::std::addressof(this->val)) value_type {
      il,
      ::core::forward<Args>(args)...
    };
    this->engaged = true;
  }

  template <class... Args>
  void emplace (Args&&... args) {
    *this = nullopt;
    ::new(::std::addressof(this->val)) value_type {
      ::core::forward<Args>(args)...
    };
    this->engaged = true;
  }

  constexpr value_type const& value () const noexcept(false) {
    return *this
      ? **this
      : (throw bad_optional_access { "optional is disengaged" }, **this);
  }

  value_type& value () noexcept(false) {
    if (this->engaged) { return **this; }
    throw bad_optional_access { "optional is disengaged" };
  }

  template <
    class T,
    class=enable_if_t<
      all_traits<
        ::std::is_copy_constructible<value_type>,
        ::std::is_convertible<T, value_type>
      >::value
    >
  > constexpr value_type value_or (T&& val) const& {
    return *this ? **this : static_cast<value_type>(::core::forward<T>(val));
  }

  template <
    class T,
    class=enable_if_t<
      all_traits<
        ::std::is_move_constructible<value_type>,
        ::std::is_convertible<T, value_type>
      >::value
    >
  > value_type value_or (T&& val) && {
    return *this
      ? value_type { ::core::move(**this) }
      : static_cast<value_type>(::core::forward<T>(val));
  }

};

template <class Type>
constexpr auto make_optional (Type&& value) -> optional<decay_t<Type>> {
  return optional<decay_t<Type>> { ::core::forward<Type>(value) };
}

template <class T>
constexpr bool operator == (
  optional<T> const& lhs,
  optional<T> const& rhs
) noexcept {
  return static_cast<bool>(lhs) != static_cast<bool>(rhs)
    ? false
    : (not lhs and not rhs) ? true : *lhs == *rhs;
}

template <class T>
constexpr bool operator < (
  optional<T> const& lhs,
  optional<T> const& rhs
) noexcept {
  return static_cast<bool>(rhs) == false
    ? false
    : static_cast<bool>(lhs) == false ? true : ::std::less<T> { }(*lhs, *rhs);
}

template <class T>
constexpr bool operator == (optional<T> const& lhs, nullopt_t) noexcept {
  return not lhs;
}

template <class T>
constexpr bool operator == (nullopt_t, optional<T> const&) noexcept {
  return false;
}

template <class T>
constexpr bool operator < (optional<T> const& lhs, nullopt_t) noexcept {
  return not lhs;
}

template <class T>
constexpr bool operator < (nullopt_t, optional<T> const& rhs) noexcept {
  return static_cast<bool>(rhs);
}

template <class T>
constexpr bool operator == (optional<T> const& opt, T const& value) noexcept {
  return opt ? *opt == value : false;
}

template <class T>
constexpr bool operator == (T const& value, optional<T> const& opt) noexcept {
  return opt ? value == *opt : false;
}

template <class T>
constexpr bool operator < (optional<T> const& opt, T const& value) noexcept {
  return opt ? ::std::less<T>{ }(*opt, value) : true;
}

template <class T>
constexpr bool operator < (T const& value, optional<T> const& opt) noexcept {
  return opt ? ::std::less<T> { }(value, *opt) : false;
}

template <class T>
void swap (optional<T>& lhs, optional<T>& rhs) noexcept(
  noexcept(lhs.swap(rhs))
) { lhs.swap(rhs); }

}} /* namespace core::v1 */

namespace std {

template <class Type>
struct hash< ::core::v1::optional<Type>> {
  using result_type = typename hash<Type>::result_type;
  using argument_type = ::core::v1::optional<Type>;

  result_type operator () (argument_type const& value) const noexcept {
    return value ? hash<Type> { }(*value) : result_type { };
  }
};

} /* namespace std */

#endif /* CORE_OPTIONAL_HPP */
