#ifndef CORE_OPTIONAL_HPP
#define CORE_OPTIONAL_HPP

#include <initializer_list>
#include <system_error>
#include <functional>
#include <exception>
#include <stdexcept>
#include <memory>

#include <cstdint>

#include <core/type_traits.hpp>
#include <core/utility.hpp>

namespace core {
inline namespace v1 {
namespace impl {

template <class T>
class has_addressof {
  template <class U>
  static auto test (U* ptr) noexcept -> decltype(ptr->operator&(), void());
  template <class> static void test (...) noexcept(false);
public:
  static constexpr bool value = noexcept(test<T>(nullptr));
};

template <class T>
struct addressof : ::std::integral_constant<bool, has_addressof<T>::value> { };

struct place_t { };
constexpr place_t place { };

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
  constexpr explicit storage (place_t, Args&&... args) :
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
  constexpr explicit storage (place_t, Args&&... args) :
    val { ::core::forward<Args>(args)... },
    engaged { true }
  { }
};

} /* namespace impl */

struct in_place_t { };
struct nullopt_t { constexpr explicit nullopt_t (int) noexcept { } };

constexpr in_place_t in_place { };
constexpr nullopt_t nullopt { 0 };

struct bad_optional_access final : ::std::logic_error {
  using ::std::logic_error::logic_error;
};

struct bad_expected_type : ::std::logic_error {
  using ::std::logic_error::logic_error;
};

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
    base { impl::place, ::core::forward<Args>(args)... }
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
  ) : base { impl::place, il, ::core::forward<Args>(args)... } { }

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
      ::std::is_assignable<value_type&, T>::value
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

  void swap (optional& that) noexcept(
    all_traits<
      is_nothrow_swappable<value_type>,
      ::std::is_nothrow_move_constructible<value_type>
    >::value
  ) {
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

  constexpr explicit operator bool () const { return this->engaged; }

  constexpr value_type const& operator * () const noexcept {
    return this->val;
  }

  value_type& operator * () noexcept { return this->val; }

  constexpr value_type const* operator -> () const noexcept {
    return this->ptr(impl::addressof<value_type> { });
  }

  value_type* operator -> () noexcept { return ::std::addressof(this->val); }

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
    if (*this) { return **this; }
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

private:
  constexpr value_type const* ptr (::std::false_type) const noexcept {
    return &this->val;
  }

  value_type const* ptr (::std::true_type) const noexcept {
    return ::std::addressof(this->val);
  }
};

template <class Type>
struct expected final {
  using value_type = Type;

  static constexpr bool nothrow = ::std::is_nothrow_move_constructible<
    value_type
  >::value;

  /* compiler enforcement */
  static_assert(
    not ::std::is_reference<value_type>::value,
    "Cannot have expected reference (ill-formed)"
  );

  static_assert(
    not ::std::is_same<decay_t<value_type>, nullopt_t>::value,
    "Cannot have expected<nullopt_t> (ill-formed)"
  );

  static_assert(
    not ::std::is_same<decay_t<value_type>, in_place_t>::value,
    "Cannot have expected<in_place_t> (ill-formed)"
  );

  static_assert(
    not ::std::is_same<decay_t<value_type>, ::std::exception_ptr>::value,
    "Cannot have expected<std::exception_ptr> (tautological)"
  );

  static_assert(
    ::std::is_object<value_type>::value,
    "Cannot have expected with non-object type (undefined behavior)"
  );

  static_assert(
    ::std::is_nothrow_destructible<value_type>::value,
    "Cannot have expected with throwable destructor (undefined behavior)"
  );

  explicit expected (::std::exception_ptr ptr) noexcept :
    ptr { ptr }
  { }

  explicit expected (value_type const& val) :
    val { val },
    valid { true }
  { }

  explicit expected (value_type&& val) noexcept(nothrow) :
    val { ::core::move(val) },
    valid { true }
  { }

  template <
    class... Args,
    class=enable_if_t< ::std::is_constructible<value_type, Args...>::value>
  > explicit expected (in_place_t, Args&&... args) :
    val { ::core::forward<Args>(args)... },
    valid { true }
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
  > explicit expected (
    in_place_t,
    ::std::initializer_list<T> il,
    Args&&... args
  ) : val { il, ::core::forward<Args>(args)... }, valid { true } { }

  expected (expected const& that) :
    valid { that.valid }
  {
    if (this->valid) {
      ::new (::std::addressof(this->val)) value_type { that.val };
    } else {
      ::new (::std::addressof(this->ptr)) ::std::exception_ptr { that.ptr };
    }
  }

  expected (expected&& that) noexcept(nothrow) :
    valid { that.valid }
  {
    if (this->valid) {
      ::new (::std::addressof(this->val)) value_type { ::core::move(that.val) };
    } else {
      ::new (::std::addressof(this->ptr)) ::std::exception_ptr { that.ptr };
    }
  }

  expected () :
    val { },
    valid { true }
  { }

  ~expected () noexcept { this->reset(); }

  expected& operator = (expected const& that) {
    expected { that }.swap(*this);
    return *this;
  }

  expected& operator = (expected&& that) noexcept(
    all_traits<
      ::std::is_nothrow_move_assignable<value_type>,
      ::std::is_nothrow_move_constructible<value_type>
    >::value
  ) {
    expected { ::core::move(that) }.swap(*this);
    return *this;
  }

  template <
    class T,
    class=enable_if_t<
      all_traits<
        no_traits<
          ::std::is_same<decay_t<T>, expected>,
          ::std::is_same<decay_t<T>, ::std::exception_ptr>
        >,
        ::std::is_constructible<value_type, T>,
        ::std::is_assignable<value_type&, T>
      >::value
    >
  > expected& operator = (T&& value) {
    if (not *this) { this->emplace(::core::forward<T>(value)); }
    else { **this = ::core::forward<T>(value); }
    return *this;
  }

  expected& operator = (value_type const& value) {
    if (not *this) {
      this->ptr.~exception_ptr();
      ::new (::std::addressof(this->val)) value_type { value };
      this->valid = true;
    } else { **this = value; }
    return *this;
  }

  expected& operator = (value_type&& value) {
    if (not *this) {
      this->ptr.~exception_ptr();
      ::new (::std::addressof(this->val)) value_type { ::core::move(value) };
      this->valid = true;
    } else { **this = ::core::move(value); }
    return *this;
  }

  expected& operator = (::std::exception_ptr ptr) {
    if (not *this) { this->ptr = ptr; }
    else {
      this->val.~value_type();
      ::new (::std::addressof(this->ptr)) ::std::exception_ptr { ptr };
      this->valid = false;
    }
    return *this;
  }

  void swap (expected& that) noexcept(
    all_traits<
      is_nothrow_swappable<value_type>,
      ::std::is_nothrow_move_constructible<value_type>
    >::value
  ) {
    using ::std::swap;

    if (not this->valid and not that.valid) {
      swap(this->ptr, that.ptr);
      return;
    }

    if (this->valid and that.valid) {
      swap(this->val, that.val);
      return;
    }

    auto& to_invalidate = *this ? *this : that;
    auto& to_validate = *this ? that : *this;
    auto ptr = to_validate.ptr;
    to_validate.emplace(::core::move(*to_invalidate));
    to_invalidate = ptr;
  }

  explicit operator bool () const noexcept { return this->valid; }

  value_type const& operator * () const noexcept { return this->val; }
  value_type& operator * () noexcept { return this->val; }

  value_type const* operator -> () const noexcept {
    return ::std::addressof(this->val);
  }
  value_type* operator -> () noexcept { return ::std::addressof(this->val); }

  template <class T, class... Args>
  void emplace (::std::initializer_list<T> il, Args&&... args) {
    this->reset();
    ::new(::std::addressof(this->val)) value_type {
      il,
      ::core::forward<Args>(args)...
    };
    this->valid = true;
  }

  template <class... Args>
  void emplace (Args&&... args) {
    this->reset();
    ::new(::std::addressof(this->val)) value_type {
      ::core::forward<Args>(args)...
    };
    this->valid = true;
  }

  value_type const& value () const noexcept(false) {
    if (not *this) { ::std::rethrow_exception(this->ptr); }
    return **this;
  }

  value_type& value () noexcept(false) {
    if (not *this) { ::std::rethrow_exception(this->ptr); }
    return **this;
  }

  template <
    class T,
    class=enable_if_t<
      all_traits<
        ::std::is_copy_constructible<value_type>,
        ::std::is_convertible<T, value_type>
      >::value
    >
  > value_type value_or (T&& val) const& {
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

  template <class E>
  E expect () const noexcept(false) {
    try { this->raise(); }
    catch (E const& e) { return e; }
    catch (...) {
      ::std::throw_with_nested(bad_expected_type { "unexpected exception" });
    }
  }

  [[noreturn]] void raise () const noexcept(false) {
    if (this->valid) { throw bad_expected_type { "expected<T> is valid" }; }
    ::std::rethrow_exception(this->ptr);
  }

  ::std::exception_ptr get_ptr () const noexcept(false) {
    if (this->valid) { throw bad_expected_type { "expected<T> is valid" }; }
    return this->ptr;
  }

private:

  void reset () {
    *this ? this->val.~value_type() : this->ptr.~exception_ptr();
  }

  union {
    value_type val;
    ::std::exception_ptr ptr;
  };
  bool valid { false };
};

template <>
struct expected<void> final {
  using value_type = void;

  explicit expected (::std::exception_ptr ptr) noexcept : ptr { ptr } { }
  expected (expected const& that) noexcept : ptr { that.ptr } { }
  expected (expected&& that) noexcept :
    ptr { ::core::move(that.ptr) }
  { }

  expected () noexcept : ptr { nullptr } { }
  ~expected () noexcept { }

  expected& operator = (::std::exception_ptr ptr) noexcept {
    expected { ptr }.swap(*this);
    return *this;
  }

  expected& operator = (expected const& that) noexcept {
    expected { that }.swap(*this);
    return *this;
  }

  expected& operator = (expected&& that) noexcept {
    expected { ::core::move(that) }.swap(*this);
    return *this;
  }

  void swap (expected& that) noexcept {
    using ::std::swap;
    swap(this->ptr, that.ptr);
  }

  explicit operator bool () const noexcept { return not this->ptr; }

  template <class E>
  E expect () const noexcept(false) {
    try { this->raise(); }
    catch (E const& e) { return e; }
    catch (...) {
      ::std::throw_with_nested(bad_expected_type { "unexpected exception" });
    }
  }

  [[noreturn]] void raise () const noexcept(false) {
    if (*this) { throw bad_expected_type { "valid expected<void>" }; }
    ::std::rethrow_exception(this->ptr);
  }

  ::std::exception_ptr get_ptr () const noexcept(false) {
    if (*this) { throw bad_expected_type { "valid expected<void>" }; }
    return this->ptr;
  }

private:
  ::std::exception_ptr ptr;
};

/* comparison with optional<T> */
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
constexpr bool operator != (
  optional<T> const& lhs,
  optional<T> const& rhs
) noexcept { return not (lhs == rhs); }

template <class T>
constexpr bool operator >= (
  optional<T> const& lhs,
  optional<T> const& rhs
) noexcept { return not (lhs < rhs); }

template <class T>
constexpr bool operator <= (
  optional<T> const& lhs,
  optional<T> const& rhs
) noexcept { return not (rhs < lhs); }

template <class T>
constexpr bool operator > (
  optional<T> const& lhs,
  optional<T> const& rhs
) noexcept { return rhs < lhs; }

template <class T>
constexpr bool operator < (
  optional<T> const& lhs,
  optional<T> const& rhs
) noexcept {
  return static_cast<bool>(rhs) == false
    ? false
    : static_cast<bool>(lhs) == false ? true : *lhs < *rhs;
}

/* comparison with expected<T> */
template <class T>
bool operator == (expected<T> const& lhs, expected<T> const& rhs) noexcept {
  if (lhs and rhs) { return *lhs == *rhs; }
  return false;
}

template <class T>
bool operator != (expected<T> const& lhs, expected<T> const& rhs) noexcept {
  return not (lhs == rhs);
}

template <class T>
bool operator >= (expected<T> const& lhs, expected<T> const& rhs) noexcept {
  return not (lhs < rhs);
}

template <class T>
bool operator <= (expected<T> const& lhs, expected<T> const& rhs) noexcept {
  return not (rhs < lhs);
}

template <class T>
bool operator > (expected<T> const& lhs, expected<T> const& rhs) noexcept {
  return rhs < lhs;
}

template <class T>
bool operator < (expected<T> const& lhs, expected<T> const& rhs) noexcept {
  if (not rhs) { return false; }
  if (not lhs) { return true; }
  return *lhs < *rhs;
}

template <>
inline bool operator == <void>(
  expected<void> const& lhs,
  expected<void> const& rhs
) noexcept { return bool(lhs) == bool(rhs); }

/* comparison with nullopt */
template <class T>
constexpr bool operator == (optional<T> const& lhs, nullopt_t) noexcept {
  return not lhs;
}

template <class T>
constexpr bool operator == (nullopt_t, optional<T> const& rhs) noexcept {
  return not rhs;
}

template <class T>
constexpr bool operator != (optional<T> const& lhs, nullopt_t) noexcept {
  return static_cast<bool>(lhs);
}

template <class T>
constexpr bool operator != (nullopt_t, optional<T> const& rhs) noexcept {
  return static_cast<bool>(rhs);
}

template <class T>
constexpr bool operator >= (optional<T> const&, nullopt_t) noexcept {
  return true;
}

template <class T>
constexpr bool operator >= (nullopt_t, optional<T> const& rhs) noexcept {
  return not rhs;
}

template <class T>
constexpr bool operator <= (optional<T> const& lhs, nullopt_t) noexcept {
  return not lhs;
}

template <class T>
constexpr bool operator <= (nullopt_t, optional<T> const&) noexcept {
  return true;
}

template <class T>
constexpr bool operator > (optional<T> const& lhs, nullopt_t) noexcept {
  return static_cast<bool>(lhs);
}

template <class T>
constexpr bool operator > (nullopt_t, optional<T> const&) noexcept {
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

/* comparison of optional<T> with T */
template <class T>
constexpr bool operator == (optional<T> const& opt, T const& value) noexcept {
  return opt ? *opt == value : false;
}

template <class T>
constexpr bool operator != (optional<T> const& opt, T const& value) noexcept {
  return opt ? not (*opt == value) : true;
}

template <class T>
constexpr bool operator >= (optional<T> const& opt, T const& value) noexcept {
  return not (opt < value);
}

template <class T>
constexpr bool operator <= (optional<T> const& opt, T const& value) noexcept {
  return not (opt > value);
}

template <class T>
constexpr bool operator > (optional<T> const& opt, T const& value) noexcept {
  return opt ? value < *opt : false;
}

template <class T>
constexpr bool operator < (optional<T> const& opt, T const& value) noexcept {
  return opt ? *opt < value : true;
}

/* comparison of expected<T> with T */
template <class T>
bool operator == (expected<T> const& exp, T const& value) noexcept {
  return exp ? *exp == value : false;
}

template <class T>
bool operator != (expected<T> const& exp, T const& value) noexcept {
  return not (*exp == value);
}

template <class T>
bool operator >= (expected<T> const& exp, T const& value) noexcept {
  return not (exp < value);
}

template <class T>
bool operator <= (expected<T> const& exp, T const& value) noexcept {
  return not (exp > value);
}

template <class T>
bool operator > (expected<T> const& exp, T const& value) noexcept {
  return exp ? value < *exp : true;
}

template <class T>
bool operator < (expected<T> const& exp, T const& value) noexcept {
  return exp ? *exp < value : false;
}

/* comparison of T with optional<T> */
template <class T>
constexpr bool operator == (T const& value, optional<T> const& opt) noexcept {
  return opt ? value == *opt : false;
}

template <class T>
constexpr bool operator != (T const& value, optional<T> const& opt) noexcept {
  return opt ? not (value == *opt) : true;
}

template <class T>
constexpr bool operator >= (T const& value, optional<T> const& opt) noexcept {
  return not (value < opt);
}

template <class T>
constexpr bool operator <= (T const& value, optional<T> const& opt) noexcept {
  return not (value > opt);
}

template <class T>
constexpr bool operator > (T const& value, optional<T> const& opt) noexcept {
  return opt ? *opt < value : true;
}

template <class T>
constexpr bool operator < (T const& value, optional<T> const& opt) noexcept {
  return opt ? value < *opt : false;
}

/* make_ functions */
template <class Type>
auto make_optional (Type&& value) -> optional<decay_t<Type>> {
  return optional<decay_t<Type>> { ::core::forward<Type>(value) };
}

template <class T>
auto make_expected (T&& value) -> enable_if_t<
  not ::std::is_base_of< ::std::exception, decay_t<T>>::value,
  expected<decay_t<T>>
> { return expected<T> { ::core::forward<T>(value) }; }

template <class T, class U>
auto make_expected (U&& value) -> enable_if_t<
  ::std::is_base_of< ::std::exception, decay_t<U>>::value,
  T
> {
  return make_expected<T>(
    ::std::make_exception_ptr(::core::forward<U>(value))
  );
}

template <class T>
auto make_expected (::std::exception_ptr error) -> expected<T> {
  return expected<T> { error };
}

template <class T>
void swap (optional<T>& lhs, optional<T>& rhs) noexcept(
  noexcept(lhs.swap(rhs))
) { lhs.swap(rhs); }

template <class T>
void swap (expected<T>& lhs, expected<T>& rhs) noexcept(
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
