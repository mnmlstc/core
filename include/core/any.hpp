#ifndef CORE_ANY_HPP
#define CORE_ANY_HPP

#include <stdexcept>
#include <typeinfo>
#include <memory>

#include <core/type_traits.hpp>

namespace core {
inline namespace v1 {
namespace impl {

template <class Type>
using is_small = std::integral_constant<bool, sizeof(Type) <= sizeof(void*)>;

struct any_dispatch {
  using destroy_function = void (*)(void**);
  using clone_function = void(*)(void* const*, void**);
  using type_function = std::type_info const& (*)();

  destroy_function const destroy;
  clone_function const clone;
  type_function const type;
};

template <class Type, bool=is_small<Type>::value>
struct any_dispatch_select;

template <class Type>
struct any_dispatch_select<Type, true> {
  using allocator_type = std::allocator<Type>;

  static void clone (void* const* source, void** data) {
    allocator_type alloc { };
    auto const& value = *reinterpret_cast<Type const*>(source);
    auto pointer = reinterpret_cast<Type*>(data);
    std::allocator_traits<allocator_type>::construct(alloc, pointer, value);
  }

  static void destroy (void** data) {
    allocator_type alloc { };
    auto pointer = reinterpret_cast<Type*>(data);
    std::allocator_traits<allocator_type>::destroy(alloc, pointer);
  }
};

template <class Type>
struct any_dispatch_select<Type, false> {
  using allocator_type = std::allocator<Type>;

  static void clone (void* const* source, void** data) {
    allocator_type alloc { };
    auto const& value = **reinterpret_cast<Type* const*>(source);
    auto pointer = std::allocator_traits<allocator_type>::allocate(alloc, 1);
    std::allocator_traits<allocator_type>::construct(alloc, pointer, value);
    *data = pointer;
  }

  static void destroy (void** data) {
    allocator_type alloc { };
    auto& value = *reinterpret_cast<Type**>(data);
    std::allocator_traits<allocator_type>::destroy(alloc, value);
    std::allocator_traits<allocator_type>::deallocate(alloc, value, 1);
  }
};

template <class Type>
any_dispatch const* get_any_dispatch () {
  static any_dispatch const instance = {
    any_dispatch_select<Type>::destroy,
    any_dispatch_select<Type>::clone,
    [] () -> std::type_info const& { return typeid(Type); }
  };
  return std::addressof(instance);
}

template <>
inline any_dispatch const* get_any_dispatch<void> () {
  static any_dispatch const instance = {
    [] (void**) { },
    [] (void* const*, void**) { },
    [] () -> std::type_info const& { return typeid(void); }
  };
  return std::addressof(instance);
}

} /* namespace impl */

class bad_any_cast final : public std::bad_cast {
public:
  virtual char const* what () const noexcept override {
    return "bad any cast";
  }
};

class any final {
  template <class ValueType>
  friend ValueType const* any_cast (any const*) noexcept;
  template <class ValueType> friend ValueType* any_cast (any*) noexcept;

  impl::any_dispatch const* table;
  void* data;

  template <class ValueType>
  any (ValueType&& value, std::true_type&&) :
    table { impl::get_any_dispatch<decay_t<ValueType>>() },
    data { nullptr }
  {
    using value_type = decay_t<ValueType>;
    using allocator_type = std::allocator<value_type>;
    allocator_type alloc { };
    auto pointer = reinterpret_cast<value_type*>(std::addressof(this->data));
    std::allocator_traits<allocator_type>::construct(
      alloc, pointer, std::forward<ValueType>(value)
    );
  }

  template <class ValueType>
  any (ValueType&& value, std::false_type&&) :
    table { impl::get_any_dispatch<decay_t<ValueType>>() },
    data { nullptr }
  {
    using value_type = decay_t<ValueType>;
    using allocator_type = std::allocator<value_type>;
    allocator_type alloc { };
    auto pointer = std::allocator_traits<allocator_type>::allocate(alloc, 1);
    std::allocator_traits<allocator_type>::construct(
      alloc, pointer, std::forward<ValueType>(value)
    );
    this->data = pointer;
  }

  template <class ValueType>
  ValueType const* cast (std::true_type&&) const {
    return reinterpret_cast<ValueType const*>(std::addressof(this->data));
  }

  template <class ValueType>
  ValueType* cast (std::true_type&&) {
    return reinterpret_cast<ValueType*>(std::addressof(this->data));
  }

  template <class ValueType>
  ValueType const* cast (std::false_type&&) const {
    return reinterpret_cast<ValueType const*>(this->data);
  }

  template <class ValueType>
  ValueType* cast (std::false_type&&) {
    return reinterpret_cast<ValueType*>(this->data);
  }

public:
  any (any const& that) :
    table { that.table },
    data { nullptr }
  {
    this->table->clone(std::addressof(that.data), std::addressof(this->data));
  }

  any (any&& that) noexcept :
    table { that.table },
    data { that.data }
  {
    that.table = impl::get_any_dispatch<void>();
    that.data = nullptr;
  }

  any () noexcept :
    table { impl::get_any_dispatch<void>() },
    data { nullptr }
  { }

  template <
    class ValueType,
    class=enable_if_t<not std::is_same<any, decay_t<ValueType>>::value>
  > any (ValueType&& value) :
    any { std::forward<ValueType>(value), impl::is_small<ValueType> { } }
  { }

  ~any () noexcept { this->clear(); }

  any& operator = (any const& that) {
    any { that }.swap(*this);
    return *this;
  }

  any& operator = (any&& that) noexcept {
    any { std::move(that) }.swap(*this);
    return *this;
  }

  void swap (any& that) noexcept {
    std::swap(this->table, that.table);
    std::swap(this->data, that.data);
  }

  void clear () noexcept {
    this->table->destroy(std::addressof(this->data));
    this->table = impl::get_any_dispatch<void>();
  }

  std::type_info const& type () const noexcept { return this->table->type(); }

  bool empty () const noexcept {
    return this->table == impl::get_any_dispatch<void>();
  }

};

template <class ValueType>
ValueType const* any_cast (any const* operand) noexcept {
  return operand and operand->type() == typeid(ValueType)
    ? operand->cast<ValueType>(impl::is_small<ValueType> { })
    : nullptr;
}

template <class ValueType>
ValueType* any_cast (any* operand) noexcept {
  return operand and operand->type() == typeid(ValueType)
    ? operand->cast<ValueType>(impl::is_small<ValueType> { })
    : nullptr;
}

template <
  class ValueType,
  class=enable_if_t<
    std::is_reference<ValueType>::value or
    std::is_copy_constructible<ValueType>::value
  >
> ValueType any_cast (any const& operand) {
  using type = remove_reference_t<ValueType>;
  auto pointer = any_cast<add_const_t<type>>(std::addressof(operand));
  if (not pointer) { throw bad_any_cast { }; }
  return *pointer;
}

template <
  class ValueType,
  class=enable_if_t<
    std::is_reference<ValueType>::value or
    std::is_copy_constructible<ValueType>::value
  >
> ValueType any_cast (any&& operand) {
  using type = remove_reference_t<ValueType>;
  auto pointer = any_cast<type>(std::addressof(operand));
  if (not pointer) { throw bad_any_cast { }; }
  return *pointer;
};

template <
  class ValueType,
  class=enable_if_t<
    std::is_reference<ValueType>::value or
    std::is_copy_constructible<ValueType>::value
  >
> ValueType any_cast (any& operand) {
  using type = remove_reference_t<ValueType>;
  auto pointer = any_cast<type>(std::addressof(operand));
  if (not pointer) { throw bad_any_cast { }; }
  return *pointer;
}

}} /* namespace core::v1 */

namespace std {

inline void swap (core::any& lhs, core::any& rhs) noexcept { lhs.swap(rhs); }

} /* namespace std */

#endif /* CORE_ANY_HPP */
