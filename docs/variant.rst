.. _core-variant-component:

Variant Component
=================

.. default-domain:: cpp

.. |variant| replace:: :class:`variant <variant\<Ts>>`

The |variant| component is a generic type-safe implementation of a
discriminate union. It is equivalent to the Boost.Variant library, with several
small exceptions.

Specifically, the |variant| does not allow for recursive variants,
and makes no attempt to support them in any capacity. It is the author's
opinion that a |variant| should be stack allocated only, and not
have to handle or deal with allocating memory.

Additionally, the |variant| has the ability to perform type-based
pattern matching via lambdas. This allows a user to define the different code
paths that are desired without requiring a user to implement a visitor functor
separately. *This means that a variant is capable of performing a type-based
switch statement*

.. namespace:: core

.. class:: bad_variant_get

   :inherits: ``std::logic_error``

   Used when the call to get the value out of a |variant| uses the incorrect
   index of the type list.

.. class:: variant<Ts>

   The |variant| type represents a type-safe discriminate union. Much like
   the Boost.Variant, a |variant| is never in an uninitialized state. When
   default constructing a |variant|, the first type in the |variant|'s typelist
   is initialized.



   .. function:: visit (Visitor&&, args) const
                 visit (Visitor&&, args)

     Visiting a |variant| follows the following semantics. These semantics
     require that, when given a callable type ``Visitor``, and variadic
     arguments ``Args...``, that the return type of the visit will be
     a  result of ``common_type_t<invoke_of_t<Visitor, Ts, Args...>...>``.

     If a common type cannot be found, then the visitation function will
     fail to compile properly. This means that a visitor *must* be capable of
     being invoked with all types in the |variant|'s typelist and the arguments
     given. The visitor is executed with the *INVOKE* psuedo expression.

     These same semantics are required for :func:`match`.

     :returns: ``common_type_t<invoke_of_t<Visitor, Ts, Args...>...>``

   .. function:: match (Visitors&&) const
                 match (Visitors&&)

      Takes a variadic number of arguments that are all callable objects.
