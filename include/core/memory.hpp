#ifndef CORE_MEMORY_HPP
#define CORE_MEMORY_HPP

#include <type_traits>
#include <memory>

#include <cstddef>

namespace core {
inline namespace v1 {
namespace impl {

template <typename U>
using remove_extent_t = typename std::remove_extent<Type>::type;

} /* namespace impl */

template <
  typename Type,
  typename=typename std::enable_if<not std::is_array<Type>::value>::type,
  typename... Args
> auto make_unique(Args&&... args) -> std::unique_ptr<Type> {
  return std::unique_ptr<Type>(new Type(std::forward<Args>(args)...));
}

template <
  typename Type,
  typename=typename std::enable_if<std::is_array<Type>::value>::type,
  typename=typename std::enable_if<not std::extent<Type>::value>::type
> auto make_unique(std::size_t size) -> std::unique_ptr<Type> {
  return std::unique_ptr<Type>(new impl::remove_extent_t<Type>[size] { });
}

template <
  typename Type,
  typename=typename std::enable_if<std::is_array<Type>::value>::type,
  typename=typename std::enable_if<std::extent<Type>::value>::type,
  typename... Args
> auto make_unique(Args&&...) -> void = delete;

}} /* namespace core::v1 */

#endif /* CORE_MEMORY_HPP */
