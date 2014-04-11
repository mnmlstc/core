.. _core-algorithm-component:

Algorithm Component
===================

.. default-domain:: cpp
.. highlight:: cpp

The algorithm component can be seen as a competitor to the Boost.Range
algorithm headers. There are a few small differences (namely that this
component relies on the :ref:`core-range-component`), however these differences
are discussed below.

Non-Modifying Sequence Operations
---------------------------------



.. function:: bool all_of (Range&& range, UnaryPredicate&& up)

   :returns: ``true`` if *up* returns ``true`` for **all** elements in *range*.
   :requires: *range* must provide InputIterators.

.. function:: bool any_of (Range&& range, UnaryPredicate&& up)

   :returns: ``true`` if *up* returns ``true`` for **any** elements in *range*.
   :requires: *range* must provide InputIterators.

.. function:: bool none_of (Range&& range, UnaryPredicate&& up)

   :returns: ``true`` if *up* returns ``false`` for **all** elements in *range*.
   :requires: *range* must provide InputIterators.
