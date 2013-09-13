#ifndef CORE_POLYMORPHIC_HPP
#define CORE_POLYMORPHIC_HPP

#include <type_traits>
#include <stdexcept>
#include <typeinfo>
#include <memory>

namespace core {
inline namespace v1 {
namespace impl {

template <typename T, typename D>
std::unique_ptr<T, D> null_poly_copy (std::unique_ptr<T, D> const&) {
  return std::unique_ptr<T, D> { };
}

} /* namespace impl */

template <typename T, typename D, typename U>
std::unique_ptr<T, D> default_poly_copy (std::unique_ptr<T, D> const& ptr) {
  auto value = *dynamic_cast<U*>(ptr.get());
  return std::unique_ptr<T, D> {
    new U { std::move(value) }, ptr.get_deleter()
  };
}

struct bad_polymorphic_reset final : std::logic_error {
  explicit bad_polymorphic_reset (std::string const& what_arg) :
    std::logic_error { what_arg }
  { }

  explicit bad_polymorphic_reset (char const* what_arg) noexcept :
    std::logic_error { what_arg }
  { }

  ~bad_polymorphic_reset () noexcept { }

  virtual char const* what () const noexcept {
    return std::logic_error::what();
  }
};

template <typename T, typename Deleter=std::default_delete<T>>
struct polymorphic final {

  using unique_type = std::unique_ptr<T, Deleter>;
  using element_type = typename unique_type::element_type;
  using deleter_type = typename unique_type::deleter_type;
  using copier_type = unique_type (*)(unique_type const&);
  using pointer = typename unique_type::pointer;


  template <typename U, typename D>
  explicit polymorphic (
    std::unique_ptr<U, D>&& ptr,
    copier_type copier=nullptr
  ) noexcept :
    copier {
      copier
        ? copier
        : default_poly_copy<element_type, deleter_type, U>
    },
    data { std::move(ptr) }
  { }

  template <typename U>
  explicit polymorphic (U* ptr, copier_type copier=nullptr) noexcept :
    polymorphic { ptr, deleter_type { }, copier }
  { }

  template <typename U>
  polymorphic (
    U* ptr,
    deleter_type const& deleter,
    copier_type copier = nullptr
  ) noexcept :
    polymorphic { ptr, deleter_type { deleter }, copier }
  { }

  template <typename U>
  polymorphic (
    U* ptr,
    deleter_type&& deleter,
    copier_type copier = nullptr
  ) noexcept :
    copier {
      copier ? copier : default_poly_copy<element_type, deleter_type, U>
    },
    data { ptr, std::move(deleter) }
  {
    constexpr bool abstract = std::is_abstract<U>::value;
    constexpr bool base = std::is_base_of<element_type, U>::value;

    static_assert(not abstract, "cannot create polymorphic with abstract");
    static_assert(base, "cannot create a polymorphic with non-derived type");
  }

  polymorphic (polymorphic const& that) :
    copier { that.copier },
    data { that.copier(that.data) }
  { }

  polymorphic (polymorphic&& that) noexcept :
    copier { std::move(that.copier) },
    data { std::move(that.data) }
  { that.copier = impl::null_poly_copy<element_type, deleter_type>; }

  polymorphic () noexcept :
    copier { impl::null_poly_copy<element_type, deleter_type> },
    data { }
  { }

  ~polymorphic () noexcept { }

  template <typename U, typename D>
  polymorphic& operator = (std::unique_ptr<U, D>&& ptr) {
    polymorphic { std::move(ptr) }.swap(*this);
    return *this;
  }

  template <typename U>
  polymorphic& operator = (U* ptr) {
    polymorphic { ptr }.swap(*this);
    return *this;
  }

  polymorphic& operator = (polymorphic const& that) {
    polymorphic { that }.swap(*this);
    return *this;
  }

  polymorphic& operator = (polymorphic&& that) {
    polymorphic { std::move(that) }.swap(*this);
    return *this;
  }

  explicit operator bool () const noexcept { return bool(this->data); }

  element_type& operator * () const noexcept { return *this->data; }
  pointer operator -> () const noexcept { return this->data.get(); }

  pointer get () const noexcept { return this->data.get(); }

  deleter_type const& get_deleter () const noexcept {
    return this->data.get_deleter();
  }

  deleter_type& get_deleter () noexcept { return this->data.get_deleter(); }

  copier_type const& get_copier () const noexcept { return this->copier; }
  copier_type& get_copier () noexcept { return this->copier; }

  pointer release () noexcept {
    this->copier = impl::null_poly_copy<element_type, deleter_type>;
    return this->data.release();
  }

  void reset (pointer ptr = nullptr) {
    constexpr auto inval = "cannot reset null polymorphic with valid pointer";
    constexpr auto type = "cannot reset polymorphic with different type";

    if (ptr and not this->data) { throw bad_polymorphic_reset { inval }; }
    if (ptr and typeid(*this->data) != typeid(*ptr)) {
      throw bad_polymorphic_reset { type };
    }

    this->data.reset(ptr);
    if (not ptr) {
      this->copier = impl::null_poly_copy<element_type, deleter_type>;
    }
  }

  void swap (polymorphic& that) noexcept {
    std::swap(this->copier, that.copier);
    std::swap(this->data, that.data);
  }

private:
  static_assert(
    std::is_polymorphic<element_type>::value,
    "Cannot create a polymorphic with a non-polymorphic type"
  );

  copier_type copier { impl::null_poly_copy<element_type, deleter_type> };
  unique_type data;
};

/* Pattern used for type and deleter is:
 * T, D
 * U, E
 */

template <typename T, typename D, typename U, typename E>
bool operator == (polymorphic<T, D> const& lhs, polymorphic<U, E> const& rhs) {
  return lhs.get() == rhs.get();
}

template <typename T, typename D, typename U, typename E>
bool operator != (polymorphic<T, D> const& lhs, polymorphic<U, E> const& rhs) {
  return lhs.get() != rhs.get();
}

template <typename T, typename D, typename U, typename E>
bool operator >= (polymorphic<T, D> const& lhs, polymorphic<U, E> const& rhs) {
  return not (lhs < rhs);
}

template <typename T, typename D, typename U, typename E>
bool operator <= (polymorphic<T, D> const& lhs, polymorphic<U, E> const& rhs) {
  return not (rhs < lhs);
}

template <typename T, typename D, typename U, typename E>
bool operator > (polymorphic<T, D> const& lhs, polymorphic<U, E> const& rhs) {
  return rhs < lhs;
}

template <typename T, typename D, typename U, typename E>
bool operator < (polymorphic<T, D> const& lhs, polymorphic<U, E> const& rhs) {
  using common_type = typename std::common_type<
    typename polymorphic<T, D>::pointer,
    typename polymorphic<U, E>::pointer
  >::type;
  return std::less<common_type> { }(lhs.get(), rhs.get());
}

template <typename T, typename D>
bool operator == (polymorphic<T, D> const& lhs, std::nullptr_t) noexcept {
  return not lhs;
}

template <typename T, typename D>
bool operator == (std::nullptr_t, polymorphic<T, D> const& rhs) noexcept {
  return not rhs;
}

template <typename T, typename D>
bool operator != (polymorphic<T, D> const& lhs, std::nullptr_t) noexcept {
  return bool(lhs);
}

template <typename T, typename D>
bool operator != (std::nullptr_t, polymorphic<T, D> const& rhs) noexcept {
  return bool(rhs);
}

template <typename T, typename D>
bool operator >= (polymorphic<T, D> const& lhs, std::nullptr_t) noexcept {
  return not (lhs < nullptr);
}

template <typename T, typename D>
bool operator >= (std::nullptr_t, polymorphic<T, D> const& rhs) noexcept {
  return not (nullptr < rhs);
}

template <typename T, typename D>
bool operator <= (polymorphic<T, D> const& lhs, std::nullptr_t) noexcept {
  return not (nullptr < lhs);
}

template <typename T, typename D>
bool operator <= (std::nullptr_t, polymorphic<T, D> const& rhs) noexcept {
  return not (rhs < nullptr);
}

template <typename T, typename D>
bool operator > (polymorphic<T, D> const& lhs, std::nullptr_t) noexcept {
  return nullptr < lhs;
}

template <typename T, typename D>
bool operator > (std::nullptr_t, polymorphic<T, D> const& rhs) noexcept {
  return rhs < nullptr;
}

template <typename T, typename D>
bool operator < (polymorphic<T, D> const& lhs, std::nullptr_t) noexcept {
  using pointer = typename polymorphic<T, D>::pointer;
  return std::less<pointer> { }(lhs.get(), nullptr);
}

template <typename T, typename D>
bool operator < (std::nullptr_t, polymorphic<T, D> const& rhs) noexcept {
  using pointer = typename polymorphic<T, D>::pointer;
  return std::less<pointer> { }(nullptr, rhs.get());
}

}} /* namespace core::v1 */

namespace std {

template <typename T, typename Deleter>
void swap (
  core::v1::polymorphic<T, Deleter>& lhs,
  core::v1::polymorphic<T, Deleter>& rhs
) noexcept { lhs.swap(rhs); }

template <typename T, typename Deleter>
struct hash<core::v1::polymorphic<T, Deleter>> {
  using value_type = core::v1::polymorphic<T, Deleter>
  size_t operator ()(value_type const& value) const noexcept {
    return hash<T*> { }(value.get());
  }
};

} /* namespace std */

#endif /* CORE_POLYMORPHIC_HPP */
