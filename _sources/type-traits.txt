.. _core-type-traits-component:

Type Traits Component
=====================

.. default-domain:: cpp

.. |is_nothrow_swappable| replace::
   :type:`is_nothrow_swappable <is_nothrow_swappable\<T>>`

.. |is_swappable| replace:: :type:`is_swappable <is_swappable\<T>>`

The type traits component contains many utilities that are available in C++14,
as well as a few custom types. Such as, |is_nothrow_swappable| and
|is_swappable|. *Most* of the types provided within this component are
in fact simple type aliases defined for C++14. e.g.,
``typename std::decay<T>::type`` is aliased to ``core::decay_t<T>``. All types
that are used as type *modifiers* (rather than type *properties*) have the
equivalent alias available.

Only type traits not provided by the C++11 standard are documented. If a
type trait provided by MNMLSTC Core differs from the standard in any way, it is
also documented here.

The type traits component can be found in the ``<core/type_traits.hpp>`` header.

.. type:: is_null_pointer<T>

   An alias for ``std::true_type`` if *T* is any form of the type
   ``std::nullptr_t``. This includes its *const* and *volatile* qualified
   counterparts.

.. type:: is_unpackable<T>

   Given a type *T*, checks if the type may be used with :class:`unpack_t` and
   :func:`invoke`.

.. type:: is_runpackable<T>

   Given a type *T*, checks if the type may be used with :class:`runpack_t` and
   :func:`invoke`.

.. type:: class_of<T>

   Given a member function pointer type *T*, it extracts the underlying
   class type. For example, a type ``void A::*()`` will given the type *A* as
   its ``type`` member.

.. type:: invokable<Args>

   Given a typelist *Args*, checks if the first type in *Args* may be invoked
   with the rest of the arguments in *Args*. The rule for deciding if *Args*
   follows the *INVOKE* pseudo expression.

.. type:: invoke_of<Args>

   Given a typelist *Args*, the member typedef ``type`` will represent the
   return type if *Args* is invoked according to the *INVOKE* pseudo-expression.

.. type:: result_of<T>

   This is an SFINAE capable version of result_of. It relies on
   :type:`invoke_of\<Args>` to work correctly.

.. type:: common_type<Ts>

   A more compiler agnostic version of ``std::common_type<Ts>``. This was
   implemented to workaround an issue with Clang's ``std::common_type``
   attempting to discover the common type of two ``void&&``.

.. type:: is_swappable<T>

   Unfortunately, this type is not as strong as one would like to believe. It
   always returns true for a given type, even if an expression is malformed.
   (For instance, given a type *T const*, it will return true). The only way
   to fix this would be to place an ``enable_if`` on std::swap. And that's
   not happening.

.. type:: is_nothrow_swappable<T>

   Unlike |is_swappable|, |is_nothrow_swappable| is actually quite strong.
   It will be ``std::true_type`` if a given swap call on a type is actually
   marked as *noexcept*, and ``std::false_type`` otherwise. This type
   is comparable to libc++'s internal ``__is_nothrow_swappable``.

.. type:: all_traits<Args>

   Given a typelist *Args*, where each type in *Args* is a type trait property
   that contains a boolean member value, it will ether be equivalent to
   ``std::true_type`` if *all* of the traits ``value`` members are true.
   Otherwise, it will be equivalent to ``std::false_type``

.. type:: any_traits<Args>

   Works much like :type:`all_traits\<Args>`, however only *one* of the given
   traits in *Args* must be true.

.. type:: no_traits<Args>

   The inverse of :type:`all_traits\<Args>`. Will only be true if every
   trait in *Args* is false.
