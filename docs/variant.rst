Variant Component
=================

.. default-domain:: cpp

The :class:`variant\<Ts>` component is a generic type-safe implementation of a
discriminate union. It is equivalent to the Boost.Variant library, with several
small exceptions.

Specifically, the :class:`variant\<Ts>` does not allow for recursive variants,
and makes no attempt to support them in any capacity. It is the author's
opinion that a :class:`variant\<Ts>` should be stack allocated only, and not
have to handle or deal with allocating memory on the heap.

Additionally, the :class:`variant\<Ts>` has the ability to perform type-based
pattern matching via lambdas. This allows a user to define the different code
paths that are desired without requiring a user to implement a visitor functor
separately. *This means that a variant is capable of performing a type-based
switch statement*



.. namespace:: core
