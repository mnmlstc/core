.. _core-expected-component:

Expected Component
==================

.. default-domain:: cpp

.. |expected-v| replace:: :class:`expected\<void>`
.. |expected| replace:: :class:`expected <expected\<T>>`

The |expected| component is closely related to the ``Expected<T>`` put
forth by Andrei Alexandrescu in his 2012 talk *Systematic Error Handling in
C++*. It can be considered a combination between :class:`optional\<T>`
and a :class:`variant\<Ts>` with a type of
``variant<T, std::exception_ptr>``. It differs from that of Alexandrescu's
talk, by adding additional functionality, the ability to rethrow the contained
exception (if any), as well as the ability to *extract* the contained exception
without the need of a try-catch block placed by the user. Finally, a
specialization for |expected-v| exists, allowing a function that normally
returns void (but which may throw an exception) to return the error by way
of |expected|.

.. namespace:: core

.. class:: bad_expected_type

   :inherits: std::logic_error

.. class:: expected<T>

   |expected| works much like :class:`optional\<T>` in that it
   contains an optionally instantiated type ``T``. However, unlike
   :class:`optional\<T>`, it is never in a 'null' state. It will instead hold
   an exception that was thrown at some level within the function that returned
   the |expected|. Additionally, it does not model an object.

   .. note:: While |expected| uses placement new internally, it
      is ok to use a class that overloads the address-of operator with it.

   The following Concepts are required for the type that |expected| manages:

    * DefaultConstructible
    * CopyConstructible
    * MoveConstructible
    * CopyAssignable
    * MoveAssignable

   .. type:: value_type

      Represents the given type ``T``.

   .. function:: explicit expected (std::exception_ptr) noexcept

      Initializes the |expected| with the given exception_ptr

   .. function:: explicit expected (value_type const&)
                 explicit expected (value_type&&) noexcept

      Initializes the |expected| with the given value.

   .. function:: expected (expected const&)
                 expected (expected&&) noexcept

      Properly initializes based on whether the incoming |expected| is valid
      or not. The state of the incoming |expected| does not change.

   .. function:: expected () noexcept

      Default initializes the :type:`value_type` contained within the
      |expected|

   .. function:: expected& operator = (expected const&)
                 expected& operator = (expected&&)

      Sets the state of ``*this`` to be equivalent to, or hold the data
      contained within the incoming |expected|

   .. function:: expected& operator = (value_type const&)
                 expected& operator = (value_type&&) noexcept

      Initializes the |expected| :type:`value_type` with the assigned value.
      If the |expected| holds an exception_ptr, it is destructed, and
      the :type:`value_type` is initialized (and not assigned) the incoming
      value.

   .. function:: operator bool () const noexcept

      This function is an explicit conversion operator.

      :returns: Whether the |expected| is valid or not.

   .. function:: value_type const& operator * () const
                 value_type& operator * () const

      Throws if the |expected| is invalid.

      :returns: The object managed by |expected|
      :throws: The exception managed by |expected|
      :noexcept: false

   .. function:: value_type const& value () const
                 value_type& value () const

      Throws if the |expected| is invalid.

      :returns: The object managed by |expected|
      :throws: The exception managed by |expected|
      :noexcept: false

   .. function:: std::exception_ptr get_ptr () const

      Throws :class:`bad_expected_type` if the |expected| is valid.

      :returns: The exception pointer managed by the |expected|
      :throws: :class:`bad_expected_type`
      :noexcept: false

   .. function:: value_or<U>(U&& value)

      Returns the object managed by the |expected| if it is valid, otherwise
      *value* is returned. :type:`value_type` must be constructible with
      *U*.

      Behavior differs depending on whether |expected| is an lvalue or
      rvalue. When it is an lvalue, the managed object (if any) is copied
      and returned. When it is an rvalue, the managed object is moved out of
      the |expected|.

   .. function:: void swap (expected& that) noexcept

      :noexcept: ``std::is_nothrow_move_constructible<value_type>`` and if
                 ``std::swap`` for :type:`value_type` is *also* noexcept.

      If both |expected| are valid, then their values are swapped.
      If both |expected| are invalid, then their exception_ptr's are swapped

      Otherwise, the valid and invalid state between both |expected| is swapped
      and the 'valid' object is moved into the 'invalid' object, and vice versa

   .. function:: E expect<E> () const

      :noexcept: false

      This function will attempt to extract the given exception type *E*. If
      the |expected| is valid, :class:`bad_expected_type` is thrown. If the
      |expected| is invalid, but the given type *E* is *not* the correct 
      exception type, :class:`std::nested_exception` with
      :class:`bad_expected_type` and the actual exception are thrown.

   .. function:: void raise () const

      :noexcept: false
      :attributes: *noreturn*

      Throws the |expected| exception if the exception is invalid, otherwise
      throws :class:`bad_expected_type`. This function *always* throws and
      never returns.

.. class:: expected<void>

   |expected-v| is provided as a way to have the same semantics as |expected|,
   but for functions that do not (or cannot) return a value. It's interface is
   close to that of |expected|, however as it cannot store a value, it is
   smaller and only has member functions related to handling errors and
   exceptions.

   .. type:: value_type

      Always ``void``. Provided for generic functions to allow for compile time
      selection and specialization of functions.

   .. function:: explicit expected (std::exception_ptr) noexcept

      Initializes (and invalidates) the |expected-v|.

   .. function:: expected (expected const& that) noexcept
                 expected (expected&& that) noexcept

      Copies or moves the exception_ptr stored within the |expected-v|.

   .. function:: expected& operator = (std::exception_ptr) noexcept

      Invalidates the |expected-v| if it isn't already. If the |expected-v| is
      invalid already, it will now store the incoming exception_ptr.

   .. function:: expected& operator = (expected const&) noexcept
                 expected& operator = (expected&&) noexcept

      Assigns the contents of the incoming |expected-v| to ``*this``.

   .. function:: operator bool () const noexcept

      Works like |expected|'s explicit conversion operator. This function is
      *also* explicit.

   .. function:: std::exception_ptr get_ptr () const noexcept

      Returns the exception_ptr managed by the |expected-v|. May return a
      null exception_ptr. This differs from |expected| which may throw
      an exception if it is valid.

   .. function:: void swap (expected&) noexcept

      Swaps the |expected-v|'s exception_ptr's.

   .. function:: E expect<E> () const

      See :func:`expected\<T>::expect\<E>`

   .. function:: void raise () const

      See :func:`expected\<T>::raise`

.. function:: bool operator == (expected<T> const&, expected<T> const&)

   If both sides are valid, returns ``lhs.value() == rhs.value()``.
   Otherwise returns false.

.. function:: bool operator == (expected<T> const&, std::exception_ptr)

   If left hand side is valid, returns false.
   Otherwise compares the |expected| managed exception_ptr and the given
   ptr.

.. function:: bool operator == (std::exception_ptr, expected<T> const&)

   If right hand side is valid, returns false
   Otherwise returns whether the |expected| holds the same exception_ptr

.. function:: bool operator == (expected<T> const&, T const&)

   If the left hand side is invalid, returns false
   Otherwise returns ``lhs.value() == value``

.. function:: bool operator == (T const&, expected<T> const&)

   If the right hand side is invalid, returns false
   Otherwise returns ``value == rhs.value()``.

.. function:: bool operator < (expected<T> const&, expected<T> const&)

   If the left hand side is invalid, returns false.
   If the right hand side is invalid, return true.
   Otherwise returns ``std::less<T> { }(lhs.value(), rhs.value())``.

.. function:: bool operator < (expected<T> const&, T const&)

   If the left hand side is invalid, returns true
   Otherwise returns ``std::less<T> { }(lhs.value(), value)``

.. function:: bool operator == (expected<void> const&, expected<void> const&)

   Returns whether the |expected-v| have the same exception_ptr.

.. function:: auto make_expected<T>(T&& value) noexcept

   Returns a valid |expected| containing a T constructed with *value*.

.. function:: auto make_expected<T>(std::exception_ptr) noexcept

   Returns an invalidated |expected| containing the given exception_ptr. Can
   be easily used in conjunction with ``std::make_exception_ptr``::

      auto invalid_expected = make_expected<int>(
        std::make_exception_ptr(std::logic_error { "something went wrong" })
      );

Specializations
---------------

.. namespace:: std

.. function:: void swap(expected<T>& lhs, expected<T>& rhs)

   :noexcept: ``noexcept(lhs.swap(rhs))``

   Calls ``lhs.swap(rhs)``.
