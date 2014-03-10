.. _core-iterator-component:

Iterator Component
==================

.. default-domain:: cpp

The iterator component provides the additional functions provided in C++14 with
respect to iterators. Specifically the free form functions that work like
``std::begin`` and ``std::end``.

.. namespace:: core

.. function:: cbegin (Container const& container)


   :returns: ``std::begin(container)``

.. function:: cend (Container const& container)

   :returns: ``std::end(container)``

.. function:: rbegin (Container const& container)
              rbegin (Container& container)

   Requires that the given *container* have a member function named ``rbegin``.
   If no such member function exists, the function will fail to compile.

   :returns: ``container.rbegin()``

.. function:: crbegin (Container const& container)

   :returns: ``rbegin(container)``

.. function:: rend (Container const& container)
              rend (Container& container)

   Requires that the given *container* have a member function named ``rend``.
   If no such member function exists, the function will fail to compile.

   :returns: ``container.rend()``

.. function:: crend (Container const& container)

   :returns: ``rend(container)``

.. class:: infix_ostream_iterator<T>

   The :class:`infix_ostream_iterator\<T>` is used as a replacement for
   ``std::ostream_iterator``, where the delimiter is not desired for the last
   element. This iterator comes in use when one might wish to print a list.
