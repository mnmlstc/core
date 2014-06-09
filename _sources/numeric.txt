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

The numeric component resides in the ``<core/numeric.hpp>`` header.

.. note:: Several functions in this component take a range as its input, and
   an output iterator for output. These functions *do not* change the output
   to also be a range. An iterator *must* be passed to them (This reduces the
   number of overloads needed while also allowing the continued use of
   ``back_insert_iterator`` and friends.

.. function:: void iota (Range&& range, T&& value)

   :requires: *range* must provide ForwardIterators

   Fills *range* with sequentially increasing values, starting with *value*.
   While *range* is treated as a universal reference, its lifetime should
   extend past that of the call to :func:`iota`, in the event that *range*
   is an rvalue reference. The *value* is simply perfectly forwarded to the
   underlying ``std::iota`` call.

.. function:: decay_t<T> accumulate (Range&& range, T&& init)
              decay_t<T> accumulate (Range&& range, T&&, BinaryOp&& op)

   Computes the sum of the given value *init* and the elements in *range*. The
   first version uses ``operator +``, while the second version uses *op*.
   Both *init* and *op* are perfectly forwarded. The lifetime of *range* may
   end with the call to this function.

   :requires: *range* must provide InputIterators.

.. function:: decay_t<T> inner_product (Range&& range, InputIt&& it, T&& value)
              decay_t<T> inner_product (\
                Range&& range,\
                InputIt&& it,\
                T&& value,\
                BinaryOp&& op,\
                BinaryOp2&& op2\
              )

   Computes the inner product (that is, the sum of products of the element in
   *range*. The first version uses ``operator *`` to compute the product of
   element pairs and ``operator +`` to sum the products. The second version
   uses *op* and *op2* for these tasks respectively.

   :requires: *range* must provide InputIterators

.. function:: decay_t<OutputIt> adjacent_difference (\
                Range&& range,\
                OutputIt&& it\
              )
              decay_t<OutputIt> adjacent_difference (\
                Range&& range,\
                OutputIt&& it,\
                BinaryOp&& op\
              )

   Computes the differences between the second and the first of each adjacent
   pair of elements in *range*, and writes them to the range beginning at
   *it* + 1. The first version uses ``operator -``, while the second uses
   *op*. These parameters are perfectly forwarded to the underlying call to
   ``std::adjacent_difference``.

   :requires: *range* must provide InputIterators.

.. function:: decay_t<OutputIt> partial_sum (Range&& range, OutputIt&& it)
              decay_t<OutputIt> partial_sum (\
                Range&& range,\
                OutputIt&& it,\
                BinaryOp&& op\
              )

    Computes the partial sum of the slemenets in the subranges of *range*.
    It then writes these values to the range beginning at *it*. The first
    version uses ``operator +``, while the second uses the binary operation
    *op*.

   :requires: *range* must provide InputIterators.
