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

template <class Container>
auto rbegin (Container const& container) -> decltype(container.rbegin()) {
  return container.rbegin();
}

template <class Container>
auto rbegin (Container& container) -> decltype(container.rbegin()) {
  return container.rbegin();
}

template <class Container>
auto crbegin (Container const& container) -> decltype(rbegin(container)) {
  return rbegin(container);
}

template <class Container>
auto rend (Container const& container) -> decltype(container.rend()) {
  return container.rend();
}

template <class Container>
auto rend (Container& container) -> decltype(container.rend()) {
  return container.rend();
}

template <class Container>
auto crend (Container const& container) -> decltype(rend(container)) {
  return rend(container);
}

}} /* namespace core::v1 */

#endif /* CORE_ITERATOR_HPP */
