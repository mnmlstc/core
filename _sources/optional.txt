.. _core-optional-component:

Optional Component
==================

.. default-domain:: cpp

.. |optional| replace:: :class:`optional <optional\<T>>`

The |optional| component is currently available in Boost. However,
this implementation of |optional| follows the C++14 proposal as closely as
possible.

.. note:: Due to the relaxation of rules regarding ``constexpr`` in C++14,
   |optional| is not able to implement the entire interface. For those parts
   of the interface that cannot be used, a note much like this one will be in
   the function description.

   Additionally, the current proposal states that one cannot use
   ``std::aligned_storage`` to hold the data contained within (this is a result
   of the number of constexpr constructors). This optional implementation
   ignores this, and uses ``std::aligned_storage``.

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

.. class:: optional<T>

   The |optional| manages an *optional* value. This value may be in either an
   initialized state, or an uninitialized state. This value is guaranteed to be
   allocated within the |optional|. Instead of modelling a pointer, such as
   ``std::unique_ptr<T>`` or ``std::shared_ptr<T>``, |optional| models an
   object, even though :func:`optional\<T>::operator->` and
   :func:`optional\<T>::operator*` are provided.

   An |optional| object is *engaged* when one of the following occurs:

    * The object is initialized with a value of type T
    * The object is assigned an *engaged* |optional|.

   An |optional| object is *disengaged* when one of the following occurs:

    * The object is default-initialized.
    * The object is initialized with an instance of :class:`nullopt_t`  or a
      *disengaged* |optional|.
    * The object is assigned with an instance of :class:`nullopt_t` or a
      *disengaged* |optional|.


   .. warning:: An |optional|'s :type:`value_type` *may not* be:

      * :class:`in_place_t`
      * :class:`nullopt_t`
      * ``std::nullptr_t``
      * ``void``
      * ``bool``

      The decision to disallow an ``optional<bool>`` is due to the explicit
      boolean conversion operator. It was decided that getting the true or
      false state would be error prone and too easy to miss.

   .. type:: value_type

      Represents the underlying type stored within an |optional|.

   .. function:: optional (optional const&)

      Copies the contents of the incoming |optional|. If the incoming
      |optional| is engaged, the contents of it are initialized into the
      new |optional| object.

   .. function:: optional (optional&& that)

      Constructs a new |optional| by moving the state of the incoming
      |optional|. If the invoming |optional| is engaged, its contents will be
      moved into the new object.

      :postcondition: ``that.empty() == true``
      :noexcept: ``std::is_nothrow_move_constructible<value_type>``

   .. function:: constexpr optional (nullopt_tr) noexcept
                 constexpr optional () noexcept

      Constructs a new |optional| into a disengaged state.

   .. function:: optional (value_type const&)
                 optional (value_type&&)

      Constructs a new |optional| into an *engaged* state with the contents of
      the value_type.

      :noexcept: ``std::is_nothrow_move_constructible<value_type>``

   .. function:: explicit optional (in_place_t, std::initializer_list<T>, args)
                 explicit optional (in_place_t, args)

      Constructs a new |optional| into an *engaged* state by constructing a
      :type:`value_type` in place.

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

   .. function:: value_type const* operator -> () const
                 value_type* operator -> ()

      Accessing the managed object when the |optional| is in a disengaged state
      will result in undefined behavior.

      :returns: pointer to the object managed by the |optional|

   .. function:: value_type const& operator * () const
                 value_type& operator * ()

      If the |optional| does not manage an object, dereferencing the 
      |optional| will result in undefined behavior.

      :returns: An lvalue reference to the object managed by the |optional|

   .. function:: operator bool () const

      This conversion operator is *explicit*.

      :returns: true if the object is *engaged*, false otherwise.

   .. function value_type value_or<U>(U&& value)

      If ``*this`` is an lvalue reference the :type`value_type` will be copy
      constructed. If ``*this`` is an rvalue reference, the :type:`value_type`
      is move constructed.

      :returns: the object managed by |optional| or a :type:`value_type`
                constructed from *value*.

   .. function:: value_type const& value () const
                 value_type& value ()

      .. note:: The ``value_type const&`` overload is marked ``constexpr`` in
                the C++14 interface.

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

.. function:: optional<T> make_optional<T>(T&& value)

   :raises: Any exceptions thrown by the constructor of T

   Creates an |optional| object from value. Effectively calls::

       optional<typename std::decay<T>::type>(std::forward<T>(value));

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

   For the first overload, if the right |optional| is *disengaged*, it will
   return false. If the left |optional| is *disengaged*, it will return true.
   Otherwise, the result of a comparison ``std::less<T>`` is returned.

   The second overload returns true if the |optional| is *disengaged*.
   The third overload returns true if the |optional| is *engaged*.
   The fourth optional returns true if the |optional| is *disengaged*.
   Otherwise, the result of a comparison with ``std::less<T>`` is returned.

Specializations
---------------

.. namespace:: std

Several specializations for the standard namespace are overloaded for the
|optional| component.

.. function:: void swap (optional<T>& lhs, optional<T>& rhs)

   Calls :func:`optional\<T>::swap`.

   :noexcept: ``lhs.swap(rhs)``

.. class:: hash<optional<T>>

   Requires that the :type:`optional\<T>::value_type` be specialized for
   ``std::hash``. If the |optional| is engaged it will return the hash
   value for ``hash<value_type>``. Otherwise, it will return a default
   constructed ``std::hash<value_type>::result_type``.
