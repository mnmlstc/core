Propagating Const
=================

.. namespace:: core

The :any:`propagate_const` component was added in the C++ Library Fundamentals
V2 specification. It is a const-propagating wrapper for pointers and pointer
like types. This type solves the issue where a ``std::unique_ptr<T>`` would
return a ``T*``, *even* if the ``std::unique_ptr<T>`` were ``const`` in a
given context.

This component resides in :file:`<core/{propagate_const}.hpp>`

.. class:: template <class T> propagate_const
