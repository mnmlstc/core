Optional Component
==================

.. namespace:: core

.. index::
    single: optional
    single: expected
    single: result

The optional component contains several types that store an optional value.
Arguably the most well known is the :any:`optional` type (for which this
component is named). :any:`optional` is available in Boost. However, this
implementation of :any:`optional` follows the revision 5 proposal (
:wg21:`N3793`) as closely as possible (there are several constexpr issues in
older compilers)

In addition to :any:`optional`, an :any:`expected` type is provided. This type
is based off of a type mentioned by Andrei Alexandrescu in his 2012 talk
*Systematic Error Handling in C++*. Instead of an optional null-state, the
:any:`expected` type contains either a type :samp:`{T}`, or an exception.
It differes significantly from Alexandrescu's talk in that it's interface
closely resembles :any:`optional`. It also has the ability to *extract* the
contained exception without the need of a try-catch block placed by the user,
as well as the ability to rethrow the contained exception (if any such
exception exists). Additionally, a specialization for :any:`expected\<void>`
exists, allowing a function that normally returns :cxx:`void`, (but one that
may throw an exception) to return the error by way of :any:`expected`.

Lastly, there is a third optional type provided, named :any:`result`. This type
is closely related to :any:`expected`. However, it does not contain an
exception, but rather a :cxx:`std::error_condition`. This type was partially
inspired by the Rust language's ``Result<T, E>``. However, the :any:`result`
type is intended to model non-exception, portable error conditions to check
against, hence it holding either a instance of some type :samp:`{T}` or a
non-zero :cxx:`std::error_condition`. Much like :any:`expected\<void>`, a
:any:`result\<void>` is also provided to easily allow checking for a non-zero
:cxx:`std::error_condition` in a function that ideally would return
:cxx:`void`. This type was provided to allow generic use of :any:`result`.
*Technically*, this type can be replaced with a :cxx:`std::error_condition`.
However it removes the ability to mark a function as returning :cxx:`void`.

The optional component resides in the :file:`<core/{optional}.hpp>` header.

.. class:: in_place_t

   :any:`in_place_t` is an empty class type used to disambiguate the overloads
   and member functions of :any:`optional` that take arguments (such as
   :any:`~optional\<T>::emplace`) for in-place construction of some value.

   An object of this type is available under the name :samp:`{in_place}`.

.. class:: nullopt_t

   :any:`nullopt_t` is an empty class type used to indicate an :any:`optional`
   type with uninitialized state.

   An object of this type is available under the name :samp:`{nullopt}`.

.. index::
   single: optional; exceptions
   single: exceptions; bad_optional_access

.. class:: bad_optional_access

   :inherits: std::logic_error

   Thrown when accessing an :any:`optional` that is in a disengaged state.

   Accessing an :any:`optional` in a disengaged state does not refer to
   accessing the underlying object. Specifically it means calling
   :any:`optional` member functions, and not member functions of the underlying
   object it may or may not currently manage.

   This type is unavailable if :c:macro:`CORE_NO_EXCEPTIONS` is defined.


.. index::
   single: expected; exceptions
   single: exceptions; bad_expected_type

.. class:: bad_expected_type

   :inherits: std::logic_error

   Thrown when calling :any:`~expected\<T>::expect`, if the expected type
   is incorrect. Also thrown when attempting to expect or raise an exception
   when an :any:`expected` is in a *valid* state (that is, it does not
   currently manage an exception)

   This type is unavailable if :c:macro:`CORE_NO_EXCEPTIONS` is defined.

.. index::
   single: result; exceptions
   single: exceptions; bad_result_condition

.. class:: bad_result_condition

   :inherits: std::logic_error

   Thrown when attempting to access the :cxx:`std::error_condition` of a
   :any:`result` in a *valid* state. A :any:`result` will **never** contain a
   :cxx:`std::error_condition` with a value of 0, and will only be *invalid* if
   it actively manages a :cxx:`std::error_condition`.

   This type is unavailable if :c:macro:`CORE_NO_EXCEPTIONS` is defined.

Optional Type
-------------

.. class:: template <class T> optional

   The :any:`optional` manages an *optional* value. This value may be in either an
   initialized state, or an uninitialized state. This value is guaranteed to be
   allocated within the :any:`optional`. Instead of modelling a pointer, such as
   :samp:`std::unique_ptr<{T}>` or :samp:`std::shared_ptr<{T}>`,
   :any:`optional` models an object, even though
   :any:`~optional\<T>::operator->` and :any:`~optional\<T>::operator*` are
   provided.

   .. versionadded:: 1.1

      :any:`optional` follows the :wg21:`N3793` proposal. This means
      :any:`optional` is now usable as a constexpr-able type. Additionally,
      :any:`optional` now has the other comparison operators available. These
      are implemented in terms of :any:`~optional\<T>::operator ==` and
      :any:`~optional\<T>::operator <`.

   An :any:`optional` object is *engaged* when one of the following occurs:

    * The object is initialized with a value of type T
    * The object is assigned an *engaged* :any:`optional`.

   An :any:`optional` object is *disengaged* when one of the following occurs:

    * The object is default-initialized.
    * The object is initialized with an instance of :class:`nullopt_t`  or a
      *disengaged* :any:`optional`.
    * The object is assigned with an instance of :class:`nullopt_t` or a
      *disengaged* :any:`optional`.


   .. type:: value_type

      Represents the underlying type stored within an :any:`optional`.

      An :any:`optional`'s :any:`value_type` *may not* be:

         * :any:`in_place_t`
         * :any:`nullopt_t`
         * :cxx:`std::nullptr_t`
         * :cxx:`void`
         * any type for which :samp:`std::is_reference<{T}>::value` is *true*.
         * any type for which :samp:`std::is_object<{T}>::value` is *false*

   .. function:: optional (optional const& that)

      Copies the contents of the incoming :any:`optional`. If :samp:`{that}` is
      engaged, the contents of it are initialized into the new :any:`optional`
      object.

   .. function:: optional (optional&& that)

      Constructs a new :any:`optional` by moving the state of the incoming
      :any:`optional`. If the incoming :any:`optional` is engaged, its contents
      will be moved into the new object. The effects of :samp:`bool({that})`
      will remain the same.

      :noexcept: :samp:`std::is_nothrow_move_constructible<{value_type}>`

   .. function:: constexpr optional (nullopt_t) noexcept
                 constexpr optional () noexcept

      Constructs a new :any:`optional` in a disengaged state.

   .. function:: constexpr optional (value_type const& value)
                 constexpr optional (value_type&& value)

      Constructs a new :any:`optional` into an *engaged* state with the\
      contents of :samp:`{value}`.

      :noexcept: :samp:`std::is_nothrow_move_constructible<{value_type}>`

   .. function:: explicit constexpr optional (      \
                   in_place_t,                      \
                   std::initializer_list<U> ilist,  \
                   Args&&... args)
                 explicit constexpr optional (in_place_t, Args&&... args)

      Constructs a new :any:`optional` into an *engaged* state by constructing
      a :any:`value_type` in place with :samp:`{ilist}` or :samp:`{args}`.

   .. function:: optional& operator = (optional const& that)
                 optional& operator = (optional&& that)

      :noexcept: :samp:`std::is_nothrow_move_assignable<{value_type}>` and
                 :samp:`std::is_nothrow_move_constructible<{value_type}>`.

      Copies or moves the state of :samp:`{that}` into the :any:`optional`.

   .. function:: optional& operator = (T&& value)

      This assignment operator is only valid if :any:`value_type` is
      constructible *and* assignable from :samp:`{value}`.

      If the :any:`optional` is *disengaged*, it will be placed into an
      *engaged* state afterwards. If the :any:`optional` is already engaged, it
      will call the assignment constructor of :any:`value_type`.

   .. function:: optional& operator = (nullopt_t)

      If the :any:`optional` is in an engaged state, it will be placed into a
      *disengaged* state.

   .. function:: constexpr value_type const* operator -> () const
                 value_type* operator -> ()

      Accessing the managed object when the :any:`optional` is in a disengaged
      state will result in undefined behavior.

      :returns: pointer to the object managed by the :any:`optional`

   .. function:: constexpr value_type const& operator * () const
                 value_type& operator * ()

      If the :any:`optional` does not manage an object, dereferencing the 
      :any:`optional` will result in undefined behavior.

      :returns: An lvalue reference to the object managed by the
                :any:`optional`

   .. function:: constexpr explicit operator bool () const noexcept

      :returns: true if the :any:`optional` is *engaged*, false otherwise.

   .. function:: constexpr value_type value_or (U&& value) const &
                 value_type value_or (U&& value) &&

      :returns: the object managed by :any:`optional` or a :type:`value_type`
                constructed from *value*.

      If the :any:`optional` is an lvalue reference the :any:`value_type` will
      be copy constructed. If the :any:`optional` is an rvalue reference, the
      :any:`value_type` is move constructed.

   .. function:: constexpr value_type const& value () const
                 value_type& value ()

      :returns: reference to the object managed by :any:`optional`.
      :raises: :class:`bad_optional_access`

   .. function:: void swap (optional& that)

      Swaps the contents of :samp:`{that}` with the :any:`optional`. The
      behavior that is taken varies as such:

       * If neither the :any:`optional`, nor :samp:`{that}` are *engaged*, this
         function is a no-op.
       * If only one of either the :any:`optional` and :samp:`{that}` are
         *engaged*, the contained value of the *disengaged* object is
         initialized by moving the contained value of the *engaged* object.
         This is followed by the destruction of the originally *engaged*
         object's value. The state of both objects has been switched.
       * If both the :any:`optional` and :samp:`{that}` are *engaged*, their
         contained values are swapped.

   .. function:: void emplace (std::initializer_list<U> ilist, Args&&... args)
                 void emplace (Args&&... args)

      Constructs an object to be managed by the :any:`optional`. If the
      :any:`optional` is already engaged, it will first destruct the object it
      is currently managing.

   .. function:: constexpr auto visit (Visitor&& visitor, Args&&... args)
                 auto visit (Visitor&& visitor, Args&&... args)

      This is an extension function that allows *visitation* of an
      :any:`optional`, that also places its interface more in line with
      :any:`variant`.

      :returns: The common type of calling :samp:`{visitor}` with
                :any:`value_type` or :any:`nullopt_t` and :samp:`{args}`.

   .. function:: auto match (Visitors&&... visitors)

      An alternate choice for *visitation*, :any:`match` permits passing
      two lambdas to do basic pattern matching at runtime.


Expected Type
-------------

.. class:: template <class T> expected

   .. deprecated:: 2.0

   :any:`expected` works much like :any:`optional` in that it contains an
   optionally instantiated type :samp:`{T}`. However, unlike :any:`optional` it
   is never in a *disengaged* state. Instead its managed object is either
   *valid* or *invalid*. Like :any:`optional` it does not model a pointer, but
   rather an object and provides the pointer access operator overloads for
   convenience.

   This type is unavailable if :c:macro:`CORE_NO_EXCEPTIONS` is defined.
   
   :any:`expected` does *not* implement the interface proposed in :wg21:`N4015`.
   :any:`expected` was originally written over a year in advance of the
   proposal, and any work on it was unknown to the author of this library
   component.

   An :any:`expected` object is *valid* when one of the following occurs:

    * The object is initialized with a value of type :samp:`{T}`
    * The object is constructed with a *valid* :any:`expected`
    * The object is assigned a *valid* :any:`expected`
    * The object is default-initialized.

   An :any:`expected` object is *invalid* when one of the following occurs:

    * The object is initialized with a :cxx:`std::exception_ptr` or an
      *invalid* :any:`expected`.
    * The object is assigned a :cxx:`std::exception_ptr` or an *invalid*
      :any:`expected`.

   .. versionadded:: 1.1 In addition to ``operator ==`` and ``operator <``,
      :any:`expected` is now comparable via the other relational operators.

   .. type:: value_type

      Represents the given type :samp:`{T}`.

      .. warning:: An :any:`expected`'s :type:`value_type` *may not* be:

         * :class:`in_place_t`
         * :class:`nullopt_t`
         * ``std::exception_ptr``
         * any type for which ``std::is_reference<T>::value`` is *true*.
         * any type for which ``std::is_object<T>::value`` is *false*.

   .. function:: explicit expected (std::exception_ptr) noexcept

      Initializes the :any:`expected` with the given exception_ptr. The :any:`expected`
      is then placed into an *invalid* state.

   .. function:: expected (value_type const&)
                 expected (value_type&&)

      Initializes the :any:`expected` with the given value. Afterwards, the
      :any:`expected` is in a *valid* state.

   .. function:: expected (expected const&)
                 expected (expected&&)

      Initializes the :any:`expected` base on the incoming :any:`expected`'s valid state.
      The state of the incoming :any:`expected` does not change.

   .. function:: expected ()

      Default initializes the :any:`expected` to be in a *valid* state. This
      default constructs a :type:`expected\<T>::value_type` inside the
      :any:`expected`.

   .. function:: explicit expected (in_place_t, std::initializer_list<U>, Args)
                 explicit expected (in_place_t, Args)

      Constructs a new :any:`expected` into an *engaged* state by constructing a
      :type:`value_type` in place with the given arguments.

   .. function:: expected& operator = (expected const&)
                 expected& operator = (expected&&)

      Assigns the content of the incoming :any:`expected` to ``*this``.

   .. function:: expected& operator = (value_type const&)
                 expected& operator = (value_type&&)

      Initializes the :any:`expected` with the assigned value. If the :any:`expected`
      holds an exception_ptr, it is destructed, and the
      :type:`expected\<T>::value_type` is initialized (*not assigned*) the
      incoming value.

   .. function:: expected& operator = (std::exception_ptr)

      If the :any:`expected` is in a *valid* state, it will be placed into an
      *invalid* state.

   .. function:: void swap (expected& that)

      :noexcept: ``std::is_nothrow_move_constructible<value_type`` and 
                 ``core::is_nothrow_swappable<value_type>``.

      If both :any:`expected` are *valid*, then their values are swapped.
      If both :any:`expected` are *invalid*, then their exception_ptrs are swapped.

      Otherwise, the *valid* and *invalid* state between both :any:`expected` is
      swapped and the *valid* object is moved into the *invalid* object, and
      vice versa.

   .. function:: explicit operator bool () const noexcept

      :returns: Whether the :any:`expected` is *valid* or *invalid*.

   .. function:: value_type const* operator -> () const noexcept
                 value_type* operator -> () noexcept

      Accessing the managed object when the :any:`expected` is *invalid* will
      result in undefined behavior.

      .. versionadded:: 1.1

      :returns: pointer to the object managed by the :any:`expected`

   .. function:: value_type const& operator * () const noexcept
                 value_type& operator * () noexcept

      :returns: The object managed by the :any:`expected`. Accessing this object
                when the :any:`expected` is *invalid* will result in undefined
                behavior.

   .. function:: value_type const& value () const
                 value_type& value ()

      :returns: The object managed by :any:`expected`
      :throws: The exception managed by :any:`expected` if the :any:`expected`
      :noexcept: ``false``

   .. function:: value_type value_or (U&& value) const &
                 value_type value_or (U&& value) &&

      :returns: The object managed by :any:`expected` if *valid*, otherwise, *value*
                is returned. This function will not compile if *U* is not
                convertible to :type:`expected\<T>::value_type`.

   .. function:: void emplace (std::initializer_list<U>, Args)
                 void emplace (Args)

      .. versionadded:: 1.1

      Constructs the object managed by :any:`expected`. If the :any:`expected` is
      already *valid*, it will first destruct the object it is currently
      managing.

   .. function:: E expect () const

      :noexcept: ``false``

      This function attempts to extract the given exception type *E*. If
      :any:`expected` is *valid*, :class:`bad_expected_type` is thrown. If
      :any:`expected` is *invalid*, but *E* is not the correct exception type,
      ``std::nested_exception`` with :class:`bad_expected_type` and the actual
      exception are thrown. Otherwise, the exception is returned by value.

   .. function:: void raise () const

      :noexcept: ``false``
      :attributes: ``[[noreturn]]``.

      Throws the :any:`expected`'s managed exception if *invalid*. Otherwise, throws
      :class:`bad_expected_type`. This function *always* throws, and will never
      return.

   .. function:: std::exception_ptr pointer () const

      This function will throw if :any:`expected` is *invalid*.

      .. versionadded:: 1.1 Replaces :func:`get_ptr`.

      :returns: The exception pointer managed by :any:`expected`
      :throws: :class:`bad_expected_type`
      :noexcept: ``false``

   .. function:: std::exception_ptr get_ptr () const

      .. deprecated:: 1.1 Use :func:`pointer` as a replacement.

      :returns: The exception pointer managed by :any:`expected`
      :throws: :class:`bad_expected_type`
      :noexcept: ``false``

Result Type
-----------

.. class:: template <class T> result

   .. versionadded:: 1.1
   .. deprecated:: 2.0

   :any:`result` works much like :any:`expected`. However, it does not manage an
   exception, but rather a ``std::error_condition``. This is done to provide a
   nice rounding out for functions which may want to signal an error, but not
   require the 'output' value to be passed by reference or by pointer.

   A :any:`result` object is *valid* when one of the following occurs:

    * The object is initialized with a value of type *T*.
    * The object is constructed with a *valid* :any:`result`.
    * The object is assigned a *valid* :any:`result`.
    * The object is default initialized.
    * The object is constructed with a ``std::error_condition`` whose value
      is 0.
    * The object is assigned a ``std::error_condition`` whose value is 0.

   A :any:`result` object is *invalid* when one of the following occurs:
    * The object is initialized with a non-zero ``std::error_condition``.
    * The object is assigned a non-zero ``std:error_condition``.

   .. type:: value_type

      Represents the given type *T*.

      .. warning:: A :any:`result`'s :type:`value_type` *may not* be:

         * :class:`in_place_t`
         * :class:`nullopt_t`
         * ``std::error_condition``
         * any type for which ``std::is_error_condition_enum<T>::value`` is
           *true*
         * any type for which ``std::is_reference<T>::value`` is *true*.
         * any type for which ``std::is_object<T>::value`` is *false*.
         * any type for which ``std::is_nothrow_destructible<T>::value`` is
           *false*

   .. function:: result (int val, ::std::error_category const& cat) noexcept
                 result (std::error_condition const& condition) noexcept
                 result (ErrorConditionEnum e) noexcept

      Initializes the :any:`result` to be *invalid*. The third overload may be
      passed any value for which ``std::is_error_condition_enum`` is *true*.

   .. function:: result (value_type const& value)
                 result (value_type&& value)

      Initializes :any:`result` into a *valid* state with the given *value*. The
      move constructor is marked ``noexcept`` *only* if
      ``std::is_nothrow_move_constructible`` is *true* for :type:`value_type`.

   .. function:: result (in_place_t p, std::initializer<U> il, Args&& args)
                 result (in_place_t p, Args&& args)

      Initializes :any:`result` into a *valid* state by constructing a
      :type:`value_type` in place with the given arguments. These constructors
      only participate if :type:`value_type` is constructible with the given
      arguments. *args* is a variadic template of arguments.

   .. function:: result (result const& that)
                 result (result&& that)

      Copies or moves the state stored in *that* into :any:`result` as well as its
      managed value or error condition.

   .. function:: result ()

      Initializes :any:`result` into a *valid* state by default constructing a
      :type:`value_type`.

   .. function:: result& operator = (result const& that)
                 result& operator = (result&& that)

      Assigns the contents and state of *that* to :any:`result`. If the state of
      *that* and :any:`result` differ, the condition or object managed by :any:`result`
      will be destroyed and :any:`result`'s state will then be constructed with
      the data stored in *that*.

   .. function:: result& operator = (std::error_condition const& condition)
                 result& operator = (ErrorConditionEnum e)

      Assigns the given *condition* or error condition enum value *e* to
      :any:`result`. If :any:`result` is in a *valid* state, its managed object will
      be destructed, and the incoming value assigned. If *condition* or *e*
      would result in a default constructed :type:`value_type`, (such as
      ``bool(condition) == false``), the managed object is still destructed
      and :any:`result` will then be assigned a default constructed
      :type:`value_type`.

   .. function:: result& operator = (value_type const& value)
                 result& operator = (value_type&& value)
                 result& operator = (U&& value)

      Assigns *value* to :any:`result`. If :any:`result` is in a *valid* state, its
      managed object is also assigned *value*. If it is in an *invalid* state,
      it will then destroy the stored condition, and then place the :any:`result`
      into a valid state.

      The third overload requires that :type:`value_type` be assignable and
      constructible from *U*.

   .. function:: void swap (result& that)

      Swaps the state of *that* with :any:`result`. If both *that* and :any:`result`
      are *valid*, then they swap their managed objects. If both *that* and
      :any:`result` are *invalid*, they swap their managed conditions.
      If their states differ, the *invalid* instance is constructed with the
      contents of the *valid* instance via move construction. The *valid*
      instance is then invalidated with the previously *invalid* instance's
      condition.

      :noexcept: ``std::is_nothrow_move_constructible<value_type>`` and
                 ``core::is_nothrow_swappable<value_type>``

   .. function:: explicit operator bool () const noexcept

      :returns: Whether :any:`result` is *valid* or *invalid*.


   .. function:: value_type const& operator * () const noexcept
                 value_type& operator * () noexcept

      Calling this function when :any:`result` is *invalid* will result in
      undefined behavior.

      :returns: The object managed by :any:`result`.

   .. function:: value_type const* operator -> () const noexcept
                 value_type* operator -> () noexcept

      Calling this function when :any:`result` is *invalid* will result in
      undefined behavior.

      :returns: The address of the object managed by :any:`result`

   .. function:: void emplace (std::initializer_list<T> il, Args&& args)
                 void emplace (Args&& args)

      Destroys whatever state is managed by :any:`result` and then reinitializes
      it to be *valid* while constructing a :type:`value_type` with the given
      arguments. *args* is a variadic template argument.

   .. function:: value_type const& value () const
                 value_type& value ()

      If :any:`result` is *invalid*, this function will throw a ``std::system_error``
      exception with the managed error condition.

      :returns: Object managed by :any:`result`
      :throws: ``std::system_error``

   .. function:: value_type value_or (U&& value) const&
                 value_type value_or (U&& value) &&

      If :any:`result` is in an *invalid* state, a :type:`value_type` converted
      from *value* is returned. Otherwise, the :any:`result`'s managed object is
      copied or moved into the returning value, depending on whether :any:`result`
      is an rvalue or const lvalue reference.

      :requires: :type:`value_type` be move or copy constructible
                 and that *U* is convertible to :type:`value_type`.

      :returns: :type:`value_type`

   .. function:: std::error_condition const& condition () const

      If :any:`result` is *invalid*, the condition it manages is returned. Otherwise
      an exception is thrown.

      :returns: ``std::error_condition`` managed by :any:`result`
      :noexcept: false
      :throws: :class:`bad_result_condition`

Functions
---------

.. function:: template <class T> optional<T> make_optional(T&& value)

   :raises: Any exceptions thrown by the constructor of T

   Creates an :any:`optional` object from value. Effectively calls::

       optional<typename std::decay<T>::type>(std::forward<T>(value));

   Due to a bug in Apple Clang-503.0.40, this function is *not* marked
   constexpr, and this causes an incompatibility with :wg21:`N3793`.

.. function:: template <class T> expected<T> make_expected (T&& value)
              template <class T> expected<T> make_expected (E&& exception)
              template <class T> expected<T> make_expected (std::exception_ptr)

   .. versionadded:: 1.1 The overload version which takes exception type *E*

   The first overload returns a *valid* :any:`expected` containing a T constructed
   with *value*. The second overload returns an *invalid* :any:`expected` with
   an exception_ptr to *exception*. For this version to be usable, *E* must
   inherit from ``std::exception``. The third overload takes an exception
   pointer and returns an *invalid* :any:`expected` from it.

.. function:: template <class T> result<T> make_result (T&& value)
              template <class T> result<T> make_result (std::error_condition cnd)
              template <class T> result<T> make_result (ErrorConditionEnum e)

   .. versionadded:: 1.1

   The first overload returns a *valid* :any:`result` containing a T constructed
   with *value*. The second overload returns an *invalid* :any:`expected` with an
   error_condition. The last overload takes any type for which
   ``std::is_error_condition_enum`` is *true*.


Operators
^^^^^^^^^

.. function:: bool operator == (optional const&, optional const&) noexcept
              bool operator == (optional const&, nullopt_t) noexcept
              bool operator == (nullopt_t, optional const&) noexcept
              bool operator == (optional<T> const&, T const&) noexcept
              bool operator == (T const&, optional<T> const&) noexcept

   For the first overload, if only one of the given :any:`optional` values is
   *engaged*, it will return false. If both :any:`optional` values are 
   *disengaged*, it will return true. Otherwise the :any:`optional` values compare
   their managed objects with ``operator ==``

   The second overload returns whether or not the :any:`optional` value is *engaged*.
   The third overload *always* returns false.
   The fourth and fifth overloads will check if the :any:`optional` value is
   *engaged*. If it is, the object managed by :any:`optional` will be compared
   with ``operator ==``. Otherwise it will return false.

.. function:: bool operator < (optional<T> const&, optional<T> const&) noexcept
              bool operator < (optional<T> const&, nullopt_t) noexcept
              bool operator < (nullopt_t, optional<T> const&) noexcept
              bool operator < (optional<T> const&, T const&) noexcept
              bool operator < (T const&, optional<T> const&) noexcept

   For the first overload, if the right :any:`optional` is *disengaged*, it will
   return false. If the left :any:`optional` is *disengaged*, it will return true.
   Otherwise, the result of ``*lhs < *rhs`` is returned.

   The second overload returns true if the :any:`optional` is *disengaged*.
   The third overload returns true if the :any:`optional` is *engaged*.
   The fourth optional returns true if the :any:`optional` is *disengaged*.
   The fifth optional returns false if the :any:`optional` is *disengaged*.
   Otherwise the result ``*opt < value`` or ``value < *opt`` is returned.

.. note:: The rest of the relational operators for :any:`optional` are (mostly)
   implemented in terms of ``operator ==`` and ``operator <``.

.. function:: bool operator == (expected const&, expected const&) noexcept
              bool operator == (expected const&, exception_ptr) noexcept
              bool operator == (exception_ptr, expected const&) noexcept
              bool operator == (expected const&, T const&) noexcept
              bool operator == (T const&, expected const&) noexcept

   .. versionchanged:: 1.1 The comparison of an :any:`expected` to an exception_ptr
      no longer compare the actual underlying exception_ptr if the :any:`expected`
      is *invalid*. Comparing an :any:`expected` to a ``std::exception_ptr`` now
      works as though one compared an :any:`optional` to ``nullopt``.

   For the first overload if only one of the :any:`expected` values is *valid*,
   it will return ``false``. If both :any:`expected` values are *invalid*, it will
   return ``true`` Otherwise, the :any:`expected` values compare their managed
   objects with ``operator ==``.

   The second and third overload return ``true`` if the :any:`expected` value is
   *invalid*.

   The fourth and fifth overload returns ``true`` only if the :any:`expected` value
   is *valid* and its managed object compares equal wth the *T* via *T*'s
   ``operator ==``.

.. function:: bool operator < (expected const&, expected const&) noexcept
              bool operator < (expected const&, exception_ptr) noexcept
              bool operator < (exception_ptr, expected const&) noexcept
              bool operator < (expected const&, T const&) noexcept
              bool operator < (T const&, expected const&) noexcept

   For the first overload, if the right :any:`expected` is *invalid*, it will
   return ``false``. If the left :any:`expected` is *invalid* it will return
   ``true``. If both :any:`expected` objects are *valid*, then their managed values
   are compared via ``operator <``.

   The second overload returns ``true`` if the :any:`expected` is *invalid*.
   The third overload returns ``true`` if the :any:`expected` is *valid*.
   The fourth overload returns ``true`` if the :any:`expected` is *invalid*.
   The fifth overload returns ``false`` if the :any:`expected` is *invalid*.
   Otherwise the result of ``*exp < value`` or ``value < *exp`` is returned.

.. note:: The rest of the relational operators for :any:`expected` are implemented
   in terms of ``operator ==`` and ``operator <``.

.. function:: bool operator == (result const&, result const&)
              bool operator == (result const&, error_condition const&)
              bool operator == (error_condition const&, result const&)
              bool operator == (result const&, error_code const&)
              bool operator == (error_code const&, result const&)
              bool operator == (result const&, T const&)
              bool operator == (T const&, result const&)

   .. versionadded:: 1.1

   For the first overload if only one of the :any:`result` objects is *valid*,
   it will return ``false``. If both :any:`result` objects are *invalid*, the
   result of comparing their ``error_condition`` is returned. Otherwise, the
   :any:`result` values compare via ``operator ==``.

   The second, third, fourth, and fifth overload will return ``false`` if
   the :any:`result` object is *valid* (even if the ``std::error_condition`` or
   ``std::error_code`` were to return ``false`` in a boolean context. This was
   done to minimize issues with differing categories). If the :any:`result` is
   *invalid*, its :func:`result\<T>::condition` is compared against the
   ``std::error_condition`` or ``std::error_code`` via ``operator ==``.

   The sixth and seventh overloads will return ``false`` if :any:`result` is
   *invalid*. Otherwise the :any:`result` value is compared with the given *T*
   via ``operator ==``.

.. function:: bool operator < (result const&, result const&)
              bool operator < (result const&, error_condition const&)
              bool operator < (error_condition const&, result const&)
              bool operator < (result const&, T const&)
              bool operator < (T const&, result const&)

   .. versionadded:: 1.1

   For the first overload, if both :any:`result` objects are *invalid*, the
   ``operator <`` comparison of their :func:`result\<T>::condition` are
   returned. If both :any:`result` objects are *valid*, the comparison of their
   values via ``operator <`` is returned. If the :any:`result` on the left is
   *invalid*, but the :any:`result` on the right is not, ``true`` is returned.
   Otherwise ``false``.

   The second overload returns ``false`` if the :any:`result` is *valid* (even
   if the ``std::error_condition`` would evaluate to ``false`` in a boolean
   context. This was done to minimize issues with differing categories). If
   the :any:`result` is *invalid*, its :func:`result\<T>::condition` is compared
   against the ``std::error_condition`` via ``operator <``

   Conversely, the third overload returns ``true`` if the :any:`result` is *valid*.
   If the :any:`result` is *invalid*, its :func:`result\<T>::condition` is compared
   against the ``std::error_condition`` via ``operator <``.

   For the fourth overload, if the :any:`result` is *invalid*, ``false`` is returned.
   Otherwise, the comparison of the :any:`result` value and *T* via ``operator <``
   is returned.

   For the fifth overload, if the :any:`result` is *invalid*, ``true`` is returned.
   Otherwise, the comparison of the :any:`result` value and *T* via ``operator <``
   is returned.

.. note:: The rest of the relational operators for :any:`result` are implemented
   in terms of ``operator ==`` and ``operator <``.

Swap
^^^^

.. function:: void swap (optional& lhs, optional& rhs)
              void swap (expected& lhs, expected& rhs)
              void swap (result& lhs, result& rhs)

   These swap functions are provided to allow for ADL calls to swap. They
   are all equivalent to calling :samp:`{lhs}.swap({rhs})`.

Specializations
---------------

.. class:: template <> expected<void>

   .. deprecated:: 2.0

   :any:`expected\<void>` is provided as a way to have the same semantics as
   :any:`expected`, but for functions that do not (or cannot) return a value.
   Its interface is close to that of :any:`expected`, however as it cannot
   store a value, it is smaller and only has member functions related to
   handling the exception stored within the :any:`expected\<void>`.

   .. type:: value_type

      Always :cxx:`void`.

   .. function:: explicit expected (std::exception_ptr ptr) noexcept

      Initializes (and invalidates) the :any:`expected\<void>` if :samp:`{ptr}`
      is a non-null :cxx:`std::exception_ptr`.

   .. function:: expected (expected const& that) = default
                 expected (expected&& that) = default

      Copies the :cxx:`std::exception_ptr` stored within :samp:`{that}`.

   .. function:: expected& operator = (expected const& that) = default
                 expected& operator = (expected&& that) = default

      Copies the :cxx:`std::exception_ptr` stored within :samp:`{that}`. The
      :any:`expected\<void>` will then hold the same state of validity as
      :samp:`{that}`.

   .. function:: void swap (expected& that) noexcept

      Swaps the managed :cxx:`std::exception_ptr` of :samp:`{that}` with the
      :any:`expected\<void>`.

   .. function:: explicit operator bool () const noexcept

      :returns: Whether the :any:`expected\<void>` is *valid* or *invalid*.

   .. function:: template <class E> E expect () const

      See :any:`expected\<T>::expect`

   .. function:: void raise () const

      See :any:`expected<T>::raise`

   .. function:: std::exception_ptr pointer () const

      Throws if :any:`expected\<void>` is *valid*.

      :returns: The managed :cxx:`std::exception_ptr` if the
                :any:`expected\<void>` is *invalid*.
      :throws: :any:`bad_expected_type` if the :any:`expected\<void>` is
               *valid*.
      :noexcept: ``false``.

   .. function:: std::exception_ptr get_ptr () const

      .. deprecated:: 1.1 Use :any:`pointer` as a replacement.

      :returns: The managed :cxx:`std::exception_ptr` if the
                :any:`expected\<void>` is *invalid*.
      :throws: :any:`bad_expected_type` if the :any:`expected\<void>` is
               *valid*.
      :noexcept: :cxx:`false`.

.. class:: template <> result

   :any:`result\<void>` is provided as a way to have the same semantics as
   :any:`result`, but for functions that do not (or cannot) return a value. Its
   interface is close to that of :any:`result`, however as it cannot store an
   object, it is smaller and only has member functions related to handling
   error conditions.

   Technically speaking, this type is unnecessary as an error_condition can be
   supplied instead. However, it's sometimes nice to allow for more generic
   code to be written, and worrying about whether or not you might accidentally
   instantiate a :any:`result\<void>` isn't something one should have to worry
   about.

   The copy, move, and default constructors as well as the copy and move
   assignment operators for :any:`result\<void>` are defaulted, letting the
   stored :cxx:`std::error_condition` be managed instead.

   .. type:: value_type

      Always :cxx:`void`.

   .. function:: result (int value, std::error_category const& category)
                 result (std::error_condition cnd)
                 result (ErrorConditionEnum e)

      Constructs the :any:`result\<void>` with the given value. Any type for
      which :cxx:`std::is_error_condition_enum` is :cxx:`true` may be used as
      an argument in the third overload.

   .. function:: result& operator = (std::error_condition const& condition)
                 result& operator = (ErrorConditionEnum e) noexcept

      Assigns :any:`result\<void>` with the given value. Any value for which
      :cxx:`std::is_error_condition_enum` is :cxx:`true` may be used in the
      second overload.

   .. function:: explicit operator bool () const noexcept

      :returns: Whether the :any:`result\<void>` is *valid* or *invalid*.

   .. function:: void swap (result& that)

      Swaps the stored :cxx:`std::error_condition` with :samp:`{that}`.

   .. function:: std::error_condition const& condition () const

      If the :any:`result\<void>` is *valid*, it will throw an exception.
      Otherwise, the :cxx:`std::error_condition` stored will be returned.

      :noexcept: false
      :throws: :class:`bad_result_condition`
      :returns: The stored ``std::error_condition`` if :any:`result\<void>` is
                *invalid*.

std::hash
^^^^^^^^^

.. class:: template <class T> hash<optional<T>>

   Specialization of :cxx:`std::hash`.

   Requires that the :any:`optional\<T>::value_type` be specialized for
   :cxx:`std::hash`. If the :any:`optional` is engaged it will return the hash
   value for :cxx:`hash<value_type>`. Otherwise, it will return a default
   constructed :cxx:`std::hash<value_type>::result_type`.

.. class:: template <class T> hash<expected<T>>

   .. deprecated:: 2.0

   Specialization of :cxx:`std::hash`.

   Requires that the :any:`expected\<T>::value_type` be specialized for
   :cxx:`std::hash`. If the :any:`expected` is *valid*, it will return the hash value
   for :cxx:`hash<value_type>`. Otherwise, it will return a default constructed
   :cxx:`std::hash<value_type>::result_type`.

   This specialization is unavailable if :c:macro:`CORE_NO_EXCEPTIONS` is
   defined.

.. class:: template <class T> hash<result<T>>

   .. deprecated:: 2.0

   Specialization of :cxx:`std::hash`.

   Requires that the :any:`result\<T>::value_type` be specialized for
   :cxx:`std::hash`. If the :any:`result` is *valid*, it will return the hash
   value for :cxx:`hash<value_type>`. Otherwise, it will return a default
   constructed :cxx:`std::hash<value_type>::result_type`.
