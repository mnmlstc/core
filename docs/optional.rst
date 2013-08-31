Optional Component
==================

.. default-domain:: cpp

The ``optional<T>`` component is currently available in Boost. However,
this implementation of ``optional<T>`` follows the C++14 implementation as
closely as possible.

.. note:: Due to the relaxation of rules regarding ``constexpr`` in C++14,
          :class:`optional\<T>` is not able to implement the entire
          interface. For those parts of the interface that cannot be used,
          a note much like this one will be in the function description.

.. namespace:: core

.. class:: in_place_t

   :class:`in_place_t` is an empty class type used to disambiguate
   the overloads and member functions of :class:`optional\<T>` that
   take arguments (such as :func:`optional\<T>::emplace`) for
   in-place construction of some value.

   An object of this type is available under the name ``in_place``.

.. class:: nullopt_t

   nullopt_t is an empty class type used to indicate an
   :class:`optional\<T>` type with uninitialized state.

   An object of this type is available under the name ``nullopt``.

.. class:: bad_optional_access

   :inherits: std::logic_error

   Thrown when accessing an :class:`optional\<T>` that is in a
   disengaged state.

   .. note:: Accessing an :class:`optional\<T>` in a disengaged
             state does not refer to accessing the underlying object.
             Specifically it means calling :class:`optional\<T>`
             member functions, and not member functions of the
             underlying object it may or may not contain.

.. class:: optional<T>

   The :class:`optional\<T>` manages an *optional* value. This value may be
   in either an initialized state, or an uninitialized state. This value is
   guaranteed to be allocated within the :class:`optional\<T>`. Instead of
   modelling a pointer, such as ``std::unique_ptr<T>`` or
   ``std::shared_ptr<T>``, :class:`optional\<T>` models an object, even though
   :func:`optional\<T>::operator->` and :func:`optional\<T>::operator*`
   are provided.

   An :class:`optional\<T>` object is *engaged* when one of the following
   occurs:

    * The object is initialized with a value of type T
    * The object is assigned an *engaged* :class:`optional\<T>`.

   An :class:`optional\<T>` object is *disengaged* when one of the following
   occurs:

    * The object is default-initialized.
    * The object is initialized with an instance of :class:`nullopt_t`  or a
      *disengaged* :class:`optional\<T>`.
    * The object is assigned with an instance of :class:`nullopt_t` or a
      *disengaged* :class:`optional\<T>`.


   .. type:: value_type

      Represents the underlying type stored within an :class:`optional\<T>`.

   .. function:: operator bool () const

      :returns: true if the object is *engaged*, false otherwise.

   .. function value_type value_or<U>(U&& value)

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

   .. function:: void emplace (Args&&)

.. function:: optional<T> make_optional<T>(T&& value)

   :raises: Any exceptions thrown by the constructor of T

   Creates an :class:`optional\<T>` object from value. Effectively calls::

       optional<typename std::decay<T>::type>(std::forward<T>(value));


