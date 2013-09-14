#ifndef CORE_MEMORY_HPP
#define CORE_MEMORY_HPP

#include <type_traits>
#include <stdexcept>
#include <typeinfo>
#include <memory>
#include <tuple>

#include <cstddef>

namespace core {
inline namespace v1 {
namespace impl {

template <typename U>
using remove_extent = typename std::remove_extent<U>::type;

template <typename T>
using add_lvalue_reference = typename std::add_lvalue_reference<T>::type;

template <typename T> using result_of = typename std::result_of<T>::type;

template <typename T>
using remove_reference = typename std::remove_reference<T>::type;

template <typename T>
using deep_lvalue = typename std::conditional<
  std::is_reference<T>::value, T, T const&
>::type;

template <typename T, typename D>
class pointer {
  template <typename U>
  static auto check (typename U::pointer*) noexcept -> typename U::pointer;
  template <typename> static auto check (...) noexcept(false) -> T*;

  using deleter_type = typename std::remove_reference<D>::type;

public:
  static constexpr bool value = noexcept(check<deleter_type>(nullptr));
  using type = decltype(check<deleter_type>(nullptr));
};

} /* namespace impl */

/* poly_ptr copier */
template <typename T, typename D, typename U>
std::unique_ptr<T, D> default_poly_copy (std::unique_ptr<T, D> const& ptr) {
  auto value = *dynamic_cast<U*>(ptr.get());
  auto const& deleter = ptr.get_deleter();
  return std::unique_ptr<T, D> { new U { std::move(value) }, deleter };
}

/* null-state poly_ptr copier (don't copy that poly!) */
template <typename T, typename D>
std::unique_ptr<T, D> null_poly_copy (std::unique_ptr<T, D> const&) noexcept {
  return std::unique_ptr<T, D> { };
}

/* deep_ptr copier */
template <typename T>
struct default_copy {
  using allocator_type = std::allocator<T>;
  using pointer = T*;

  constexpr default_copy () noexcept { }
  template <typename U>
  default_copy (default_copy<U> const& copier) noexcept;

  pointer operator ()(pointer const ptr) const {
    allocator_type alloc { };
    auto val = std::allocator_traits<allocator_type>::allocate(alloc, 1);
    std::allocator_traits<allocator_type>::construct(alloc, val, *ptr);
    return val;
  }
};

struct bad_polymorphic_reset : std::logic_error {
  explicit bad_polymorphic_reset (std::string const& what_arg) :
    std::logic_error { what_arg }
  { }

  explicit bad_polymorphic_reset (char const* what_arg) noexcept :
    std::logic_error { what_arg }
  { }

  virtual ~bad_polymorphic_reset () noexcept { }
};

template <typename T, typename Deleter=std::default_delete<T>>
struct poly_ptr final {
  using unique_type = std::unique_ptr<T, Deleter>;
  using element_type = typename unique_type::element_type;
  using deleter_type = typename unique_type::deleter_type;
  using copier_type = unique_type (*)(unique_type const&);
  using pointer = typename unique_type::pointer;

  template <typename U>
  explicit poly_ptr (U* ptr) noexcept :
    poly_ptr { ptr, deleter_type { } }
  { }

  template <typename U, typename E>
  explicit poly_ptr (
    std::unique_ptr<U, E>&& ptr,
    copier_type copier=default_poly_copy<element_type, deleter_type, U>
  ) noexcept :
    copier { copier },
    ptr { std::move(ptr) }
  {
    constexpr bool abstract = std::is_abstract<U>::value;
    constexpr bool base = std::is_base_of<element_type, U>::value;

    static_assert(not abstract, "cannot create poly_ptr with abstract ptr");
    static_assert(base, "cannot create poly_ptr with non-derived type");
  }

  template <typename U, typename E>
  poly_ptr (
    U* ptr, E&& deleter,
    copier_type copier=default_poly_copy<element_type, deleter_type, U>
  ) noexcept :
    poly_ptr {
      unique_type { std::move(ptr), std::forward<E>(deleter) }, copier
    }
  { }

  poly_ptr (poly_ptr const& that) :
    copier { that.copier },
    ptr { that.copier(that.ptr) }
  { }

  poly_ptr (poly_ptr&& that) noexcept :
    copier { std::move(that.copier) },
    ptr { std::move(that.ptr) }
  { that.copier = null_poly_copy<element_type, deleter_type>; }

  constexpr poly_ptr () noexcept { }

  ~poly_ptr () noexcept { }

  template <typename U, typename E>
  poly_ptr& operator = (std::unique_ptr<U, E>&& ptr) {
    poly_ptr { std::move(ptr) }.swap(*this);
    return *this;
  }

  template <typename U>
  poly_ptr& operator = (U* ptr) {
    poly_ptr { ptr }.swap(*this);
    return *this;
  }

  poly_ptr& operator = (std::nullptr_t) noexcept {
    this->reset();
    return *this;
  }

  poly_ptr& operator = (poly_ptr const& that) {
    return *this = poly_ptr { that };
  }

  poly_ptr& operator = (poly_ptr&& that) noexcept {
    poly_ptr { std::move(that) }.swap(*this);
    return *this;
  }

  explicit operator bool () const noexcept { return bool(this->ptr); }

  /*
  operator unique_type () const& {
    return unique_type {
      this->get_copier()(this->get()),
      this->get_deleter()
    };
  }

  operator unique_type () && noexcept {
    this->get_copier() = null_poly_copy<element_type>;
    return unique_type { std::move(this->ptr) };
  }
  */

  impl::add_lvalue_reference<element_type> operator * () const noexcept {
    return *this->ptr;
  }

  pointer operator -> () const noexcept { return this->ptr.get(); }

  pointer get () const noexcept { return this->ptr.get(); }

  deleter_type const& get_deleter () const noexcept {
    return this->ptr.get_deleter();
  }

  deleter_type& get_deleter () noexcept { return this->ptr.get_deleter(); }

  copier_type const& get_copier () const noexcept { return this->copier; }
  copier_type& get_copier () noexcept { return this->copier; }

  pointer release () noexcept {
    this->copier = null_poly_copy<element_type, deleter_type>;
    return this->ptr.release();
  }

  void reset (pointer ptr = pointer { }) {
    constexpr auto invalid = "cannot reset null poly_ptr with valid pointer";
    constexpr auto type = "cannot reset poly_ptr with different type";

    if (ptr and not this->ptr) { throw bad_polymorphic_reset { invalid }; }
    if (ptr and typeid(*this->ptr) != typeid(*ptr)) {
      throw bad_polymorphic_reset { type };
    }

    this->ptr.reset(ptr);
    if (not ptr) { this->copier = null_poly_copy<element_type, deleter_type>; }
  }

  void swap (poly_ptr& that) noexcept {
    std::swap(this->get_copier(), that.get_copier());
    std::swap(this->ptr, that.ptr);
  }

private:
  static_assert(
    std::is_polymorphic<element_type>::value,
    "cannot create a poly_ptr with a non-polymorphic type"
  );

  copier_type copier { null_poly_copy<element_type, deleter_type> };
  unique_type ptr;
};

template <
  typename T,
  typename Deleter=std::default_delete<T>,
  typename Copier=default_copy<T>
> struct deep_ptr final {

  using element_type = T;
  using deleter_type = Deleter;
  using copier_type = Copier;
  using pointer = typename impl::pointer<element_type, deleter_type>::type;

  static_assert(
    std::is_same<impl::result_of<copier_type(pointer)>, pointer>::value,
    "deleter_type and copier_type have differing pointer types"
  );

  deep_ptr (
    pointer ptr,
    impl::deep_lvalue<deleter_type> deleter,
    impl::deep_lvalue<copier_type> copier
  ) noexcept :
    data { ptr, deleter, copier }
  { }

  deep_ptr (
    pointer ptr,
    impl::remove_reference<deleter_type>&& deleter,
    impl::remove_reference<copier_type>&& copier
  ) noexcept :
    data { std::move(ptr), std::move(deleter), std::move(copier) }
  { }

  template <typename U, typename E>
  deep_ptr (std::unique_ptr<U, E>&& that) noexcept :
    deep_ptr { that.release(), std::move(that.get_deleter()), copier_type { } }
  { }

  explicit deep_ptr (pointer ptr) noexcept :
    deep_ptr { ptr, deleter_type { }, copier_type { } }
  { }

  constexpr deep_ptr (std::nullptr_t) noexcept : deep_ptr { } { }

  deep_ptr (deep_ptr const& that) :
    deep_ptr {
      that.get() ? that.get_copier()(that.get()) : that.get(),
      that.get_deleter(),
      that.get_copier()
    }
  { }

  deep_ptr (deep_ptr&& that) noexcept :
    data {
      that.release(),
      std::move(that.get_deleter()),
      std::move(that.get_copier())
    }
  { }

  constexpr deep_ptr () noexcept : data { } { }

  ~deep_ptr () noexcept {
    auto& ptr = std::get<0>(this->data);
    if (not ptr) { return; }
    this->get_deleter()(ptr);
    ptr = nullptr;
  }

  deep_ptr& operator = (std::nullptr_t) noexcept {
    this->reset();
    return *this;
  }

  deep_ptr& operator = (deep_ptr const& that) {
    return *this = deep_ptr { that };
  }

  deep_ptr& operator = (deep_ptr&& that) noexcept {
    deep_ptr { std::move(that) }.swap(*this);
    return *this;
  }

  explicit operator bool () const noexcept { return this->get(); }

  impl::add_lvalue_reference<element_type> operator * () const noexcept {
    return *this->get();
  }
  pointer operator -> () const noexcept { return this->get(); }
  pointer get () const noexcept { return std::get<0>(this->data); }

  deleter_type const& get_deleter () const noexcept {
    return std::get<1>(this->data);
  }

  deleter_type& get_deleter () noexcept { return std::get<1>(this->data); }

  copier_type const& get_copier () const noexcept {
    return std::get<2>(this->data);
  }

  copier_type& get_copier () noexcept { return std::get<2>(this->data); }

  pointer release () noexcept {
    auto ptr = this->get();
    std::get<0>(this->data) = nullptr;
    return ptr;
  }

  void reset (pointer ptr = pointer { }) noexcept {
    std::swap(std::get<0>(this->data), ptr);
    if (not ptr) { return; }
    this->get_deleter()(ptr);
  }

  void swap (deep_ptr& that) noexcept { std::swap(this->data, that.data); }

private:
  using data_type = std::tuple<pointer, deleter_type, copier_type>;
  data_type data;
};

/* poly_ptr convention for type and deleter is:
 * T, D : U, E
 */
template <typename T, typename D, typename U, typename E>
bool operator == (
  poly_ptr<T, D> const& lhs,
  poly_ptr<U, E> const& rhs
) noexcept { return lhs.get() == rhs.get(); }

template <typename T, typename D, typename U, typename E>
bool operator != (
  poly_ptr<T, D> const& lhs,
  poly_ptr<U, E> const& rhs
) noexcept { return lhs.get() != rhs.get(); }

template <typename T, typename D, typename U, typename E>
bool operator >= (
  poly_ptr<T, D> const& lhs,
  poly_ptr<U, E> const& rhs
) noexcept { return not (lhs < rhs); }

template <typename T, typename D, typename U, typename E>
bool operator <= (
  poly_ptr<T, D> const& lhs,
  poly_ptr<U, E> const& rhs
) noexcept { return not (rhs < lhs); }

template <typename T, typename D, typename U, typename E>
bool operator > (
  poly_ptr<T, D> const& lhs,
  poly_ptr<U, E> const& rhs
) noexcept { return rhs < lhs; }

template <typename T, typename D, typename U, typename E>
bool operator < (
  poly_ptr<T, D> const& lhs,
  poly_ptr<U, E> const& rhs
) noexcept {
  using common_type = typename std::common_type<
    typename poly_ptr<T, D>::pointer,
    typename poly_ptr<U, E>::pointer
  >::type;
  return std::less<common_type> { }(lhs.get(), rhs.get());
}

/* deep_ptr convention for type, deleter, copier is
 * T, D, C : U, E, K
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

/* poly_ptr nullptr operator overloads */
template <typename T, typename D>
bool operator == (poly_ptr<T, D> const& lhs, std::nullptr_t) noexcept {
  return not lhs;
}

template <typename T, typename D>
bool operator == (std::nullptr_t, poly_ptr<T, D> const& rhs) noexcept {
  return not rhs;
}

template <typename T, typename D>
bool operator != (poly_ptr<T, D> const& lhs, std::nullptr_t) noexcept {
  return bool(lhs);
}

template <typename T, typename D>
bool operator != (std::nullptr_t, poly_ptr<T, D> const& rhs) noexcept {
  return bool(rhs);
}

template <typename T, typename D>
bool operator >= (poly_ptr<T, D> const& lhs, std::nullptr_t) noexcept {
  return not (lhs < nullptr);
}

template <typename T, typename D>
bool operator >= (std::nullptr_t, poly_ptr<T, D> const& rhs) noexcept {
  return not (nullptr < rhs);
}

template <typename T, typename D>
bool operator <= (poly_ptr<T, D> const& lhs, std::nullptr_t) noexcept {
  return not (nullptr < lhs);
}

template <typename T, typename D>
bool operator <= (std::nullptr_t, poly_ptr<T, D> const& rhs) noexcept {
  return not (rhs < nullptr);
}

template <typename T, typename D>
bool operator > (poly_ptr<T, D> const& lhs, std::nullptr_t) noexcept {
  return nullptr < lhs;
}

template <typename T, typename D>
bool operator > (std::nullptr_t, poly_ptr<T, D> const& rhs) noexcept {
  return rhs < nullptr;
}

template <typename T, typename D>
bool operator < (poly_ptr<T, D> const& lhs, std::nullptr_t) noexcept {
  using pointer = typename poly_ptr<T, D>::pointer;
  return std::less<pointer> { }(lhs.get(), nullptr);
}

template <typename T, typename D>
bool operator < (std::nullptr_t, poly_ptr<T, D> const& rhs) noexcept {
  using pointer = typename poly_ptr<T, D>::pointer;
  return std::less<pointer> { }(nullptr, rhs.get());
}

/* deep_ptr nullptr operator overloads */
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
  using pointer = typename deep_ptr<T, D, C>::pointer;
  return std::less<pointer> { }(nullptr, rhs.get());
}

/* make_poly */
template <
  typename T,
  typename U,
  typename=typename std::enable_if<
    std::is_polymorphic<T>::value and std::is_base_of<T, U>::value
  >::type
> auto make_poly (U&& value) -> poly_ptr<T> {
  return poly_ptr<T> { new U { std::forward<U>(value) } };
}

/* make_unique */
template <
  typename Type,
  typename=typename std::enable_if<not std::is_array<Type>::value>::type,
  typename... Args
> auto make_unique(Args&&... args) -> std::unique_ptr<Type> {
  return std::unique_ptr<Type> { new Type(std::forward<Args>(args)...) };
}

template <
  typename Type,
  typename=typename std::enable_if<std::is_array<Type>::value>::type,
  typename=typename std::enable_if<not std::extent<Type>::value>::type
> auto make_unique(std::size_t size) -> std::unique_ptr<Type> {
  return std::unique_ptr<Type> { new impl::remove_extent<Type>[size] { } };
}

template <
  typename Type,
  typename=typename std::enable_if<std::is_array<Type>::value>::type,
  typename=typename std::enable_if<std::extent<Type>::value>::type,
  typename... Args
> auto make_unique(Args&&...) -> void = delete;

}} /* namespace core::v1 */

namespace std {

template <typename T, typename D>
void swap (
  core::v1::poly_ptr<T, D>& lhs,
  core::v1::poly_ptr<T, D>& rhs
) noexcept { lhs.swap(rhs); }

template <typename T, typename Deleter, typename Copier>
void swap (
  core::v1::deep_ptr<T, Deleter, Copier>& lhs,
  core::v1::deep_ptr<T, Deleter, Copier>& rhs
) noexcept { lhs.swap(rhs); }

template <typename T, typename D>
struct hash<core::v1::poly_ptr<T, D>> {
  using value_type = core::v1::poly_ptr<T, D>;
  size_t operator ()(value_type const& value) const noexcept {
    return hash<typename value_type::pointer>{ }(value.get());
  }
};

template <typename T, typename Deleter, typename Copier>
struct hash<core::v1::deep_ptr<T, Deleter, Copier>> {
  using value_type = core::v1::deep_ptr<T, Deleter, Copier>;
  size_t operator ()(value_type const& value) const noexcept {
    return hash<typename value_type::pointer> { }(value.get());
  }
};

} /* namespace std */

#endif /* CORE_MEMORY_HPP */
