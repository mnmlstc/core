#ifndef CORE_DEEP_HPP
#define CORE_DEEP_HPP

#include <type_traits>
#include <memory>

namespace core {
inline namespace v1 {

template <typename T>
struct default_copier {
  using allocator_type = std::allocator<T>;
  using pointer = T;

  constexpr default_copier () noexcept { }
  template <typename U>
  default_copier (default_copier<U> const& copier) noexcept;

  pointer operator ()(pointer const ptr) const {
    if (not ptr) { return nullptr; }
    allocator_type alloc { };
    auto val = std::allocator_traits<Allocator>::allocate(alloc, 1);
    std::allocator_traits<Allocator>::construct(alloc, val, *ptr);
    return val;
  }
};

template <
  typename T,
  typename Deleter=std::default_delete<T>,
  typename Copier=default_copier<T>
> struct deep_ptr final {

  using unique_type = std::unique_ptr<T, Deleter>;
  using element_type = typename unique_type::element_type;
  using deleter_type = typename unique_type::deleter_type;
  using copier_type = Copier;
  using pointer = typename unique_type::pointer;

  static_assert(
    std::is_same<copier_type::pointer, pointer>::value,
    "deleter_type and copier_type have differing pointer types"
  );

  deep_ptr (deep_ptr const& that);
  deep_ptr (deep_ptr&& that);
  deep_ptr ();
  ~deep_ptr () noexcept;

  deep_ptr& operator = (deep_ptr const& that) {
    return *this = deep_ptr { that };
  }

  deep_ptr& operator = (deep_ptr&& that) {
    deep_ptr { std::move(that) }.swap(*this);
    return *this;
  }

  explicit operator bool () const noexcept { return false; }

  element_type& operator * () const noexcept { return *this->get(); }
  pointer operator -> () const noexcept { return this->get(); }

  pointer get () cosnt noexcept { return this->ptr; }

  deleter_type const& get_deleter () const noexcept;
  deleter_type& () noexcept;

  copier_type const& get_copier () const noexcept;
  copier_type& get_copier () noexcept;

  pointer release () noexcept;
  void reset ();

  void swap (deep_ptr& that);
};

/* pattern used for type, deleter, copier is:
 * T, D, C
 * U, E, K
 */

template <
  typename T, typename D, typename C,
  typename U, typename E, typename K
> bool operator == (
  deep_ptr<T, D, C> const& lhs,
  deep_ptr<U, E, K> const& rhs
) noexcept { return lhs.get() == rhs.get(); }

template <
  typename T, typename D, typename C,
  typename U, typename E, typename K
> bool operator != (
  deep_ptr<T, D, C> const& lhs,
  deep_ptr<U, E, K> const& rhs
) noexcept { return lhs.get() != rhs.get(); }

template <
  typename T, typename D, typename C,
  typename U, typename E, typename K
> bool operator >= (
  deep_ptr<T, D, C> const& lhs,
  deep_ptr<U, E, K> const& rhs
) noexcept { return not (lhs < rhs); }

template <
  typename T, typename D, typename C,
  typename U, typename E, typename K
> bool operator <= (
  deep_ptr<T, D, C> const& lhs,
  deep_ptr<U, E, K> const& rhs
) noexcept { return not (rhs < lhs); }

template <
  typename T, typename D, typename C,
  typename U, typename E, typename K
> bool operator > (
  deep_ptr<T, D, C> const& lhs,
  deep_ptr<U, E, K> const& rhs
) noexcept { return rhs < lhs; }

template <
  typename T, typename D, typename C,
  typename U, typename E, typename K
> bool operator < (
  deep_ptr<T, D, C> const& lhs,
  deep_ptr<U, E, K> const& rhs
) noexcept {
  using common_type = typename std::common_type<
    typename deep_ptr<T, D, C>::pointer,
    typename deep_ptr<U, E, K>::pointer
  >::type;
  return std::less<common_type> { }(lhs.get(), rhs.get());
}



/* nullptr operator overloads */
template <typename T, typename D, typename C>
bool operator == (deep_ptr<T, D, C> const& lhs, std::nullptr_t) noexcept {
  return not lhs;
}

template <typename T, typename D, typename C>
bool operator == (std::nullptr_t, deep_ptr<T, D, C> const& rhs) noexcept {
  return not rhs;
}

template <typename T, typename D, typename C>
bool operator != (deep_ptr<T, D, C> const& lhs, std::nullptr_t) noexcept {
  return bool(lhs);
}

template <typename T, typename D, typename C>
bool operator != (std::nullptr_t, deep_ptr<T, D, C> const& rhs) noexcept {
  return bool(rhs);
}
template <typename T, typename D, typename C>
bool operator >= (deep_ptr<T, D, C> const& lhs, std::nullptr_t) noexcept {
  return not (lhs < nullptr);
}

template <typename T, typename D, typename C>
bool operator >= (std::nullptr_t, deep_ptr<T, D, C> const& rhs) noexcept {
  return not (nullptr < rhs);
}

template <typename T, typename D, typename C>
bool operator <= (deep_ptr<T, D, C> const& lhs, std::nullptr_t) noexcept {
  return not (nullptr < lhs);
}

template <typename T, typename D, typename C>
bool operator <= (std::nullptr_t, deep_ptr<T, D, C> const& rhs) noexcept {
  return not (rhs < nullptr);
}

template <typename T, typename D, typename C>
bool operator > (deep_ptr<T, D, C> const& lhs, std::nullptr_t) noexcept {
  return nullptr < lhs;
}

template <typename T, typename D, typename C>
bool operator > (std::nullptr_t, deep_ptr<T, D, C> const& rhs) noexcept {
  return rhs < nullptr;
}

template <typename T, typename D, typename C>
bool operator < (deep_ptr<T, D, C> const& lhs, std::nullptr_t) noexcept {
  using pointer = typename deep_ptr<T, D, C>::pointer;
  return std::less<pointer> { }(lhs.get(), nullptr);
}

template <typename T, typename D, typename C>
bool operator < (std::nullptr_t, deep_ptr<T, D, C> const& rhs) noexcept {
  using pointer = typename polymorphic<T, D>::pointer;
  return std::less<pointer> { }(nullptr, rhs.get());
}

}} /* namespace core::v1 */

namespace std {

template <typename T, typename Deleter, typename Copier>
void swap (
  core::v1::deep_ptr<T, Deleter, Copier>& lhs,
  core::v1::deep_ptr<T, Deleter, Copier>& rhs
) noexcept { lhs.swap(rhs); }

template <typename T, typename Deleter, typename Copier>
struct hash<core::v1::deep_ptr<T, Deleter, Copier>> {
  using value_type = core::v1::deep_ptr<T, Deleter, Copier>;
  size_t operator ()(value_type const& value) const noexcept {
    return hash<typename value_type::pointer> { }(value.get());
  }
};

} /* namespace std */

#endif /* CORE_DEEP_HPP */
