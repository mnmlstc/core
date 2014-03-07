.. _core-utility-component:

Utility Component
=================

.. default-domain:: cpp

The utility component provides additional functions and meta templates that
cannot be categorized. Most of the utilities provided are for working with
type lists.

.. namespace:: core

.. class:: integer_sequence<T, Is>

   meta template to generate a series of *T* to allow easier unpacking.

.. class:: index_sequence<I>

   A shortcut for :class:`integer_sequence\<T, Is>`.

.. class:: make_index_sequence<N>

   Used to generate the :class:`index_sequence\<I>`, with *N* numbers.

.. class:: typelist_index<T, Ts>

   :inherits: ``std::integral_constant<std::size_t, N>``

   The type of std::integral_constant that :class:`typelist_index\<T, Ts>` will
   inherit from depends on where *T* exists in the type list *Ts*. No attempts
   are made to allow multiple *T*s in *Ts*.

.. class:: type_at<N, Ts>

   .. type:: type

      When accessed, provides the type at the index *N* in the type list *Ts*

.. function:: auto value_at<N, Ts>(Ts&&)

   :returns: The value located at the index *N* in the type list *Ts*.

.. class:: scope_guard<Callable>

   Scope guard acts much like the Boost.ScopeExit library. It is non-copyable,
   but movable. It can be constructed with any callable type *Callable*. The
   type *Callable* must be move assignable and move constructible, and marked
   as *noexcept*. *Callable* must have a function arity of 0 (that is, it takes
   no arguments). It does not matter whether or not *Callable* returns a value,
   as it will be ignored. To easily construct a scope_guard, a function named
   :func:`make_scope_guard` is provided for type inference. This can easily
   be used to give a scope_guard a lambda. As an example::

      int x = new int { 4 }
      auto scope_guard = core::make_scope_guard([x] { delete x; });

.. function:: make_scope_guard(Callable callable) noexcept

   Constructs a :class:`scope_guard\<Callable>` from the given *callable*.
