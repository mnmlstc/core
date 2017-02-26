String Component
================

.. namespace:: core

The string component contains types related to strings, and utilities for
strings. Specifically, the :any:`basic_string_view` class resides in the string
component.

Everything discussed in this section resides in the :file:`<core/{string}.hpp>`
header.

.. class:: template <class CharT, class Traits> basic_string_view

   The :any:`basic_string_view` type has been recreated in many different
   projects over the years, especially where work on a string is desired
   without having to mutate the string itself. A :any:`basic_string_view`
   provides an interface to a :cxx:`std::string const`, and any
   :cxx:`std::string` or :cxx:`char const*` is implicity convertible to a
   :any:`basic_string_view`. A :any:`basic_string_view` is *only* valid as long
   as data it *views* is valid.

   Attempting to access a :any:`basic_string_view` after the data is is viewing
   has been destructed will result in undefined behavior.

   .. type:: difference_type

      An alias of :cxx:`std::ptrdiff_t`

   .. type:: value_type

      An alias of :samp:`{CharT}`

   .. type:: size_type

      An alias of :cxx:`std::size_t`

   .. type:: const_reference
             reference

      An alias of :samp:`{value_type} const&`

   .. type:: const_pointer
             pointer

      An alias of :samp:`{value_type} const*`

   .. type:: const_iterator
             iterator

      An alias of :any:`pointer`

   .. type:: const_reverse_iterator
             reverse_iterator

      An alias of :cxx:`std::reverse_iterator<const_iterator>`

   .. var:: static constexpr size_type npos

      Equal to :cxx:`std::numeric_limits<size_type>::max()`

   .. function:: constexpr basic_string_view(pointer s, size_type len) noexcept

      Constructs the object such that it views the given string and has a
      size of :samp:`{len}`.

      :postcondition: :any:`size` == :samp:`{len}`

   .. function:: basic_string_view(pointer s) noexcept

      Constructs the :any:`basic_string_view` such that it views the entire
      length of :samp:`{s}`.

      :requires: :samp:`{s}` must be a null terminated string.
      :postcondition: :any:`size` == :samp:`strlen({s})`

   .. function:: basic_string_view (std::string const& str) noexcept

      Constructs the :any:`basic_string_view` such that it views the entire
      length of the string :samp:`{str}`.

      :postcondition: :any:`size` == :samp:`{str}.size()`

   .. function:: constexpr basic_string_view(basic_string_view const&) noexcept

      Copies the given :any:`basic_string_view`

      :note: Because copying a :any:`basic_string_view` is cheap (it is simply
             copying a pointer and an integer), no move constructor is
             defined.

   .. function:: constexpr basic_string_view () noexcept

      Constructs an empty :any:`basic_string_view`.

      :postcondition: :any:`empty` == :cxx:`true`

   .. function:: explicit operator basic_string () const

      Creates a new :cxx:`std::basic_string` from the :any:`basic_string_view`

   .. function:: constexpr const_iterator cbegin () const noexcept
                 constexpr const_iterator begin () const noexcept

      :returns: :any:`const_iterator` to the start of the
                :any:`basic_string_view`

   .. function:: constexpr const_iterator cend () const noexcept
                 constexpr const_iterator end () const noexcept

      :returns: :any:`const_iterator` to the end of the
                :any:`basic_string_view`

   .. function:: const_reverse_iterator crbegin () const noexcept
                 const_reverse_iterator rbegin () const noexcept

      :returns: :any:`const_reverse_iterator` to the start of the
                :any:`basic_string_view`

   .. function:: const_reverse_iterator crend () const noexcept
                 const_reverse_iterator rend () const noexcept

      :returns: :any:`const_reverse_iterator` to the end of the
                :any:`basic_string_view`

   .. function:: constexpr size_type max_size () const noexcept

      :returns: :any:`size`

   .. function:: constexpr size_type length () const noexcept
                 constexpr size_type size () const noexcept

      :returns: Length of the :any:`basic_string_view` in terms of
                :samp:`sizeof({CharT}) * N`

   .. function:: constexpr bool empty () const noexcept

      :returns: :any:`size` == 0

   .. function:: constexpr reference operator [] (size_type idx) const noexcept

      :returns: :any:`reference` to the :any:`value_type` located at *idx*.

   .. function:: reference at (size_type idx) const

      :returns: :any:`reference` to the :any:`value_type` located at *idx*.

      :throws: :cxx:`std::out_of_range` if *idx* is greater than or equal to
               :any:`size`.

   .. function:: constexpr reference front () const noexcept

      :returns: :any:`reference` to the :any:`value_type` located at index 0.

   .. function:: constexpr reference back () const noexcept

      :returns: :any:`reference` to the :any:`value_type` located at the end of
                the :any:`basic_string_view`

      :warning: Calling this function on an empty :any:`basic_string_view`
                will result in undefined behavior.

   .. function:: constexpr pointer data () const noexcept

      :returns: :any:`pointer` to the data viewed by the
                :any:`basic_string_view`. This pointer is not guaranteed to be
                null terminated, and should be treated as such. It is provided
                for interoperating with :cxx:`std::string`, but also for those
                cases where a :any:`basic_string_view` is guaranteed to observe
                null terminated string

   .. function:: void remove_prefix (size_type n) noexcept

      Moves the start of the :any:`basic_string_view` forward *n* characters of
      :any:`size` if *n* is greater than :any:`size`.

   .. function:: void remove_suffix (size_type n) noexcept

      Moves the end of the :any:`basic_string_view` backwards *n* characters or
      :any:`size` if *n* is greater than :any:`size`.

   .. function:: void clear () noexcept

      Sets the :any:`basic_string_view` to no longer observe a string.

      :postconditions: :any:`empty` == :cxx:`true`

   .. function:: size_type copy (CharT* s, size_type n, size_type pos=0) const

      Copies the substring :samp:`[{pos}, {pos} + {count})` to the character
      string pointed to by :samp:`{s}`. :samp:`{count}` is the smaller of
      :samp:`{n}` and :samp:`size() - {pos}`.

      :returns: Number of characters copied.

      :throws: :cxx:`std::out_of_range` if :samp:`{pos}` is greater than
               :any:`size`

   .. function:: constexpr basic_string_view substr (\
                   size_type pos,          \
                   size_type n=npos) const noexcept

      :returns: a new :any:`basic_string_view` with starting point :samp:{pos}
                and a length of :samp:`{n}` characters. If :samp:{n} is equal
                to :any:`npos`, or :samp:`{pos} + {n}` is greater than
                :any:`size`, the length will be the remainder of the string.
                Otherwise it will be :samp:`{n}` characters.

      :throws: :cxx:`std::out_of_range` if :samp:`{pos}` is greater than
               :any:`size`

   .. function:: bool starts_with (basic_string_view value) const noexcept
                 bool starts_with (value_type value) const noexcept

      :returns: Whether the :any:`basic_string_view` starts with the given
                :samp:`{value}`.

   .. function:: bool ends_with (basic_string_view value) const noexcept
                 bool ends_with (value_type value) const noexcept

      :returns: Whether the :any:`basic_string_view` ends with the given 
                :samp:`{value}`.

   .. function:: difference_type compare (basic_string_view s) const noexcept

      Compares two character sequences. Compares the :any:`basic_string_view`
      with :samp:`{s}` by calling
      :samp:`{traits}::compare(data(), {s}.data(), {length})`, where
      :samp:`{length}` is the smaller of :any:`size` and :samp:`{s}.size()`.

      :returns: A value according to the following table

   +--------------------------------+----------------------------------+------+
   |                            Condition                              |Result|
   +================================+==================================+======+
   | :samp:`{traits}::compare < 0`                                     |``-1``|
   +--------------------------------+----------------------------------+------+
   |                                | :any:`size` < :samp:`{s}.size()` |``-1``|
   +                                +----------------------------------+------+
   | :samp:`{traits}::compare == 0` | :any:`size` == :samp:`{s}.size()`| ``0``|
   +                                +----------------------------------+------+
   |                                | :any:`size` > :samp:`{s}.size()` | ``1``|
   +--------------------------------+----------------------------------+------+
   | :samp:`{traits}::compare > 0`                                     | ``1``|
   +--------------------------------+----------------------------------+------+

   .. function:: difference_type compare (\
                   size_type pos1,        \
                   size_type n1,          \
                   basic_string_view s,   \
                   size_type pos2,        \
                   size_type n2) const noexcept

      Constructs a substring :any:`basic_string_view` with :samp:`{pos1}, {n1}`
      and then calls its :any:`compare` function on a substring
      :any:`basic_string_view` constructed from :samp:`{s}` with
      :samp:`{pos2}, {n2}`.

   .. function:: difference_type compare (\
                   size_type pos,         \
                   size_type n1,          \
                   pointer s,             \
                   size_type n2) const noexcept
      
      Constructs a substring :any:`basic_string_view` with :samp:`{pos}, {n1}`
      and then calls its :any:`compare` function with
      :samp:`basic_string_view({s}, {n2}`.

   .. function:: difference_type compare (\
                   size_type pos,         \
                   size_type n,           \
                   basic_string_view s) const noexcept

      Constructs a :any:`basic_string_view` with :samp:`{pos}, {n}`, and
      then calls its :any:`compare` function with :samp:`{s}`

   .. function:: difference_type compare (\
                   size_type pos,         \
                   size_type n,           \
                   pointer s) const noexcept

      Constructs a :any:`basic_string_view` with :samp:`{pos}, {n}`
      then calls its :any:`compare` function with
      :samp:`basic_string_view({s})`.

   .. function:: difference_type compare (pointer s) const noexcept

      Calls  :any:`compare` with :samp:`basic_string_view({s})`.

   .. function:: size_type find_first_not_of (\
                   basic_string_view str,     \
                   size_type pos = 0) const noexcept

      Finds the first character not equal to any of the characters of
      :samp:`{str}` in the :any:`basic_string_view`, starting at :samp:`{pos}`

   .. function:: size_type find_first_not_of (\
                   pointer s,                 \
                   size_type pos,             \
                   size_type n) const noexcept

      Calls :any:`find_first_not_of` with
      :samp:`basic_string_view({s}, {n}), {pos}`

   .. function:: size_type find_first_not_of (\
                   pointer s,                 \
                   size_type pos=0) const noexcept

      Calls :any:`find_first_not_of` with
      :samp:`basic_string_view({s}), {pos}`

   .. function::  size_type find_first_not_of (\
                   value_type c,              \
                   size_type pos=0) const noexcept

      Calls  :any:`find_first_not_of` with
      :samp:`basic_string_view(&{c}, 1), {pos}`

   .. function:: size_type find_first_of (\
                   basic_string_view str, \
                   size_type pos = 0) const noexcept

      Finds the first occurence of any of the characters of :samp:`{str}`
      starting at position :samp:`{pos}`.

   .. function:: size_type find_first_of (\
                   pointer s,             \
                   size_type pos,         \
                   size_type n) const noexcept

      Calls :any:`find_first_of` with
      :samp:`basic_string_view({s}, {n}), {pos}`

   .. function:: size_type find_first_of (\
                   pointer s,             \
                   size_type pos = 0) const noexcept

      Calls :any:`find_first_of` with :samp:`basic_string_view({s}), {pos}`.

   .. function:: size_type find_first_of (\
                   value_type c,          \
                   size_type pos = 0) const noexcept

      Calls :any:`find_first_of` with :samp:`basic_string_view(&{c}, 1), {pos}`

   .. function:: size_type find (         \
                   basic_string_view str, \
                   size_type pos = 0) const noexcept

      Finds the first occurence of :samp:`{str}` in the
      :any:`basic_string_view`, starting at :samp:`{pos}`.

   .. function:: size_type find (         \
                   pointer s,             \
                   size_type pos,         \
                   size_type n) const noexcept

      Calls :any:`find` with :samp:`basic_string_view({s}, {n}), {pos}`

   .. function:: size_type find (pointer s, size_type pos=0) const noexcept

      Calls :any:`find` with :samp:`basic_string_view({s}), {pos}`

   .. function:: size_type find (value_type c, size_type pos=0) const noexcept

      Calls :any:`find` with :samp:`basic_string_view(&{c}, 1), {pos}`.

   .. function:: size_type find_last_not_of (\
                   basic_string_view str,    \
                   size_type pos=npos) const noexcept

      Finds the last character not equal to any of the characters of
      :samp:`{str}` in this view, starting at position :samp:`{pos}`. This
      function searches from the end of the :any:`basic_string_view`.

   .. function:: size_type find_last_not_of (\
                   pointer s,                \
                   size_type pos,            \
                   size_type n) const noexcept

      Calls :any:`find_last_not_of` with
      :samp:`basic_string_view({s}, {n}), {pos}`

   .. function:: size_type find_last_not_of (\
                   pointer s,                \
                   size_type p = npos) const noexcept

      Calls :any:`find_last_not_of` with :samp:`basic_string_view({s}), {pos}`

   .. function:: size_type find_last_not_of (\
                   value_type c,             \
                   size_type pos = npos) const noexcept

      Calls :any:`find_last_not_of` with
      :samp:`basic_string_view(&{c}, 1), {pos}`.

   .. function:: size_type find_last_of ( \
                   basic_string_view str, \
                   size_type pos = npos) const noexcept

      Finds the last occurence of any of the characters of :samp:`{str}` in
      this view, starting at position :samp:`{pos}`. This function starts from
      the end of the :any:`basic_string_view`.

   .. function:: size_type find_last_of ( \
                   pointer s,             \
                   size_type pos,         \
                   size_type n) const noexcept

      Calls :any:`find_last_of` with :samp:`basic_string_view({s}, {n}), {pos}`

   .. function:: size_type find_last_of ( \
                   pointer s,             \
                   size_type pos = npos) const noexcept

      Calls :any:`find_last_of` with :samp:`basic_string_view({s}), {pos}`

   .. function:: size_type find_last_of ( \
                   value_type c,          \
                   size_type pos = npos) const noexcept

      Calls :any:`find_last_of` with :samp:`basic_string_view(&{c}, 1), {pos}`

   .. function:: size_type rfind (         \
                   basic_string_view str,  \
                   size_type pos = npos) const noexcept

      Finds the last occurence of :samp:`{str}` in :any:`basic_string_view`,
      starting at :samp:`{pos}`. :samp:`{pos}` represents the position from
      the *end* of the :any:`basic_string_view`.

   .. function:: size_type rfind (         \
                   pointer s,              \
                   size_type pos,          \
                   size_type n) const noexcept

   .. function:: size_type rfind (pointer s, size_type pos=npos) const noexcept

      Calls :any:`rfind` with :samp:`basic_string_view({s}), {pos}`

   .. function:: size_type rfind (\
                   value_type c,  \
                   size_type pos=npos) const noexcept

      Calls :any:`rfind` with :samp:`basic_string_view(&{c}, 1), {pos}`.

   .. function:: void swap (basic_string_view& that) noexcept

      Swaps the contents of the :any:`basic_string_view` with :samp:`{that}`.

.. function:: bool operator == (       \
                basic_string_view lhs, \
                basic_string_view rhs) noexcept
              bool operator != (       \
                basic_string_view lhs, \
                basic_string_view rhs) const noexcept
              bool operator >= (       \
                basic_string_view lhs, \
                basic_string_view rhs) const noexcept
              bool operator <= (       \
                basic_string_view lhs, \
                basic_string_view rhs) const noexcept
              bool operator > (        \
                basic_string_view lhs, \
                basic_string_view rhs) const noexcept
              bool operator < (        \
                basic_string_view lhs, \
                basic_string_view rhs) const noexcept

   Compares the contents of a :any:`basic_string_view` :samp:`{lhs}` with the
   contents of a :any:`basic_string_view` :samp:`{rhs}`

   :returns: Whther the given :any:`basic_string_view`'s meet the requirements
             for the given operator. Follows the same semantics as the
             :cxx:`std::string` comparison operators.

.. function:: std::basic_ostream<T>& operator << (\
                std::basic_ostream<T>& os,        \
                basic_string_view const& str)

   Overload to print :any:`basic_string_view` directly to the given stream *os*.

   :note: This function calls :any:`to_string` to perform this operation.

.. type:: string_view

   A type alias for :any:`basic_string_view` where :samp:`{CharT}` is
   :cxx:`char`.

.. type:: wstring_view

   A type alias for :any:`basic_string_view` where :samp:`{CharT}` is
   :cxx:`wchar_t`.

.. type:: u16string_view

   A type alias for :any:`basic_string_view` where :samp:`{CharT}` is
   :cxx:`char16_t`.

.. type:: u32string_view

   A type alias for :any:`basic_string_view` where :samp:`{CharT}` is
   :cxx:`char32_t`.

Specializations
---------------

Several specializations for standard code are provided

.. function:: void swap(basic_string_view& l, basic_string_view& r) noexcept

   Calls :samp:`{l}.swap({r})`. It is provided for Argument Dependent Lookup
   purposes.

.. namespace:: std

.. class:: template <> hash<core::basic_string_view<CharT, Traits>>

   Specialization hash for :any:`core::basic_string_view`.

   .. versionadded:: 1.2

      This now uses MurmurHash2 for 32-bit and 64-bit systems. Specifically,
      an endian neutral version is used.

   .. note:: Versions before 1.2 would simply return a pointer to the
             underlying data, and could not be relied on for content hashing.
