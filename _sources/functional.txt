.. _core-functional-component:

Functional Component
====================

.. default-domain:: cpp

The functional component contains several utilities to assist with functions.
Among these are the implementation of the *INVOKE* pseudo-expression in code
form, as well as an extension of this pseudo-expression with *unpack* semantics
at both compile time and runtime.

Additionally, the functional component provides a :class:`function_traits\<T>`
object to discern at compile time the return type and arity of a function. The
ability to discover the types of a given function's argument is also possible.

The functional component can be found in the ``<core/functional.hpp>`` header.

.. namespace:: core

.. class:: unpack_t

   A sentinel class to represent that one wishes to call :func:`invoke` with
   unpack semantics. An instance is available under the name ``unpack``.

.. class:: runpack_t

   A sentinel class to represent that one wishes to call :func:`invoke` with
   runtime unpack semantics. An instance is available under the name
   ``runpack``.

.. class:: function_traits<T>

   Function traits to discern various amounts of details for a given callable
   type *T*. The traits allow one to get the :member:`arity` of a callable,
   as well as explore the types of various argument indices.

   .. type:: return_type

      The return type of a given function *T*.

   .. type:: pointer

      A type alias that represents the callable as a function pointer.

   .. member:: static constexpr arity

      :type: ``std::size_t``

      Represents the number of arguments the callable *T* takes.

   .. type:: argument<N>

      Given a ``std::size_t`` *N*, argument will be a type alias for the type
      located at the index in its parameter list.

.. function:: auto invoke (...)

   There are 5 overloads available for this function. They are the
   implementation of the *INVOKE* pseudo-expression discussed in the C++11
   standard. These overloads *are not* constexpr however due to C++11 not
   marking ``std::forward`` as constexpr.

.. function:: auto invoke (unpack_t, Functor&&, Unpackable&&)
              auto invoke (unpack_t, Unpackable&&)

   :requires: *Unpackable* be capable of having ``std::get<N>`` called on it,
              and an overload for ``std::tuple_size``.

   This version of :func:`invoke` uses compile time unpacking semantics. It
   will take every member of *Unpackable* and call ``std::get<N>``.
   ``std::tuple_size`` is used to get the number of elements in *Unpackable*.

.. function:: auto invoke (runpack_t, Functor&&, Runpackable&&)

   :requires: *Runpackable* be have a member function named *at*, which returns
              ``Runpackable::value_type``, and takes a ``std::size_t`` as its
              parameters.
   :throws: ``std::out_of_range``
   :noexcept: false

   This version of :func:`invoke` uses runtime unpacking semantics. It will
   take the arity of *Functor*, and then unpack *Runpackable* via its ``at``
   member function. As an example a *Functor* with 4 arguments, and a
   *Runpackable* of type ``std::vector<int>`` would expand to::

      core::invoke(
        std::forward<Functor>(functor),
        std::forward<Runpackable>(runpackable).at(N)...
      );

   As the standard containers all throw ``std::out_of_range``, this function
   should be expected to as well.

   .. note:: *ALL* standard containers with an ``at`` member function can be
      used with this, including the associative containers, such as
      ``std::map``, and ``std::unordered_map``. *However*, the requirement that
      the ``at`` member function take a ``std::size_t`` remains.
