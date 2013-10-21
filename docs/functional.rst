Functional Component
====================

.. default-domain:: cpp

The functional component contains several utilities to assist with functions.
Among these are the implementation of the *INVOKE* pseudo-expression in code
form, as well as an extension of this pseudo-expression with *unpack* semantics
at both compile time and runtime.

Additionally, the functional component provides a :class:`function_traits`
object to discern at compile time the return type and arity of a function. The
ability to discover the types of a given function's argument is also possible.

.. namespace:: core

.. class:: unpack_t

.. class:: runpack_t

.. function:: auto invoke (unpack_t, Functor&&, Unpackable&&)
              auto invoke (unpack_t, Unpackable&&)

.. function:: auto invoke (runpack_t, Functor&&, Runpackable&&)
