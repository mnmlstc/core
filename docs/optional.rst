.. _core-optional-component:

Optional Component
==================

.. default-domain:: cpp

.. |optional| replace:: :class:`optional <optional\<T>>`
.. |expected| replace:: :class:`expected <expected\<T>>`
.. |result| replace:: :class:`result <result\<T>>`

.. |expected-v| replace:: :class:`expected\<void>`
.. |result-v| replace:: :class:`result\<void>`

The optional component contains several types that store an optional value.
Arguably the most well known is the |optional| type (for which this component
is named). |optional| is available in Boost. However, this implementation of
|optional| follows the revision 5 proposal (N3793_) as closely as possible.

In addition to |optional|, an |expected| type is provided. This type is based
off of a type mentioned by Andrei Alexandrescu in his 2012 talk *Systematic
Error Handling in C++*. Instead of an optional null-state, the |expected| type
contains either a type *T*, or an exception. It differes significantly from
Alexandrescu's talk in that it's interface closely resembles |optional|. It
also has the ability to *extract* the contained exception without the need
of a try-catch block placed by the user, as well as the ability to rethrow
the contained exception (if any such exception exists). Additionally, a
specialization for |expected-v| exists, allowing a function that normally
returns ``void``, (but one that may throw an exception) to return the error
by way of |expected|.

Lastly, there is a third optional type provided, named |result|. This type
is closely related to |expected|. However, it does not contain an exception,
but rather a ``std::error_condition``. This type was partially inspired by
the Rust language's ``Result<T, E>``. However, the |result| type is intended
to model non-exception, portable error conditions to check against, hence
it holding either a instance of type *T* or a non-zero ``std::error_condition``.
Much like |expected-v|, a |result-v| is also provided to easily allow checking
for a non-zero ``std::error_condition`` in a function that ideally would return
void. This type was provided to allow generic use of |result|. *Technically*,
this type can be replaced with a ``std::error_condition``. However it removes
the ability to mark a function as returning ``void``.

The optional component resides in the ``<core/optional.hpp>`` header.

.. namespace:: core

.. class:: in_place_t

   :class:`in_place_t` is an empty class type used to disambiguate
   the overloads and member functions of |optional| that
   take arguments (such as :func:`optional\<T>::emplace`) for
   in-place construction of some value.

   An object of this type is available under the name ``in_place``.

.. class:: nullopt_t

   nullopt_t is an empty class type used to indicate an
   |optional| type with uninitialized state.

   An object of this type is available under the name ``nullopt``.

.. class:: bad_optional_access

   :inherits: std::logic_error

   Thrown when accessing an |optional| that is in a disengaged state.

   .. note:: Accessing an |optional| in a disengaged state does not refer to
      accessing the underlying object. Specifically it means calling |optional|
      member functions, and not member functions of the underlying object it
      may or may not currently manage.

.. class:: bad_expected_type

   :inherits: std::logic_error

   Thrown when calling :func:`expected\<T>::expect`, if the expected type
   is incorrect. Also thrown when attempting to expect or raise an exception
   when an |expected| is in a *valid* state (that is, it does not currently
   manage an exception)

.. class:: bad_result_condition

   :inherits: std::logic_error

   Thrown when attempting to access the ``std::error_condition`` of a |result|
   in a *valid* state. A |result| will **never** contain a
   ``std::error_condition`` with a value of 0, and will only be *invalid* if
   it actively manages a ``std::error_condition``.

Optional Type
-------------

.. class:: optional<T>

   The |optional| manages an *optional* value. This value may be in either an
   initialized state, or an uninitialized state. This value is guaranteed to be
   allocated within the |optional|. Instead of modelling a pointer, such as
   ``std::unique_ptr<T>`` or ``std::shared_ptr<T>``, |optional| models an
   object, even though :func:`optional\<T>::operator->` and
   :func:`optional\<T>::operator*` are provided.

   .. versionadded:: 1.1

      |optional| follows the N3793_ proposal. This means |optional| is now
      usable as a constexpr-able type. Additionally, |optional| now has the
      other comparison operators available. These are implemented in terms of
      ``operator ==`` and ``operator <``.

   An |optional| object is *engaged* when one of the following occurs:

    * The object is initialized with a value of type T
    * The object is assigned an *engaged* |optional|.

   An |optional| object is *disengaged* when one of the following occurs:

    * The object is default-initialized.
    * The object is initialized with an instance of :class:`nullopt_t`  or a
      *disengaged* |optional|.
    * The object is assigned with an instance of :class:`nullopt_t` or a
      *disengaged* |optional|.


   .. type:: value_type

      Represents the underlying type stored within an |optional|.

      .. warning:: An |optional|'s :type:`value_type` *may not* be:

         * :class:`in_place_t`
         * :class:`nullopt_t`
         * ``std::nullptr_t``
         * ``void``
         * any type for which ``std::is_reference<T>::value`` is *true*.
         * any type for which ``std::is_object<T>::value`` is *false*

   .. function:: optional (optional const&)

      Copies the contents of the incoming |optional|. If the incoming
      |optional| is engaged, the contents of it are initialized into the
      new |optional| object.

   .. function:: optional (optional&& that)

      Constructs a new |optional| by moving the state of the incoming
      |optional|. If the incoming |optional| is engaged, its contents will be
      moved into the new object. The effects of ``bool(that)`` remain the same.

      :noexcept: ``std::is_nothrow_move_constructible<value_type>``

   .. function:: constexpr optional (nullopt_tr) noexcept
                 constexpr optional () noexcept

      Constructs a new |optional| in a disengaged state.

   .. function:: constexpr optional (value_type const&)
                 constexpr optional (value_type&&)

      Constructs a new |optional| into an *engaged* state with the contents of
      the value_type.

      :noexcept: ``std::is_nothrow_move_constructible<value_type>``

   .. function:: explicit constexpr optional (\
                   in_place_t,\
                   std::initializer_list<U>,\
                   Args\
                 )
                 explicit constexpr optional (in_place_t, Args)

      Constructs a new |optional| into an *engaged* state by constructing a
      :type:`value_type` in place with the variadic arguments *Args*.

   .. function:: optional& operator = (optional const&)
                 optional& operator = (optional&&)

      :noexcept: ``std::is_nothrow_move_assignable<value_type>`` and
                 ``std::is_nothrow_move_constructible<value_type>``.

      Assigns the state of the incoming |optional|. This is done by
      constructing an |optional|, and then calling :func:`swap` on it and
      ``*this``.

   .. function:: optional& operator = (T&& value)

      This assignment operator is only valid if :type:`value_type` is
      constructible *and* assignable from *value*.

      If ``*this`` is *disengaged*, it will be placed into an *engaged* state
      afterwards. If ``*this`` is already engaged, it will call the assignment
      constructor of :type:`value_type`.

   .. function:: optional& operator = (nullopt_t)

      If ``*this`` is in an engaged state, it will be placed into a
      *disengaged* state.

   .. function:: constexpr value_type const* operator -> () const
                 value_type* operator -> ()

      Accessing the managed object when the |optional| is in a disengaged state
      will result in undefined behavior.

      :returns: pointer to the object managed by the |optional|

   .. function:: constexpr value_type const& operator * () const
                 value_type& operator * ()

      If the |optional| does not manage an object, dereferencing the 
      |optional| will result in undefined behavior.

      :returns: An lvalue reference to the object managed by the |optional|

   .. function:: constexpr explicit operator bool () const noexcept

      :returns: true if the object is *engaged*, false otherwise.

   .. function:: constexpr value_type value_or (U&& value) const &
                 value_type value_or (U&& value) &&

      If ``*this`` is an lvalue reference the :type`value_type` will be copy
      constructed. If ``*this`` is an rvalue reference, the :type:`value_type`
      is move constructed.

      :returns: the object managed by |optional| or a :type:`value_type`
                constructed from *value*.

   .. function:: constexpr value_type const& value () const
                 value_type& value ()

      :raises: :class:`bad_optional_access`

   .. function:: void swap (optional& that)

      Swaps the contents of 'this' with the given object. The behavior that is
      taken varies as such:

       * If neither ``*this``, nor ``that`` are *engaged*, this function is a
         no-op.
       * If only one of either ``*this`` and ``that`` are *engaged*, the
         contained value of the *disengaged* object is initialized by moving
         the contained value of the *engaged* object. This is followed by the
         destruction of the originally *engaged* object's value. The state of
         both objects has been switched.
       * If both ``*this`` and ``that`` are *engaged*, their contained values
         are swapped with ``std::swap(**this, *that)``.

   .. function:: void emplace (std::initializer_list<U>, args)
                 void emplace (args)

      Constructs the object managed by the |optional|. If the |optional| is
      already engaged, it will first destruct the object it is currently
      managing.

Expected Type
-------------

.. class:: expected<T>

   |expected| works much like |optional| in that it contains an optionally
   instantiated type *T*. However, unlike |optional| it is never in a
   *disengaged* state. Instead its managed object is either *valid* or
   *invalid*. Like |optional| it does not model a pointer, but rather an object
   and provides the pointer access operator overloads for convenience.

   .. note:: |expected| does *not* implement the interface proposed in N4015_.
      |expected| was originally written over a year in advance of the proposal.

   An |expected| object is *valid* when one of the following occurs:

    * The object is initialized with a value of type *T*
    * The object is constructed with a *valid* |expected|
    * The object is assigned a *valid* |expected|
    * The object is default-initialized.

   An |expected| object is *invalid* when one of the following occurs:

    * The object is initialized with a ``std::exception_ptr`` or an *invalid*
      |expected|.
    * The object is assigned a ``std::exception_ptr`` or an *invalid*
      |expected|.

   .. versionadded:: 1.1 In addition to ``operator ==`` and ``operator <``,
      |expected| is now comparable via the other relational operators.

   .. type:: value_type

      Represents the given type *T*.

      .. warning:: An |expected|'s :type:`value_type` *may not* be:

         * :class:`in_place_t`
         * :class:`nullopt_t`
         * ``std::exception_ptr``
         * any type for which ``std::is_reference<T>::value`` is *true*.
         * any type for which ``std::is_object<T>::value`` is *false*.

   .. function:: explicit expected (std::exception_ptr) noexcept

      Initializes the |expected| with the given exception_ptr. The |expected|
      is then placed into an *invalid* state.

   .. function:: expected (value_type const&)
                 expected (value_type&&)

      Initializes the |expected| with the given value. Afterwards, the
      |expected| is in a *valid* state.

   .. function:: expected (expected const&)
                 expected (expected&&)

      Initializes the |expected| base on the incoming |expected|'s valid state.
      The state of the incoming |expected| does not change.

   .. function:: expected ()

      Default initializes the |expected| to be in a *valid* state. This
      default constructs a :type:`expected\<T>::value_type` inside the
      |expected|.

   .. function:: explicit expected (in_place_t, std::initializer_list<U>, Args)
                 explicit expected (in_place_t, Args)

      Constructs a new |expected| into an *engaged* state by constructing a
      :type:`value_type` in place with the given arguments.

   .. function:: expected& operator = (expected const&)
                 expected& operator = (expected&&)

      Assigns the content of the incoming |expected| to ``*this``.

   .. function:: expected& operator = (value_type const&)
                 expected& operator = (value_type&&)

      Initializes the |expected| with the assigned value. If the |expected|
      holds an exception_ptr, it is destructed, and the
      :type:`expected\<T>::value_type` is initialized (*not assigned*) the
      incoming value.

   .. function:: expected& operator = (std::exception_ptr)

      If the |expected| is in a *valid* state, it will be placed into an
      *invalid* state.

   .. function:: void swap (expected& that)

      :noexcept: ``std::is_nothrow_move_constructible<value_type`` and 
                 ``core::is_nothrow_swappable<value_type>``.

      If both |expected| are *valid*, then their values are swapped.
      If both |expected| are *invalid*, then their exception_ptrs are swapped.

      Otherwise, the *valid* and *invalid* state between both |expected| is
      swapped and the *valid* object is moved into the *invalid* object, and
      vice versa.

   .. function:: explicit operator bool () const noexcept

      :returns: Whether the |expected| is *valid* or *invalid*.

   .. function:: value_type const* operator -> () const noexcept
                 value_type* operator -> () noexcept

      Accessing the managed object when the |expected| is *invalid* will
      result in undefined behavior.

      .. versionadded:: 1.1

      :returns: pointer to the object managed by the |expected|

   .. function:: value_type const& operator * () const noexcept
                 value_type& operator * () noexcept

      :returns: The object managed by the |expected|. Accessing this object
                when the |expected| is *invalid* will result in undefined
                behavior.

   .. function:: value_type const& value () const
                 value_type& value ()

      :returns: The object managed by |expected|
      :throws: The exception managed by |expected| if the |expected|
      :noexcept: ``false``

   .. function:: value_type value_or (U&& value) const &
                 value_type value_or (U&& value) &&

      :returns: The object managed by |expected| if *valid*, otherwise, *value*
                is returned. This function will not compile if *U* is not
                convertible to :type:`expected\<T>::value_type`.

   .. function:: void emplace (std::initializer_list<U>, Args)
                 void emplace (Args)

      .. versionadded:: 1.1

      Constructs the object managed by |expected|. If the |expected| is
      already *valid*, it will first destruct the object it is currently
      managing.

   .. function:: E expect () const

      :noexcept: ``false``

      This function attempts to extract the given exception type *E*. If
      |expected| is *valid*, :class:`bad_expected_type` is thrown. If
      |expected| is *invalid*, but *E* is not the correct exception type,
      ``std::nested_exception`` with :class:`bad_expected_type` and the actual
      exception are thrown. Otherwise, the exception is returned by value.

   .. function:: void raise () const

      :noexcept: ``false``
      :attributes: ``[[noreturn]]``.

      Throws the |expected|'s managed exception if *invalid*. Otherwise, throws
      :class:`bad_expected_type`. This function *always* throws, and will never
      return.

   .. function:: std::exception_ptr pointer () const

      This function will throw if |expected| is *invalid*.

      .. versionadded:: 1.1 Replaces :func:`get_ptr`.

      :returns: The exception pointer managed by |expected|
      :throws: :class:`bad_expected_type`
      :noexcept: ``false``

   .. function:: std::exception_ptr get_ptr () const

      .. deprecated:: 1.1 Use :func:`pointer` as a replacement.

      :returns: The exception pointer managed by |expected|
      :throws: :class:`bad_expected_type`
      :noexcept: ``false``

Result Type
-----------

.. class:: result<T>

   .. versionadded:: 1.1

   |result| works much like |expected|. However, it does not manage an
   exception, but rather a ``std::error_condition``. This is done to provide a
   nice rounding out for functions which may want to signal an error, but not
   require the 'output' value to be passed by reference or by pointer.

   A |result| object is *valid* when one of the following occurs:

    * The object is initialized with a value of type *T*.
    * The object is constructed with a *valid* |result|.
    * The object is assigned a *valid* |result|.
    * The object is default initialized.
    * The object is constructed with a ``std::error_condition`` whose value
      is 0.
    * The object is assigned a ``std::error_condition`` whose value is 0.

   A |result| object is *invalid* when one of the following occurs:
    * The object is initialized with a non-zero ``std::error_condition``.
    * The object is assigned a non-zero ``std:error_condition``.

   .. type:: value_type

      Represents the given type *T*.

      .. warning:: A |result|'s :type:`value_type` *may not* be:

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

      Initializes the |result| to be *invalid*. The third overload may be
      passed any value for which ``std::is_error_condition_enum`` is *true*.

   .. function:: result (value_type const& value)
                 result (value_type&& value)

      Initializes |result| into a *valid* state with the given *value*. The
      move constructor is marked ``noexcept`` *only* if
      ``std::is_nothrow_move_constructible`` is *true* for :type:`value_type`.

   .. function:: result (in_place_t p, std::initializer<U> il, Args&& args)
                 result (in_place_t p, Args&& args)

      Initializes |result| into a *valid* state by constructing a
      :type:`value_type` in place with the given arguments. These constructors
      only participate if :type:`value_type` is constructible with the given
      arguments. *args* is a variadic template of arguments.

   .. function:: result (result const& that)
                 result (result&& that)

      Copies or moves the state stored in *that* into |result| as well as its
      managed value or error condition.

   .. function:: result ()

      Initializes |result| into a *valid* state by default constructing a
      :type:`value_type`.

   .. function:: result& operator = (result const& that)
                 result& operator = (result&& that)

      Assigns the contents and state of *that* to |result|. If the state of
      *that* and |result| differ, the condition or object managed by |result|
      will be destroyed and |result|'s state will then be constructed with
      the data stored in *that*.

   .. function:: result& operator = (std::error_condition const& condition)
                 result& operator = (ErrorConditionEnum e)

      Assigns the given *condition* or error condition enum value *e* to
      |result|. If |result| is in a *valid* state, its managed object will
      be destructed, and the incoming value assigned. If *condition* or *e*
      would result in a default constructed :type:`value_type`, (such as
      ``bool(condition) == false``), the managed object is still destructed
      and |result| will then be assigned a default constructed
      :type:`value_type`.

   .. function:: result& operator = (value_type const& value)
                 result& operator = (value_type&& value)
                 result& operator = (U&& value)

      Assigns *value* to |result|. If |result| is in a *valid* state, its
      managed object is also assigned *value*. If it is in an *invalid* state,
      it will then destroy the stored condition, and then place the |result|
      into a valid state.

      The third overload requires that :type:`value_type` be assignable and
      constructible from *U*.

   .. function:: void swap (result& that)

      Swaps the state of *that* with |result|. If both *that* and |result|
      are *valid*, then they swap their managed objects. If both *that* and
      |result| are *invalid*, they swap their managed conditions.
      If their states differ, the *invalid* instance is constructed with the
      contents of the *valid* instance via move construction. The *valid*
      instance is then invalidated with the previously *invalid* instance's
      condition.

      :noexcept: ``std::is_nothrow_move_constructible<value_type>`` and
                 ``core::is_nothrow_swappable<value_type>``

   .. function:: explicit operator bool () const noexcept

      :returns: Whether |result| is *valid* or *invalid*.


   .. function:: value_type const& operator * () const noexcept
                 value_type& operator * () noexcept

      Calling this function when |result| is *invalid* will result in
      undefined behavior.

      :returns: The object managed by |result|.

   .. function:: value_type const* operator -> () const noexcept
                 value_type* operator -> () noexcept

      Calling this function when |result| is *invalid* will result in
      undefined behavior.

      :returns: The address of the object managed by |result|

   .. function:: void emplace (std::initializer_list<T> il, Args&& args)
                 void emplace (Args&& args)

      Destroys whatever state is managed by |result| and then reinitializes
      it to be *valid* while constructing a :type:`value_type` with the given
      arguments. *args* is a variadic template argument.

   .. function:: value_type const& value () const
                 value_type& value ()

      If |result| is *invalid*, this function will throw a ``std::system_error``
      exception with the managed error condition.

      :returns: Object managed by |result|
      :throws: ``std::system_error``

   .. function:: value_type value_or (U&& value) const&
                 value_type value_or (U&& value) &&

      If |result| is in an *invalid* state, a :type:`value_type` converted
      from *value* is returned. Otherwise, the |result|'s managed object is
      copied or moved into the returning value, depending on whether |result|
      is an rvalue or const lvalue reference.

      :requires: :type:`value_type` be move or copy constructible
                 and that *U* is convertible to :type:`value_type`.

      :returns: :type:`value_type`

   .. function:: std::error_condition const& condition () const

      If |result| is *invalid*, the condition it manages is returned. Otherwise
      an exception is thrown.

      :returns: ``std::error_condition`` managed by |result|
      :noexcept: false
      :throws: :class:`bad_result_condition`

Functions
---------

.. function:: optional<T> make_optional<T>(T&& value)

   :raises: Any exceptions thrown by the constructor of T

   Creates an |optional| object from value. Effectively calls::

       optional<typename std::decay<T>::type>(std::forward<T>(value));

   Due to a bug in Apple Clang-503.0.40, this function is *not* marked
   constexpr, and this causes an incompatibility with N3793_.

.. function:: expected<T> make_expected (T&& value)
              expected<T> make_expected (E&& exception)
              expected<T> make_expected (std::exception_ptr)

   .. versionadded:: 1.1 The overload version which takes exception type *E*

   The first overload returns a *valid* |expected| containing a T constructed
   with *value*. The second overload returns an *invalid* |expected| with
   an exception_ptr to *exception*. For this version to be usable, *E* must
   inherit from ``std::exception``. The third overload takes an exception
   pointer and returns an *invalid* |expected| from it.

.. function:: result<T> make_result (T&& value)
              result<T> make_result (std::error_condition cnd)
              result<T> make_result (ErrorConditionEnum e)

   .. versionadded:: 1.1

   The first overload returns a *valid* |result| containing a T constructed
   with *value*. The second overload returns an *invalid* |expected| with an
   error_condition. The last overload takes any type for which
   ``std::is_error_condition_enum`` is *true*.


Operators
^^^^^^^^^

.. function:: bool operator == (optional const&, optional const&) noexcept
              bool operator == (optional const&, nullopt_t) noexcept
              bool operator == (nullopt_t, optional const&) noexcept
              bool operator == (optional<T> const&, T const&) noexcept
              bool operator == (T const&, optional<T> const&) noexcept

   For the first overload, if only one of the given |optional| values is
   *engaged*, it will return false. If both |optional| values are 
   *disengaged*, it will return true. Otherwise the |optional| values compare
   their managed objects with ``operator ==``

   The second overload returns whether or not the |optional| value is *engaged*.
   The third overload *always* returns false.
   The fourth and fifth overloads will check if the |optional| value is
   *engaged*. If it is, the object managed by |optional| will be compared
   with ``operator ==``. Otherwise it will return false.

.. function:: bool operator < (optional<T> const&, optional<T> const&) noexcept
              bool operator < (optional<T> const&, nullopt_t) noexcept
              bool operator < (nullopt_t, optional<T> const&) noexcept
              bool operator < (optional<T> const&, T const&) noexcept
              bool operator < (T const&, optional<T> const&) noexcept

   For the first overload, if the right |optional| is *disengaged*, it will
   return false. If the left |optional| is *disengaged*, it will return true.
   Otherwise, the result of ``*lhs < *rhs`` is returned.

   The second overload returns true if the |optional| is *disengaged*.
   The third overload returns true if the |optional| is *engaged*.
   The fourth optional returns true if the |optional| is *disengaged*.
   The fifth optional returns false if the |optional| is *disengaged*.
   Otherwise the result ``*opt < value`` or ``value < *opt`` is returned.

.. note:: The rest of the relational operators for |optional| are (mostly)
   implemented in terms of ``operator ==`` and ``operator <``.

.. function:: bool operator == (expected const&, expected const&) noexcept
              bool operator == (expected const&, exception_ptr) noexcept
              bool operator == (exception_ptr, expected const&) noexcept
              bool operator == (expected const&, T const&) noexcept
              bool operator == (T const&, expected const&) noexcept

   .. versionchanged:: 1.1 The comparison of an |expected| to an exception_ptr
      no longer compare the actual underlying exception_ptr if the |expected|
      is *invalid*. Comparing an |expected| to a ``std::exception_ptr`` now
      works as though one compared an |optional| to ``nullopt``.

   For the first overload if only one of the |expected| values is *valid*,
   it will return ``false``. If both |expected| values are *invalid*, it will
   return ``true`` Otherwise, the |expected| values compare their managed
   objects with ``operator ==``.

   The second and third overload return ``true`` if the |expected| value is
   *invalid*.

   The fourth and fifth overload returns ``true`` only if the |expected| value
   is *valid* and its managed object compares equal wth the *T* via *T*'s
   ``operator ==``.

.. function:: bool operator < (expected const&, expected const&) noexcept
              bool operator < (expected const&, exception_ptr) noexcept
              bool operator < (exception_ptr, expected const&) noexcept
              bool operator < (expected const&, T const&) noexcept
              bool operator < (T const&, expected const&) noexcept

   For the first overload, if the right |expected| is *invalid*, it will
   return ``false``. If the left |expected| is *invalid* it will return
   ``true``. If both |expected| objects are *valid*, then their managed values
   are compared via ``operator <``.

   The second overload returns ``true`` if the |expected| is *invalid*.
   The third overload returns ``true`` if the |expected| is *valid*.
   The fourth overload returns ``true`` if the |expected| is *invalid*.
   The fifth overload returns ``false`` if the |expected| is *invalid*.
   Otherwise the result of ``*exp < value`` or ``value < *exp`` is returned.

.. note:: The rest of the relational operators for |expected| are implemented
   in terms of ``operator ==`` and ``operator <``.

.. function:: bool operator == (result const&, result const&)
              bool operator == (result const&, error_condition const&)
              bool operator == (error_condition const&, result const&)
              bool operator == (result const&, error_code const&)
              bool operator == (error_code const&, result const&)
              bool operator == (result const&, T const&)
              bool operator == (T const&, result const&)

   .. versionadded:: 1.1

   For the first overload if only one of the |result| objects is *valid*,
   it will return ``false``. If both |result| objects are *invalid*, the
   result of comparing their ``error_condition`` is returned. Otherwise, the
   |result| values compare via ``operator ==``.

   The second, third, fourth, and fifth overload will return ``false`` if
   the |result| object is *valid* (even if the ``std::error_condition`` or
   ``std::error_code`` were to return ``false`` in a boolean context. This was
   done to minimize issues with differing categories). If the |result| is
   *invalid*, its :func:`result\<T>::condition` is compared against the
   ``std::error_condition`` or ``std::error_code`` via ``operator ==``.

   The sixth and seventh overloads will return ``false`` if |result| is
   *invalid*. Otherwise the |result| value is compared with the given *T*
   via ``operator ==``.

.. function:: bool operator < (result const&, result const&)
              bool operator < (result const&, error_condition const&)
              bool operator < (error_condition const&, result const&)
              bool operator < (result const&, T const&)
              bool operator < (T const&, result const&)

   .. versionadded:: 1.1

   For the first overload, if both |result| objects are *invalid*, the
   ``operator <`` comparison of their :func:`result\<T>::condition` are
   returned. If both |result| objects are *valid*, the comparison of their
   values via ``operator <`` is returned. If the |result| on the left is
   *invalid*, but the |result| on the right is not, ``true`` is returned.
   Otherwise ``false``.

   The second overload returns ``false`` if the |result| is *valid* (even
   if the ``std::error_condition`` would evaluate to ``false`` in a boolean
   context. This was done to minimize issues with differing categories). If
   the |result| is *invalid*, its :func:`result\<T>::condition` is compared
   against the ``std::error_condition`` via ``operator <``

   Conversely, the third overload returns ``true`` if the |result| is *valid*.
   If the |result| is *invalid*, its :func:`result\<T>::condition` is compared
   against the ``std::error_condition`` via ``operator <``.

   For the fourth overload, if the |result| is *invalid*, ``false`` is returned.
   Otherwise, the comparison of the |result| value and *T* via ``operator <``
   is returned.

   For the fifth overload, if the |result| is *invalid*, ``true`` is returned.
   Otherwise, the comparison of the |result| value and *T* via ``operator <``
   is returned.

.. note:: The rest of the relational operators for |result| are implemented
   in terms of ``operator ==`` and ``operator <``.

Swap
^^^^

.. function:: void swap (optional&, optional&)
              void swap (expected&, expected&)
              void swap (result&, result&)

   These swap functions are provided to allow for ADL calls to swap.

Specializations
---------------

.. class:: expected<void>

   |expected-v| is provided as a way to have the same semantics as |expected|,
   but for functions that do not (or cannot) return a value. Its interface
   is close to that of |expected|, however as it cannot store a value, it is
   smaller and only has member functions related to handling the exception
   stored within the |expected-v|.

   .. type:: value_type

      Always ``void``.

   .. function:: explicit expected (std::exception_ptr) noexcept

      Initializes (and invalidates) the |expected-v|.

   .. function:: expected (expected const&) = default
                 expected (expected&&) = default

      Copies the exception_ptr stored within the |expected-v|. Invalidates
      ``*this``.

   .. function:: expected& operator = (expected const&) = default
                 expected& operator = (expected&&) = default

      Copies the exception_ptr stored within the |expected-v|. Invalidates
      ``*this``.

   .. function:: void swap (expected&) noexcept

      Swaps the |expected-v|'s exception_ptrs.

   .. function:: explicit operator bool () const noexcept

      :returns: Whether the |expected-v| is *valid* or *invalid*.

   .. function:: E expect<E> () const

      See :func:`expected\<T>::expect\<E>`

   .. function:: void raise () const

      See :func:`expected\<T>::raise`

   .. function:: std::exception_ptr pointer () const

      Throws if |expected-v| is *valid*.

      :returns: The managed exception_ptr if the |expected-v| is *invalid*.
      :throws: :class:`bad_expected_type` if the |expected-v| is *valid*.
      :noexcept: ``false``.

   .. function:: std::exception_ptr get_ptr () const

      .. deprecated:: 1.1 Use :func:`pointer` as a replacement.

      :returns: The managed exception_ptr if the |expected-v| is *invalid*.
      :throws: :class:`bad_expected_type` if the |expected-v| is *valid*.
      :noexcept: ``false``.

.. class:: result<void>

   |result-v| is provided as a way to have the same semantics as |result|,
   but for functions that do not (or cannot) return a value. Its interface
   is close to that of |result|, however as it cannot store an object, it is
   smaller and only has member functions related to handling error conditions.

   Technically speaking, this type is unnecessary as an error_condition can be
   supplied instead. However, it's sometimes nice to allow for more generic
   code to be written, and worrying about whether or not you might accidentally
   instantiate a |result-v| isn't something one should have to worry about.

   The copy, move, and default constructors as well as the copy and move
   assignment operators for |result-v| are defaulted, letting the stored
   ``std::error_condition`` be managed instead.

   .. type:: value_type

      Always ``void``.

   .. function:: result (int value, std::error_category const& category)
                 result (std::error_condition cnd)
                 result (ErrorConditionEnum e)

      Constructs the |result-v| with the given value. Any type for which
      ``std::is_error_condition_enum`` is *true* may be used as an argument in
      the third overload.

   .. function:: result& operator = (std::error_condition const& condition)
                 result& operator = (ErrorConditionEnum e) noexcept

      Assigns |result-v| with the given value. Any value for which
      ``std::is_error_condition_enum`` is *true* may be used in the second
      overload.

   .. function:: explicit operator bool () const noexcept

      :returns: Whether the |result-v| is *valid* or *invalid*.

   .. function:: void swap (result& that)

      Swaps the stored ``std::error_condition`` with *that*.

   .. function:: std::error_condition const& condition () const

      If the |result-v| is *valid*, it will throw an exception.
      Otherwise, the ``std::error_condition`` stored will be returned.

      :noexcept: false
      :throws: :class:`bad_result_condition`
      :returns: The stored ``std::error_condition`` if |result-v| is *invalid*.

std::hash
^^^^^^^^^

.. class:: hash<optional<T>>

   Specialization of ``std::hash``.

   Requires that the :type:`optional\<T>::value_type` be specialized for
   ``std::hash``. If the |optional| is engaged it will return the hash
   value for ``hash<value_type>``. Otherwise, it will return a default
   constructed ``std::hash<value_type>::result_type``.

.. class:: hash<expected<T>>

   Specialization of ``std::hash``.

   Requests that the :type:`expected\<T>::value_type` be specialized for
   ``std::hash``. If the |expected| is *valid*, it will return the hash value
   for ``hash<value_type>``. Otherwise, it will return a default constructed
   ``std::hash<value_type>::result_type``.

.. class:: hash<result<T>>

   Specialization of ``std::hash``.

   Requests that the :type:`result\<T>::value_type` be specialized for
   ``std::hash``. If the |result| is *valid*, it will return the hash value for
   ``hash<value_type>``. Otherwise, it will return a default constructed
   ``std::hash<value_type>::result_type``.

.. _N3793: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3793.html
.. _N4015: https://isocpp.org/files/papers/n4015.pdf
