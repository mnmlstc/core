.. _core-string-component:

String Component
================

.. default-domain:: cpp

.. |string_view| replace:: :class:`string_view <basic_string_view\<T>>`
.. |npos| replace:: :member:`npos <basic_string_view\<T>::npos>`

The string component contains types related to strings, and utilities for
strings. Specifically, the |string_view| class resides in the
string component.

The string component resides in the ``<core/string.hpp>`` header.

.. namespace:: core

.. class:: basic_string_view<T>

   The |string_view| type has been recreated in many different projects over the
   years, especially where work on immutable strings is desired. A |string_view|
   provides an interface equivalent to a ``std::string const``, and any
   ``std::string`` or ``char const*`` is implicitly convertible to a
   |string_view|. A |string_view| is ONLY valid as long as the data it *views* is
   valid.

   .. warning:: Attempting to access a |string_view| after the data it is
      viewing has been destructed will result in undefined behavior.

   .. type:: difference_type

      An alias of ``std::ptrdiff_t``

   .. type:: value_type

      An alias of ``T``

   .. type:: size_type

      An alias of ``std::size_t``

   .. type:: const_reference
             reference

      ``value_type const&``

   .. type:: const_pointer
             pointer

      ``value_type const*``

   .. type:: const_iterator
             iterator

      :type:`pointer`

   .. type:: const_reverse_iterator
             reverse_iterator

      ``std::reverse_iterator<const_iterator>``

   .. member:: static constexpr npos

      :type: :type:`size_type`

      equal to ``std::numeric_limits<size_type>::max()``

   .. function:: basic_string_view (std::string const& str)

      Constructs the object such that it views the entire contents of the given
      string.

      :postcondition: :func:`size` == *str*.size()

   .. function:: constexpr basic_string_view (pointer str, size_type len)

      Constructs the object such that it views the given string and has a
      maximum length of the given length.

      :postcondition: :func:`size` == *len*

   .. function:: basic_string_view (pointer str)

      Constructs the object such that it views the given string, and has a
      length of the entire null terminated string.

   .. function:: basic_string_view (basic_string_view const&)

      Copies the object given, such that they are equivalent in every
      way.

      .. note:: Because copying a |string_view| is cheap, no move constructor is
         defined.

   .. function:: constexpr basic_string_view ()

      Constructs the object to be empty.

      :postcondition: :func:`empty` == true

   .. function:: operator std::basic_string<T> () const

      Marked as *explicit*. Creates a new ``std::basic_string`` from the
      |string_view|.

   .. function:: constexpr const_iterator cbegin () const noexcept
                 constexpr const_iterator begin () const noexcept

      :returns: Iterator to the beginning of the |string_view|.

   .. function:: constexpr const_iterator cend () const noexcept
                 constexpr const_iterator end () const noexcept

      :returns: Iterator to the end of the |string_view|.

   .. function:: const_reverse_iterator crbegin() const noexcept
                 const_reverse_iterator rbegin () const noexcept

      :returns: reverse iterator to the beginning of the |string_view|

   .. function:: const_reverse_iterator crend () const noexcept
                 const_reverse_iterator rend () const noexcept

      :returns: reverse iterator to the beginning of the |string_view|

   .. function:: constexpr size_type max_size () const noexcept

      :returns: :func:`size`

   .. function:: constexpr size_type length () const noexcept
                 constexpr size_type size () const noexcept

      :returns: Length of the object in terms of ``sizeof(T) * N``

   .. function:: constexpr bool empty () const noexcept

      :returns: Whether the object is a 'null view'

   .. function:: constexpr reference operator [] (size_type idx) const

      :returns: reference to the :type:`value_type` located at *idx*.

   .. function:: constexpr reference front () const

      :returns: reference to the :type:`value_type` located at the index 0.

   .. function:: constexpr reference back () const

      :returns: reference to the :type:`value_type` located at the end of the
                |string_view|.

   .. function:: constexpr pointer data () const

      :returns: :type:`pointer` to the data viewed by the |string_view|.
                This pointer is not guaranteed to be null terminated, and
                should be treated as such.

   .. function:: void remove_prefix (size_type n)

      Moves the front of the |string_view| forward *n* characters or
      :func:`size` if *n* is greater than :func:`size`.

   .. function:: void remove_suffix (size_type n)

      Moves the end of the |string_view| backwards *n* characters or
      :func:`size` if *n* is greater than :func:`size`.

   .. function:: void clear () noexcept

      Sets the |string_view| to be empty.

      :postconditions: :func:`empty` == true

   .. function:: constexpr basic_string_view substr \
                 (size_type pos, size_type n=npos) const

      :returns: a new |string_view| with starting point *pos* and a length of
                *n* characters. If *n* is equal to |npos|, or 
                *pos* + *n* is greater than :func:`size`, the length will be
                the remainder of the string. Otherwise it will be *n*
                characters.
      :throws: ``std::out_of_range`` if *pos* is greater than :func:`size`

   .. function:: bool starts_with (basic_string_view value) const noexcept
                 bool starts_with (value_type value) const noexcept

      :returns: Whether the |string_view| starts with the given *value*.

   .. function:: bool ends_with (basic_string_view value) const noexcept
                 bool ends_with (value_type value) const noexcept

      :returns: Whether the |string_view| ends with the given *value*.

   .. function:: difference_type compare (basic_string_view that) const

      Compares two |string_view|'s. First calculates the number of characters
      to compare, then compares via a character by character lexicographical
      comparison. If the result is 0, then their sizes are compared and the
      return value is affected by their length.

      :returns: negative value if this |string_view| is less than the other,
                zero if the both |string_view|'s are equal,
                positive value if this |string_view| is greater than the other.

   .. function:: reference at (size_type idx) const

      :returns: :type:`value_type` located at *idx*.
      :throws: ``std::out_of_range`` if *idx* is greater than or equal to
               :func:`size`.

   .. function:: size_type find_first_not_of (basic_string_view) const
                 size_type find_first_not_of (value_type) const

      Finds the first character equal to none of the characters in the given
      character sequence. 

      :returns: index of the first character not in the given sequence, or
                |npos| if no such character is found.

   .. function:: size_type find_last_not_of (basic_string_view) const
                 size_type find_last_not_of (value_type) const

      Finds the last character equal to none of the characters in the given
      character sequence.

      :returns: index of the last character not in the given sequence, or
                |npos| if no such character is found.

   .. function:: size_type find_first_of (basic_string_view) const
                 size_type find_first_of (value_type) const

      Finds the first character equal to one of characters in the given
      character sequence.

      :returns: Index of the first character found, or |npos| if no such 
                character is found.

   .. function:: size_type find_last_of (basic_string_view) const
                 size_type find_last_of (value_type) const

      Finds the last character equal to one of characters in the given
      character sequence.

      :returns: Index of the last character found, or |npos| is no such
                character is found.

   .. function:: size_type rfind (basic_string_view) const
                 size_type rfind (value_type) const

      Finds the last substring equal to the given character sequence.

      :returns: index of the desired substring, or |npos| if no such substring
                was found.

   .. function:: size_type find (basic_string_view) const
                 size_type find (value_type) const

      Finds the first substring equal to the given character sequence.

      :returns: index of the desired substring, or |npos| if no such substring
                was found.

   .. function:: void swap (basic_string_view& that) noexcept

      Swaps the contents of the |string_view| with *that*.

.. function:: bool operator == (basic_string_view, basic_string_view)
              bool operator != (basic_string_view, basic_string_view)
              bool operator >= (basic_string_view, basic_string_view)
              bool operator <= (basic_string_view, basic_string_view)
              bool operator > (basic_string_view, basic_string_view)
              bool operator < (basic_string_view, basic_string_view)

   :returns: Whether the given |string_view|'s meet the requirements for the
             given operator. Follows the same semantics as the ``std::string``
             comparison operators.

.. function:: std::basic_ostream<T>& operator << \
              (std::basic_ostream<T>& os, basic_string_view<T> const& str)

   Overload to print a |string_view| directly to the given stream *os*.

.. type:: string_view

   A type alias for |string_view| where ``T`` is ``char``.

.. type:: wstring_view

   A type alias for |string_view| where ``T`` is ``wchar_t``.

.. type:: u16string_view

   A type alias for |string_view| where ``T`` is ``char16_t``.

.. type:: u32string_view

   A type alias for |string_view| where ``T`` is ``char32_t``.

Specializations
---------------

.. namespace:: std

Several specializations for standard code are provided

.. function:: void swap(basic_string_view&, basic_string_view&)

   Calls :func:`basic_string_view\<T>::swap`

.. class:: hash<basic_string_view<T>>

   Specialization hash for |string_view|.
