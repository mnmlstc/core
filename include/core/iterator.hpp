#ifndef CORE_ITERATOR_HPP
#define CORE_ITERATOR_HPP

#include <iterator>

namespace core {
inline namespace v1 {

template <class Container>
auto cbegin (Container const& container) -> decltype(std::begin(container)) {
  return std::begin(container);
}

template <class Container>
auto cend (Container const& container) -> decltype(std::end(container)) {
  return std::end(container);
}

}} /* namespace core::v1 */

#endif /* CORE_ITERATOR_HPP */
