String Component
================

.. default-domain:: cpp

.. |string_ref| replace:: :class:`string_ref <basic_string_ref\<T>>`

The string component contains types related to strings, and utilities for
strings. Specifically, the |string_ref| class resides in the
string component.

.. namespace:: core

.. class:: basic_string_ref<T>

   The |string_ref| type has been recreated in many different projects over the
   years, especially where work on immutable strings is desired. A |string_ref|
   provides an interface equivalent to a ``std::string const``, and any
   ``std::string`` or ``char const*`` is implicitly convertible to a
   |string_ref|. A |string_ref| is ONLY valid as long as the data it *views* is
   valid.

   .. warning:: Attempting to access a |string_ref| after the data it is
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

   .. function:: basic_string_ref (std::string const& str)

      Constructs the object such that it views the entire contents of the given
      string.

      :postcondition: this->size() == str.size()

   .. function:: constexpr basic_string_ref (pointer str, size_type len)

      Constructs the object such that it views the given string and has a
      maximum length of the given length.

      :postcondition: this->size() == len

   .. function:: basic_string_ref (pointer str)

      Constructs the object such that it views the given string, and has a
      length of the entire null terminated string.

   .. function:: basic_string_ref (basic_string_ref const&)

      Copies the object given, such that they are equivalent in every
      way.

      .. note:: Because copying a |string_ref| is cheap, no move constructor is
         defined.

   .. function:: constexpr basic_string_ref ()

      Constructs the object to be empty.

      :postcondition: this->empty() == true

   .. function:: operator std::basic_string<T> () const

      Marked as *explicit*. Creates a new ``std::basic_string`` from the
      |string_ref|.

   .. function:: constexpr const_iterator begin () const noexcept
                 constexpr const_iterator end () const noexcept
                 constexpr const_iterator cbegin () const noexcept
                 constexpr const_iterator cend () const noexcept

   .. function:: const_reverse_iterator rbegin() const noexcept
                 const_reverse_iterator rend () const noexcept
                 const_reverse_iterator crbegin () const noexcept
                 const_reverse_iterator crend () const noexcept

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
                |string_ref|.

   .. function:: constexpr pointer data () const

      :returns: :type:`pointer` to the data viewed by the |string_ref|.
                This pointer is not guaranteed to be null terminated, and
                should be treated as such.

.. type:: string_ref

   A type alias for |string_ref| where ``T`` is ``char``.

.. type:: wstring_ref

   A type alias for |string_ref| where ``T`` is ``wchar_t``.

.. type:: u16string_ref

   A type alias for |string_ref| where ``T`` is ``char16_t``.

.. type:: u32string_ref

   A type alias for |string_ref| where ``T`` is ``char32_t``.
