#ifndef CORE_STRING_HPP
#define CORE_STRING_HPP

#include <initializer_list>
#include <stdexcept>
#include <iterator>
#include <string>
#include <limits>

namespace core {
inline namespace v1 {

template <class CharT, class Trait=std::char_traits<CharT>>
struct basic_string_ref {
  using difference_type = std::ptrdiff_t;
  using value_type = CharT;
  using size_type = std::size_t;

  using reference = value_type const&;
  using pointer = value_type const*;

  using const_reference = reference;
  using const_pointer = pointer;

  using const_iterator = pointer;
  using iterator = const_iterator;

  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using reverse_iterator = const_reverse_iterator;

  static constexpr size_type npos = std::numeric_limits<size_type>::max();

  template <class Allocator>
  basic_string_ref (std::basic_string<CharT, Traits, Allocator> const& that) :
    str { that.data() },
    len { that.size() }
  { }

  basic_string_ref (std::initializer_list<CharT> il);

  constexpr basic_string_ref (pointer str, size_type len) noexcept :
    str { str },
    len { len }
  { }

  basic_string_ref (pointer str) noexcept :
    basic_string_ref { str, std::strlen(str) }
  { }

  constexpr basic_string_ref (basic_string_ref const& that) noexcept :
    str { that.str },
    len { that.len }
  { }

  constexpr basic_string_ref () noexcept :
    str { nullptr },
    len { 0 }
  { }

  basic_string_ref& operator = (basic_string_ref const& that) noexcept {
    basic_string_ref { that }.swap(*this);
    return *this;
  }

  template <class Allocator>
  explicit operator std::basic_string<CharT, Traits, Allocator> () const {
    return std::basic_string<CharT, Traits, Allocator> {
      this->str,
      this->len
    };
  }

  constexpr const_iterator begin () const { return this->str; }
  constexpr const_iterator end () const { return this->str + this->size(); }

  constexpr const_iterator cbegin () const;
  constexpr const_iterator cend () const;

  const_reverse_iterator rbegin () const;
  const_reverse_iterator rend () const;

  const_reverse_iterator crbegin () const;
  const_reverse_iterator crend () const;

  constexpr size_type max_size () const;
  constexpr size_type length () const { return this->size(); }
  constexpr size_type size () const { return this->len; }

  constexpr bool empty () const { return this->len == 0; }

  constexpr reference operator [] (size_type idx) const {
    return this->str[idx];
  }

  constexpr reference front () const;
  constexpr reference back () const;
  constexpr pointer data () const;

  void remove_prefix (size_type n);
  void remove_suffix (size_type n);
  void clear ();

  constexpr basic_string_ref substr (size_type pos, size_type n=npos) const;
  bool starts_with (basic_string_ref that) const;
  bool ends_with (basic_string_ref that) const;
  int compare (basic_string_ref that) const;

  reference at (size_type idx) const {
    if (idx > this->size()) {
      throw std::out_of_range { "requested index out of range" };
    }
    return (*this)[idx];
  }

  size_type find_first_not_of (basic_string_ref that) const;
  size_type find_last_not_of (basic_string_ref that) const;
  size_type find_first_of (basic_string_ref that) const;
  size_type find_last_of (basic_string_ref that) const;
  size_type rfind (basic_string_ref that) const;
  size_type find (basic_string_ref that) const;

  size_type find_first_not_of (value_type value) const;
  size_type find_last_not_of (value_type value) const;
  size_type find_first_of (value_type value) const;
  size_type find_last_of (value_type value) const;
  size_type rfind (value_type value) const;
  size_type find (value_type value) const;

  void swap (basic_string_ref& that) noexcept {
    std::swap(this->str, that.str);
    std::swap(this->len, that.len);
  }

private:
  pointer str;
  size_type len;
};

using u32string_ref = basic_string_ref<char32_t>;
using u16string_ref = basic_string_ref<char16_t>;
using wstring_ref = basic_string_ref<wchar_t>;
using string_ref = basic_string_ref<char>;

template <class CharT, typename Traits>
bool operator == (
  basic_string_ref<CharT, Traits> lhs,
  basic_string_ref<CharT, Traits> rhs
) noexcept { return false; }

template <class CharT, typename Traits>
bool operator != (
  basic_string_ref<CharT, Traits> lhs,
  basic_string_ref<CharT, Traits> rhs
) noexcept { return false; }

template <class CharT, typename Traits>
bool operator >= (
  basic_string_ref<CharT, Traits> lhs,
  basic_string_ref<CharT, Traits> rhs
) noexcept { return false; }

template <class CharT, typename Traits>
bool operator <= (
  basic_string_ref<CharT, Traits> lhs,
  basic_string_ref<CharT, Traits> rhs
) noexcept { return false; }

template <class CharT, typename Traits>
bool operator > (
  basic_string_ref<CharT, Traits> lhs,
  basic_string_ref<CharT, Traits> rhs
) noexcept { return false; }

template <class CharT, typename Traits>
bool operator < (
  basic_string_ref<CharT, Traits> lhs,
  basic_string_ref<CharT, Traits> rhs
) noexcept { return false; }

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator << (
  std::basic_ostream& os,
  basic_string_ref<CharT, Traits> const& str
);

}} /* namespace core::v1 */

namespace std {

template <class CharT, class Traits>
void swap (
  core::v1::basic_string_ref<CharT, Traits>& lhs,
  core::v1::basic_string_ref<CharT, Traits>& rhs
) noexcept { return lhs.swap(rhs); }

template <> struct hash<core::v1::u32string_ref>;
template <> struct hash<core::v1::u16string_ref>;
template <> struct hash<core::v1::wstring_ref>;
template <> struct hash<core::v1::string_ref>;

} /* namespace std */

#endif /* CORE_STRING_HPP */
