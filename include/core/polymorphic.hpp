#ifndef CORE_POLYMORPHIC_HPP
#define CORE_POLYMORPHIC_HPP

#include <type_traits>
#include <stdexcept>
#include <typeinfo>
#include <memory>

#include <core/memory.hpp>

namespace core {
inline namespace v1 {
namespace impl {


template <typename Type> using decay = typename std::decay<Type>::type;
template <typename Type>
using lvalue = typename std::add_lvalue_reference<Type>::type;

template <class T, class U>
auto copy (std::unique_ptr<T> const& ptr) -> std::unique_ptr<T> {
  return core::make_unique<U>(*dynamic_cast<U*>(ptr.get()));
}

template <typename T>
auto null_copy (std::unique_ptr<T> const& ptr) -> std::unique_ptr<T> {
  return std::unique_ptr<T> { };
}

} /* namespace impl */

/* Can be simplified down to a std::unique_ptr<Type> and a const copy function ptr */
template <class Type>
class polymorphic final {
  using unique_type = std::unique_ptr<Type>;
  using element_type = typename unique_type::element_type;
  using deleter_type = typename unique_type::deleter_type;
  using copier_type = unique_type (*)(unique_type const&);
  using pointer = typename unique_type::pointer;

  static_assert(
    std::is_polymorphic<element_type>::value,
    "Cannot create a polymorphic<T> with a non-polymorphic type"
  );

  copier_type copier;
  unique_type data;

public:

  template <
    typename T,
    typename=typename std::enable_if<
      not std::is_same<polymorphic, impl::decay<T>>::value
    >::type
  > polymorphic (T&& value) :
    copier { impl::copy<element_type, impl::decay<T>> },
    data { core::make_unique<impl::decay<T>>(std::forward<T>(value)) }
  {
    using derived_type = impl::decay<T>;

    static_assert(
      not std::is_abstract<derived_type>::value,
      "Cannot create a polymorphic<T> from an abstract type"
    );

    static_assert(
      std::is_base_of<element_type, derived_type>::value,
      "Cannot create a polymorphic<T> from a non-derived instance"
    );
  }

  template <typename T>
  polymorphic (polymorphic<T> const& that) noexcept :
    copier { that.copier },
    data { that.copier(that.data) }
  { }

  template <typename T>
  polymorphic (polymorphic<T>&& that) noexcept :
    copier { that.copier },
    data { std::move(that.data) }
  { that.copier = impl::null_copy<T>; }

  polymorphic (polymorphic const& that) noexcept :
    copier { that.copier },
    data { that.copier(that.data) }
  { }

  polymorphic (polymorphic&& that) noexcept :
    copier { std::move(that.copier) },
    data { std::move(that.data) }
  { that.copier = impl::null_copy<element_type>; }

  polymorphic () noexcept :
    copier { impl::null_copy<element_type> },
    data { }
  { }

  ~polymorphic () noexcept { }

  polymorphic& operator = (polymorphic const& that) {
    polymorphic { that }.swap(*this);
    return *this;
  }

  polymorphic& operator = (polymorphic&& that) noexcept {
    polymorphic { std::move(that) }.swap(*this);
    return *this;
  }

  template <
    typename DerivedType,
    typename=typename std::enable_if<
      not std::is_same<polymorphic, impl::decay<DerivedType>>::value
    >::type
  > polymorphic& operator = (DerivedType&& derived) {
    polymorphic { std::forward<DerivedType>(derived) }.swap(*this);
    return *this;
  }

  template <typename T>
  polymorphic& operator = (polymorphic<T> const& that) {
    polymorphic { that }.swap(*this);
    return *this;
  }

  template <typename T>
  polymorphic& operator = (polymorphic<T>&& that) {
    polymorphic { that }.swap(*this);
    return *this;
  }

  void swap (polymorphic& that) noexcept {
    std::swap(this->copier, that.copier);
    std::swap(this->data, that.data);
  }

  explicit operator bool () const noexcept { return bool(this->data); }
  auto operator * () const noexcept -> impl::lvalue<element_type> {
    return *this->data;
  }

  pointer operator -> () const noexcept { return this->data.get(); }
};

}} /* namespace core::v1 */

namespace std {

template <typename T>
void swap (core::v1::polymorphic<T>& lhs, core::v1::polymorphic<T>& rhs) {
  lhs.swap(rhs);
}

} /* namespace std */

#endif /* CORE_POLYMORPHIC_HPP */
