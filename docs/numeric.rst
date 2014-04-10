.. _core-numeric-component:

Numeric Component
=================

.. default-domain:: cpp
.. highlight:: cpp

The numeric component is a wrapper around the C++ stdlib ``<numeric>`` header.
The functions contained within this header take a range of values. However,
as is common in the C++ standard library, it requires the user to provide a
begin and end to the range of elements. Because MNMLSTC Core provides a
:class:`range <range\<T>>` type, there is no logical reason a version of these
functions that take a range are not provided.

.. function:: iota (Range&& range, T&& value)

   :requires: *range* must provide ForwardIterators

   
