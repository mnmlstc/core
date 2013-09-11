#ifndef CORE_EXPECTED_HPP
#define CORE_EXPECTED_HPP

#include <type_traits>
#include <exception>
#include <stdexcept>
#include <utility>
#include <memory>

namespace core {
inline namespace v1 {

struct bad_expected_type final : public std::logic_error {
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

  using exception_allocator_type = std::allocator<std::exception_ptr>;
  using allocator_type = std::allocator<value_type>;

  explicit expected (std::exception_ptr ptr) noexcept :
    ptr { },
    valid { false }
  { this->ptr = ptr; }

  explicit expected (value_type const& val) :
    val { },
    valid { true }
  { this->val = val; }

  explicit expected (value_type&& val) noexcept :
    val { },
    valid { true }
  { this->val = std::move(val); }

  expected (expected const&);
  expected (expected&& that) noexcept :
    valid { std::move(that.valid) }
  {
    if (this->valid) {
      new (std::addressof(this->val)) value_type { std::move(that.val) };
      return;
    }
    new (std::addressof(this->ptr)) std::exception_ptr { std::move(that.ptr) };
  }

  expected () noexcept :
    val { },
    valid { true }
  { }

  ~expected () {
    if (not this->valid) { return; }
    this->val.~value_type();
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
    if (not this->valid) {
      this->ptr = ptr;
      return *this;
    }
    this->val.~value_type();
    this->valid = false;
    new (std::addressof(this->ptr)) std::exception_ptr { ptr };
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

  value_type const& operator * () const { return this->value(); }
  value_type& operator * () { return this->value(); }

  value_type const& value () const {
    if (not this->valid) { std::rethrow_exception(this->ptr); }
    return this->val;
  }

  value_type& value () {
    if (not this->valid) { std::rethrow_exception(this->ptr); }
    return this->val;
  }

  /* TODO: enable-if value_type is constructible with U */
  template <typename U>
  value_type value_or (U&& val) {
    if (not this->valid) { return value_type { std::forward<U>(val) }; }
    return this->val;
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
  E expect () const {
    try { this->raise(); }
    catch (E const& e) { return e; }
    catch (...) {
      std::throw_with_nested(bad_expected_type { "unexpected exception" });
    }
  }

  void raise () const {
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

  explicit expected (std::exception_ptr error) noexcept : error { error } { }
  expected (expected const& that) noexcept : error { that.error } { }
  expected (expected&& that) noexcept : error { std::move(that.error) } { }
  expected () noexcept : error { nullptr } { }
  ~expected () noexcept { }

  expected& operator = (std::exception_ptr error) noexcept {
    expected { error }.swap(*this);
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

  explicit operator bool () const noexcept { return not this->error; }

  void swap (expected& that) { std::swap(this->error, that.error); }

  template <class E> E expect () const {
    try { this->raise(); }
    catch (E const& e) { return e; }
    catch (...) {
      std::throw_with_nested(bad_expected_type { "unexpected exception" });
    }
  }

  void raise () const {
    if (not this->error) {
      throw bad_expected_type { "expected<void> is valid" };
    }
    std::rethrow_exception(this->error);
  }

private:
  std::exception_ptr error;
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
auto make_expected (T&& value) noexcept -> expected<T>;

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
