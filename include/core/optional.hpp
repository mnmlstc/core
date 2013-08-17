#ifndef CORE_OPTIONAL_HPP
#define CORE_OPTIONAL_HPP

#include <initializer_list>
#include <type_traits>
#include <stdexcept>
#include <utility>
#include <memory>

namespace core {
inline namespace v1 {

struct in_place_t final { };
struct nullopt_t final { };

constexpr in_place_t in_place { };
constexpr nullopt_t nullopt { };

class bad_optional_access final : public std::logic_error {
public:
  explicit bad_optional_access (std::string const& what_arg) :
    std::logic_error { what_arg }
  { }
  explicit bad_optional_access (char const* what_arg) noexcept :
    std::logic_error { what_arg }
  { }

  ~bad_optional_access () noexcept { }

  virtual char const* what () const { return std::logic_error::what(); }
};

template <typename Type>
class optional final {
  static_assert(
    not std::is_same<typename std::decay<Type>::type, nullopt_t>::value,
    "Cannot have an optional<nullopt_t>"
  );

  static_assert(
    not std::is_same<typename std::decay<Type>::type, in_place_t>::value,
    "Cannot have an optional<in_place_t>"
  );

  using value_type = Type;
  using allocator_type = std::allocator<value_type>;

  using nothrow_move_construct = std::is_nothrow_move_constructible<value_type>;

  typename std::aligned_storage<
    sizeof(value_type),
    std::alignment_of<value_type>::value
  > data;
  bool engaged;

public:
  optional (optional const& that) :
    engaged { that.engaged }
  {
    if (not this->engaged) { return; }
    allocator_type { };
    std::allocator_traits<allocator_type>::construct(
      alloc,
      reinterpret_cast<value_type*>(this->data),
      *that
    );
  }

  optional (optional&& that)
  noexcept(std::is_nothrow_move_constructible<value_type>::value) :
    enaged { std::move(that.engaged) }
  {
    if (not this->enaged) { return; }
    allocator_type { };
    std::allocator_traits<allocator_type>::construct(
      alloc,
      reinterpret_cast<value_type*>(this->data),
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
      reinterpret_cast<value_type*>(this->data),
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
      reinterpret_cast<value_type*>(this->data)
      std::move(value)
    );
    this->engaged = true;
  }

  template <typename... Args>
  explicit optional (in_place_t, Args&&... args) :
    data { },
    engaged { false }
  {
    allocator_type alloc { };
    std::allocator_traits<allocator_type>::construct(
      alloc,
      reinterpret_cast<value_type*>(this->data),
      std::forward<Args>(args)...
    );
    this->engaged = true;
  }

  template <typename T, typename... Args>
  explicit optional (
    in_place_t,
    std::initializer_list<T> list,
    Args&&... args
  ) : data { }, engaged { false } {
    allocator_type alloc { };
    std::allocator_traits<allocator_type>::construct(
      alloc,
      reinterpret_cast<value_type*>(this->data),
      list,
      std::forward<Args>(args)...
    );
    this->engaged = true;
  }

  ~optional () noexcept {
    if (not this->engaged) { return; }
    allocator_type alloc { };
    std::allocator_traits<allocator_type>::destroy(
      alloc, reinterpret_cast<value_type*>(this->data)
    );
  }

  optional& operator = (optional const& that) {
    allocator_type alloc { };
    if (not this->engaged and not that.engaged) { return *this; }
    if (this->engaged and not that.engaged) {
      std::allocator_traits<allocator_type>::destroy(
        alloc, reinterpret_cast<value_type*>(this->data)
      );
      this->engaged = false;
    }

    std::allocator_traits<allocator_type>::construct(
      alloc,
      reinterpret_cast<value_type*>(this->data), 
      *that
    );
    this->engaged = true;

    return *this;
  }

  optional& operator = (optional&& that) noexcept (
    std::is_nothrow_move_assignable<value_type>::value and
    std::is_nothrow_move_constructible<value_type>::value
  ) {
    allocator_type alloc { };
    if (not this->engaged and not that.engaged) { return *this; }
    if (this->engaged and not that.engaged)
  }

  template <typename T>
  optional& operator = (T&& value) {
    if (not this->engaged) {
      allocator_type alloc { };
      std::allocator_traits<allocator_type>::construct(
        alloc,
        reinterpret_cast<value_type*>(this->data),
        std::forward<T>(value)
      );
      this->engaged = true;
    } else { **this = std::forward<T>(value); }
    return *this;
  }

  optional& operator = (nullopt_t) noexcept {
    if (not this->engaged) { return *this; }
    allocator_type alloc { };
    std::allocator_traits<allocator_type>::destroy(
      alloc, reinterpret_cast<value_type*>(this->data)
    );
    this->engaged = false;
    return *this;
  }

  value_type const* operator -> () const {
    return reinterpret_cast<value_type const*>(this->data);
  }

  value_type* operator -> () {
    return reinterpret_cast<value_type*>(this->data);
  }

  value_type const& operator * () const {
    return *reinterpret_cast<value_type const*>(this->data);
  }

  value_type& operator * () {
    return *reinterpret_cast<value_type*>(this->data);
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

  template <typename T>
  value_type value_or (T&& val) {
    if (not this->engaged) { return value_type { std::forward<T>(val) }; }
    return *this;
  }

  void swap (optional& that) noexcept (
    std::is_nothrow_move_constructible<value_type>::value and
    noexcept(std::swap(std::declval<value_type>(), std::declval<value_type>()))
  ) {
    if (not this->engaged and not that.engaged) { return; }
    if (this->engaged and that.engaged) { std::swap(**this, *that); return; }
    if (this->engaged and not that.engaged) {
      allocator_type alloc { };
    }
    std::swap(this->engaged, that.engaged);
  }

  template <typename U, typename... Args>
  void emplace (std::initializer_list<U> list, Args&&... args) {
    allocator_type alloc { };
    if (this->engaged) {
      std::allocator_traits<allocator_type>::destruct(
        alloc,
        reinterpret_cast<value_type*>(this->data)
      );
      this->engaged = false;
    }

    std::allocator_traits<allocator_type>::construct(
      alloc,
      reinterpret_cast<value_type*>(this->data),
      list,
      std::forward<Args>(args)...
    );
  }

  template <typename... Args>
  void emplace (Args&&... args) {
    allocator_type alloc { };
    if (this->engaged) {
      std::allocator_traits<allocator_type>::destruct(
        alloc,
        reinterpret_cast<value_type*>(this->data)
      );
      this->engaged = false;
    }

    std::allocator_traits<allocator_type>::construct(
      alloc,
      reinterpret_cast<value_type*>(this->data),
      std::forward<Args>(args)...
    );
    this->engaged = true;
  }
};

template <typename Type>
auto make_optional (Type&& value) -> optional<typename std::decay<Type>::type> {
  return optional<typename std::decay<Type>::type>(std::forward<Type>(value));
}

}} /* namespace core::v1 */

namespace std {

template <typename Type>
void swap (core::optional<Type>& lhs, core::optional<Type>& rhs) noexcept(
  noexcept(lhs.swap(rhs))
) { lhs.swap(rhs); }

template <typename Type>
struct hash<core::optional<Type>> {
  using result_type = typename hash<Type>::result_type;

  result_type operator () (core::optional<Type> const& value) {
    return value ? hash<Type> { }(*value) : result_type { };
  }
};

} /* namespace std */

#endif /* CORE_OPTIONAL_HPP */
