#ifndef CORE_OPTIONAL_HPP
#define CORE_OPTIONAL_HPP

#include <initializer_list>
#include <functional>
#include <stdexcept>
#include <utility>
#include <memory>

#include <core/type_traits.hpp>

namespace core {
inline namespace v1 {

struct in_place_t final { };
struct nullopt_t final { };

constexpr in_place_t in_place { };
constexpr nullopt_t nullopt { };

struct bad_optional_access final : public std::logic_error {
  using std::logic_error::logic_error;
};

template <class Type>
struct optional final {
  static_assert(
    not std::is_same<decay_t<Type>, nullopt_t>::value,
    "Cannot have an optional<nullopt_t>"
  );

  static_assert(
    not std::is_same<decay_t<Type>, in_place_t>::value,
    "Cannot have an optional<in_place_t>"
  );

  static_assert(
    not std::is_same<decay_t<Type>, std::nullptr_t>::value,
    "Cannot have an optional<std::nullptr_t>"
  );

  static_assert(
    not std::is_same<decay_t<Type>, void>::value,
    "Cannot have an optional<void>"
  );

  using value_type = Type;
  using allocator_type = std::allocator<value_type>;

  optional (optional const& that) :
    engaged { that.engaged }
  {
    if (not this->engaged) { return; }
    allocator_type alloc { };
    std::allocator_traits<allocator_type>::construct(
      alloc,
      reinterpret_cast<value_type*>(std::addressof(this->data)),
      *that
    );
  }

  optional (optional&& that)
  noexcept(std::is_nothrow_move_constructible<value_type>::value) :
    engaged { std::move(that.engaged) }
  {
    if (not this->engaged) { return; }
    allocator_type alloc { };
    std::allocator_traits<allocator_type>::construct(
      alloc,
      reinterpret_cast<value_type*>(std::addressof(this->data)),
      std::move(*that)
    );
    this->engaged = true;
    that.engaged = false;
  }

  constexpr optional () noexcept : data { }, engaged { false } { }
  constexpr optional (nullopt_t) noexcept : optional { } { }

  optional (value_type const& value) : data { }, engaged { false } {
    allocator_type alloc { };
    std::allocator_traits<allocator_type>::construct(
      alloc,
      reinterpret_cast<value_type*>(std::addressof(this->data)),
      value
    );
    this->engaged = true;
  }

  optional (value_type&& value) noexcept(
    std::is_nothrow_move_constructible<value_type>::value
  ) : data { }, engaged { false } {
    allocator_type alloc { };
    std::allocator_traits<allocator_type>::construct(
      alloc,
      reinterpret_cast<value_type*>(std::addressof(this->data)),
      std::move(value)
    );
    this->engaged = true;
  }

  template <class... Args>
  explicit optional (in_place_t, Args&&... args) :
    data { },
    engaged { false }
  {
    allocator_type alloc { };
    std::allocator_traits<allocator_type>::construct(
      alloc,
      reinterpret_cast<value_type*>(std::addressof(this->data)),
      std::forward<Args>(args)...
    );
    this->engaged = true;
  }

  template <class T, class... Args>
  explicit optional (
    in_place_t,
    std::initializer_list<T> list,
    Args&&... args
  ) : data { }, engaged { false } {
    allocator_type alloc { };
    std::allocator_traits<allocator_type>::construct(
      alloc,
      reinterpret_cast<value_type*>(std::addressof(this->data)),
      list,
      std::forward<Args>(args)...
    );
    this->engaged = true;
  }

  ~optional () noexcept {
    if (not this->engaged) { return; }
    allocator_type alloc { };
    std::allocator_traits<allocator_type>::destroy(
      alloc, reinterpret_cast<value_type*>(std::addressof(this->data))
    );
  }

  optional& operator = (optional const& that) {
    optional { that }.swap(*this);
    return *this;
  }

  optional& operator = (optional&& that) noexcept (
    std::is_nothrow_move_assignable<value_type>::value and
    std::is_nothrow_move_constructible<value_type>::value
  ) {
    optional { std::move(that) }.swap(*this);
    return *this;
  }

  template <
    class T,
    class=enable_if_t<
      std::is_constructible<value_type, T>::value and
      std::is_assignable<value_type, T>::value
    >
  > optional& operator = (T&& value) {
    if (not this->engaged) { this->emplace(std::forward<T>(value)); }
    else { **this = std::forward<T>(value); }
    return *this;
  }

  optional& operator = (nullopt_t) noexcept {
    if (not this->engaged) { return *this; }
    allocator_type alloc { };
    std::allocator_traits<allocator_type>::destroy(
      alloc, reinterpret_cast<value_type*>(std::addressof(this->data))
    );
    this->engaged = false;
    return *this;
  }

  value_type const* operator -> () const {
    return reinterpret_cast<value_type const*>(std::addressof(this->data));
  }

  value_type* operator -> () {
    return reinterpret_cast<value_type*>(std::addressof(this->data));
  }

  value_type const& operator * () const {
    return reinterpret_cast<value_type const&>(this->data);
  }

  value_type& operator * () {
    return reinterpret_cast<value_type&>(this->data);
  }

  explicit operator bool () const { return this->engaged; }

  value_type const& value () const {
    if (this->engaged) { return *(*this); }
    throw bad_optional_access { "optional is disengaged" };
  }

  value_type& value () {
    if (this->engaged) { return *(*this); }
    throw bad_optional_access { "optional is disengaged" };
  }

  template <class T>
  value_type value_or (T&& val) const& {
    if (not this->engaged) { return value_type { std::forward<T>(val) }; }
    return **this;
  }

  template <class T>
  value_type value_or (T&& val) && {
    if (not this->engaged) { return value_type { std::forward<T>(val) }; }
    return value_type { std::move(**this) };
  }

  void swap (optional& that) noexcept (
    std::is_nothrow_move_constructible<value_type>::value and
    noexcept(
      std::swap(std::declval<value_type&>(), std::declval<value_type&>())
    )
  ) {
    if (not this->engaged and not that.engaged) { return; }
    if (this->engaged and that.engaged) {
      std::swap(**this, *that);
      return;
    }

    auto& to_disengage = this->engaged ? *this : that;
    auto& to_engage = this->engaged ? that : *this;

    to_engage.emplace(std::move(*to_disengage));
    to_disengage = nullopt;
  }

  template <class U, class... Args>
  void emplace (std::initializer_list<U> list, Args&&... args) {
    if (this->engaged) { *this = nullopt; }

    allocator_type alloc { };
    std::allocator_traits<allocator_type>::construct(
      alloc,
      reinterpret_cast<value_type*>(std::addressof(this->data)),
      list,
      std::forward<Args>(args)...
    );
    this->engaged = true;
  }

  template <class... Args>
  void emplace (Args&&... args) {
    if (this->engaged) { *this = nullopt; }

    allocator_type alloc { };
    std::allocator_traits<allocator_type>::construct(
      alloc,
      reinterpret_cast<value_type*>(std::addressof(this->data)),
      std::forward<Args>(args)...
    );
    this->engaged = true;
  }

private:
  using data_type = aligned_storage_t<
    sizeof(value_type),
    std::alignment_of<value_type>::value
  >;

  data_type data;
  bool engaged;
};


template <class Type>
auto make_optional (Type&& value) -> optional<decay_t<Type>> {
  return optional<decay_t<Type>>(std::forward<Type>(value));
}

template <class T>
bool operator == (optional<T> const& lhs, optional<T> const& rhs) noexcept {
  if (bool(lhs) != bool(rhs)) { return false; }
  if (not lhs and not rhs) { return true; }
  return *lhs == *rhs;
}

template <class T>
bool operator < (optional<T> const& lhs, optional<T> const& rhs) noexcept {
  if (bool(rhs) == false) { return false; }
  if (bool(lhs) == false) { return true; }
  return std::less<T> { }(*lhs, *rhs);
}

template <class T>
bool operator == (optional<T> const& lhs, nullopt_t) noexcept {
  return not lhs;
}

template <class T>
bool operator == (nullopt_t, optional<T> const&) noexcept { return false; }

template <class T>
bool operator < (optional<T> const& lhs, nullopt_t) noexcept {
  return not lhs;
}

template <class T>
bool operator < (nullopt_t, optional<T> const& rhs) noexcept {
  return bool(rhs);
}

template <class T>
bool operator == (optional<T> const& opt, T const& value) noexcept {
  return bool(opt) ? *opt == value : false;
}

template <class T>
bool operator == (T const& value, optional<T> const& opt) noexcept {
  return bool(opt) ? value == *opt : false;
}

template <class T>
bool operator < (optional<T> const& opt, T const& value) noexcept {
  return bool(opt) ? std::less<T>{ }(*opt, value) : true;
}

}} /* namespace core::v1 */

namespace std {

template <class Type>
void swap (core::optional<Type>& lhs, core::optional<Type>& rhs) noexcept(
  noexcept(lhs.swap(rhs))
) { lhs.swap(rhs); }

template <class Type>
struct hash<core::optional<Type>> {
  using result_type = typename hash<Type>::result_type;

  result_type operator () (core::optional<Type> const& value) const {
    return value ? hash<Type> { }(*value) : result_type { };
  }
};

} /* namespace std */

#endif /* CORE_OPTIONAL_HPP */
