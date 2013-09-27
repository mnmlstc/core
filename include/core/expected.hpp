#ifndef CORE_EXPECTED_HPP
#define CORE_EXPECTED_HPP

#include <exception>
#include <stdexcept>
#include <utility>
#include <memory>

#include <core/type_traits.hpp>

namespace core {
inline namespace v1 {

struct bad_expected_type : public std::logic_error {
  using std::logic_error::logic_error;
};

template <class T>
struct expected final {
  using value_type = T;

  explicit expected (std::exception_ptr ptr) noexcept :
    ptr { },
    valid { false }
  { this->ptr = ptr; }

  explicit expected (value_type const& val) :
    expected { value_type { val } }
  { }

  explicit expected (value_type&& val) noexcept :
    val { },
    valid { true }
  { this->val = std::move(val); }

  expected (expected const& that) :
    valid { that.valid }
  {
    if (this->valid) {
      new (std::addressof(this->val)) value_type { that.val };
    } else { new (std::addressof(this->ptr)) std::exception_ptr { that.ptr }; }
  }

  expected (expected&& that) noexcept :
    valid { that.valid }
  {
    if (this->valid) {
      new (std::addressof(this->val)) value_type { std::move(that.val) };
    } else { new (std::addressof(this->ptr)) std::exception_ptr { that.ptr }; }
  }

  expected () noexcept :
    val { },
    valid { true }
  { }

  ~expected () {
    this->valid ? this->val.~value_type() : this->ptr.~exception_ptr();
  }

  expected& operator = (expected const& that) {
    expected { that }.swap(*this);
    return *this;
  }

  expected& operator = (expected&& that) {
    expected { std::move(that) }.swap(*this);
    return *this;
  }

  expected& operator = (std::exception_ptr ptr) {
    if (not this->valid) { this->ptr = ptr; }
    else {
      this->val.~value_type();
      new (std::addressof(this->ptr)) std::exception_ptr { ptr };
      this->valid = false;
    }
    return *this;
  }

  expected& operator = (value_type const& val) {
    return *this = value_type { val };
  }

  expected& operator = (value_type&& val) {
    if (this->valid) { this->val = std::move(val); }
    else {
      this->ptr.~exception_ptr();
      new (std::addressof(this->val)) value_type { std::move(val) };
      this->valid = true;
    }
    return *this;
  }

  explicit operator bool () const noexcept { return this->valid; }

  value_type const& operator * () const noexcept(false) {
    return this->value();
  }

  value_type& operator * () noexcept(false) { return this->value(); }

  value_type const& value () const noexcept(false) {
    if (not this->valid) { std::rethrow_exception(this->ptr); }
    return this->val;
  }

  value_type& value () noexcept(false) {
    if (not this->valid) { std::rethrow_exception(this->ptr); }
    return this->val;
  }

  std::exception_ptr get_ptr () const noexcept(false) {
    if (this->valid) { throw bad_expected_type { "expected<T> is valid" }; }
    return this->ptr;
  }

  template <class U>
  value_type value_or (U&& val) const& {
    if (not this->valid) { return value_type { std::forward<U>(val) }; }
    return this->val;
  }

  template <class U>
  value_type value_or (U&& val) && {
    if (not this->valid) { return value_type { std::forward<U>(val) }; }
    return value_type { std::move(this->val) };
  }

  void swap (expected& that) noexcept(
    std::is_nothrow_move_constructible<value_type>::value and
    noexcept(
      std::swap(std::declval<value_type&>(), std::declval<value_type&>())
    )
  ) {
    if (not this->valid and not that.valid) {
      std::swap(this->ptr, that.ptr);
      return;
    }

    if (this->valid and that.valid) {
      std::swap(this->val, that.val);
      return;
    }

    auto& to_invalidate = this->valid ? *this : that;
    auto& to_validate = this->valid ? that : *this;
    auto ptr = to_validate.ptr;
    to_validate = std::move(to_invalidate.value());
    to_invalidate = ptr;
  }

  template <class E>
  E expect () const noexcept(false) {
    try { this->raise(); }
    catch (E const& e) { return e; }
    catch (...) {
      std::throw_with_nested(bad_expected_type { "unexpected exception" });
    }
  }

  [[noreturn]] void raise () const noexcept(false) {
    if (this->valid) { throw bad_expected_type { "expected<T> is valid" }; }
    std::rethrow_exception(this->ptr);
  }

private:

  union {
    std::exception_ptr ptr;
    value_type val;
  };

  bool valid;
};

template <>
struct expected<void> final {
  using value_type = void;

  explicit expected (std::exception_ptr ptr) noexcept : ptr { ptr } { }
  expected (expected const& that) noexcept : ptr { that.ptr } { }

  expected (expected&& that) noexcept :
    ptr { std::move(that.ptr) }
  { that.ptr = nullptr; }

  expected () noexcept : ptr { nullptr } { }
  ~expected () noexcept { }

  expected& operator = (std::exception_ptr ptr) noexcept {
    expected { ptr }.swap(*this);
    return *this;
  }

  expected& operator = (expected const& that) noexcept {
    expected { that }.swap(*this);
    return *this;
  }

  expected& operator = (expected&& that) noexcept {
    expected { std::move(that) }.swap(*this);
    return *this;
  }

  explicit operator bool () const noexcept { return not this->ptr; }

  std::exception_ptr get_ptr () const noexcept { return this->ptr; }
  void swap (expected& that) noexcept { std::swap(this->ptr, that.ptr); }

  template <class E> E expect () const noexcept(false) {
    try { this->raise(); }
    catch (E const& e) { return e; }
    catch (...) {
      std::throw_with_nested(bad_expected_type { "unexpected exception" });
    }
  }

  [[noreturn]] void raise () const noexcept(false) {
    if (not this->ptr) { throw bad_expected_type { "valid expected<void>" }; }
    std::rethrow_exception(this->ptr);
  }

private:
  std::exception_ptr ptr;
};

template <class T>
bool operator == (expected<T> const& lhs, expected<T> const& rhs) noexcept {
  if (bool(lhs) and bool(rhs)) { return lhs.value() == rhs.value(); }
  return false;
}

template <class T>
bool operator == (expected<T> const& lhs, std::exception_ptr rhs) {
  if (bool(lhs)) { return false; }
  return lhs.get_ptr() == rhs;
}

template <class T>
bool operator == (std::exception_ptr lhs, expected<T> const& rhs) {
  if (bool(rhs)) { return false; }
  return lhs == rhs.get_ptr();
}

template <class T>
bool operator == (expected<T> const& lhs, T const& rhs) noexcept {
  if (not lhs) { return false; }
  return lhs.value() == rhs;
}

template <class T>
bool operator == (T const& lhs, expected<T> const& rhs) noexcept {
  if (not rhs) { return false; };
  return lhs == rhs.value();
}

template <class T>
bool operator < (expected<T> const& lhs, expected<T> const& rhs) noexcept {
  if (not lhs) { return false; }
  if (not rhs) { return true; }
  return std::less<T> { }(lhs.value(), rhs.value());
}

template <class T>
bool operator < (expected<T> const& lhs, T const& rhs) noexcept {
  if (not lhs) { return true; }
  return std::less<T>{ }(lhs.value(), rhs);
}

/* expected<void> specialization */
template <>
bool operator == <void>(
  expected<void> const& lhs,
  expected<void> const& rhs
) noexcept { return lhs.get_ptr() == rhs.get_ptr(); }

template <class T>
auto make_expected (T&& value) noexcept -> expected<decay_t<T>> {
  return expected<decay_t<T>> { std::forward<T>(value) };
}

template <class T>
auto make_expected (std::exception_ptr error) noexcept -> expected<T> {
  return expected<T> { error };
}

}} /* namespace core::v1 */

namespace std {

template <class T>
void swap (core::v1::expected<T>& lhs, core::v1::expected<T>& rhs) noexcept(
  noexcept(lhs.swap(rhs))
) { lhs.swap(rhs); }

} /* namespace std */

#endif /* CORE_EXPECTED_HPP */
