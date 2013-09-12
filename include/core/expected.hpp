#ifndef CORE_EXPECTED_HPP
#define CORE_EXPECTED_HPP

#include <type_traits>
#include <exception>
#include <stdexcept>
#include <utility>
#include <memory>

namespace core {
inline namespace v1 {

struct bad_expected_type : public std::logic_error {
  explicit bad_expected_type (std::string const& what_arg) :
    std::logic_error { what_arg }
  { }

  explicit bad_expected_type (char const* what_arg) noexcept :
    std::logic_error { what_arg }
  { }

  ~bad_expected_type () noexcept { }

  virtual char const* what () const noexcept {
    return std::logic_error::what();
  }
};

template <typename T>
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

  ~expected () { if (this->valid) { this->val.~value_type(); } }

  expected& operator = (expected const& that) {
    expected { that }.swap(*this);
    return *this;
  }

  expected& operator = (expected&& that) {
    expected { std::move(that) }.swap(*this);
    return *this;
  }

  expected& operator = (std::exception_ptr ptr) {
    if (this->valid) { this->val.~value_type(); }
    this->ptr = ptr;
    this->valid = false;
    return *this;
  }

  expected& operator = (value_type const& val) {
    expected { val }.swap(*this);
    return *this;
  }

  expected& operator = (value_type&& val) {
    expected { std::move(val) }.swap(*this);
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

  std::exception_ptr get_ptr () noexcept(false) {
    if (this->valid) { throw bad_expected_type { "expected<T> is valid" }; }
    return this->ptr;
  }

  /* TODO: enable-if value_type is constructible with U */
  template <typename U>
  value_type value_or (U&& val) const& {
    if (not this->valid) { return value_type { std::forward<U>(val) }; }
    return this->val;
  }

  template <typename U>
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

  template <typename E>
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
  expected (expected&& that) noexcept : ptr { std::move(that.ptr) } { }
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

  void swap (expected& that) noexcept { std::swap(this->ptr, that.ptr); }

  template <class E> E expect () const {
    try { this->raise(); }
    catch (E const& e) { return e; }
    catch (...) {
      std::throw_with_nested(bad_expected_type { "unexpected exception" });
    }
  }

  [[noreturn]] void raise () const {
    if (not this->ptr) { throw bad_expected_type { "valid expected<void>" }; }
    std::rethrow_exception(this->ptr);
  }

private:
  std::exception_ptr ptr;
};

template <typename T>
bool operator == (expected<T> const& lhs, expected<T> const& rhs) noexcept {
  if (bool(lhs) and bool(rhs)) { return lhs.value() == rhs.value(); }
  return false;
}

template <typename T>
bool operator == (expected<T> const& lhs, T const& rhs) noexcept {
  if (not lhs) { return false; }
  return lhs.value() == rhs.value();
}

template <typename T>
bool operator == (T const& lhs, expected<T> const& rhs) noexcept {
  if (not rhs) { return false; };
  return lhs == rhs.value();
}

template <typename T>
bool operator < (expected<T> const& lhs, expected<T> const& rhs) noexcept {
  if (not lhs) { return false; }
  if (not rhs) { return true; }
  return std::less<T> { }(lhs.value(), rhs.value());
}

template <typename T>
bool operator < (expected<T> const& lhs, T const& rhs) noexcept {
  if (not lhs) { return true; }
  return std::less<T>{ }(lhs.value(), rhs);
}

template <typename T>
auto make_expected (T&& value) noexcept -> expected<
  typename std::decay<T>::type
> { return expected<typename std::decay<T>::type> { std::forward<T>(value) }; }

template <typename T>
auto make_expected (std::exception_ptr error) noexcept -> expected<T> {
  return expected<T> { error };
}

}} /* namespace core::v1 */

namespace std {

template <typename T>
void swap (core::v1::expected<T>& lhs, core::v1::expected<T>& rhs) noexcept(
  noexcept(lhs.swap(rhs))
) { lhs.swap(rhs); }

} /* namespace std */

#endif /* CORE_EXPECTED_HPP */
