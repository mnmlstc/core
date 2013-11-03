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
