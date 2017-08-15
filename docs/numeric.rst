Numeric Component
=================

.. namespace:: core

.. index:: numeric

The numeric component is a wrapper around the C++ stdlib :file:`<{numeric}>`
header. The functions contained within this header take a range of values.
However, as is common in the C++ standard library, it requires the user to
provide a begin and end to the range of elements. Because MNMLSTC Core provides
a :any:`range` type, there is no logical reason a version of these functions
that take a range are not provided.

The numeric component resides in the :file:`<core/{numeric}.hpp>` header.

.. note:: Several functions in this component take a range as its input, and
   an output iterator for output. These functions *do not* change the output
   to also be a range. An iterator *must* be passed to them (This reduces the
   number of overloads needed while also allowing the continued use of
   :cxx:`std::back_insert_iterator` and friends.

.. index:: numeric; functions

.. function:: void iota (Range&& range, T&& value)

   Fills :samp:`{range}` with sequentially increasing values, starting with
   :samp:`{value}`. While :samp:`{range}` is treated as a forwarding reference,
   its lifetime should extend past that of the call to :any:`iota`, in the
   event that :samp:`{range}` is an rvalue reference. The :samp:`{value}` is
   simply perfectly forwarded to the underlying :cxx:`std::iota` call.

   :requires: :samp:`{range}` must provide ForwardIterators

.. function:: decay_t<T> accumulate (Range&& range, T&& init)
              decay_t<T> accumulate (Range&& range, T&&, BinaryOp&& op)

   Computes the sum of the given value :samp:`{init}` and the elements in
   :samp:`{range}`. The first version uses :cxx:`operator +`, while the second
   version uses :samp:`{op}`. Both :samp:`{init}` and :samp:`{op}` are
   perfectly forwarded. The lifetime of :samp:`{range}` may end with the call
   to this function.

   :requires: :samp:`{range}` must provide InputIterators.

.. function:: decay_t<T> inner_product (Range&& range, InputIt&& it, T&& value)
              decay_t<T> inner_product (\
                Range&& range,          \
                InputIt&& it,           \
                T&& value,              \
                BinaryOp&& op,          \
                BinaryOp2&& op2)

   Computes the inner product (that is, the sum of products of the elements in
   :samp:`{range}`). The first version uses ``operator *`` to compute the
   product of element pairs and :cxx:`operator +` to sum the products. The
   second version uses :samp:`{op}` and :samp:`{op2}` for these tasks
   respectively.

   :requires: :samp:`{range}` must provide InputIterators

.. function:: decay_t<OutputIt> adjacent_difference (\
                Range&& range,                       \
                OutputIt&& it)
              decay_t<OutputIt> adjacent_difference (\
                Range&& range,                       \
                OutputIt&& it,                       \
                BinaryOp&& op)

   Computes the differences between the second and the first of each adjacent
   pair of elements in :samp:`{range}`, and writes them to the range beginning
   at :samp:`{it} + 1`. The first version uses ``operator -``, while the second
   uses :samp:`{op}`. These parameters are perfectly forwarded to the
   underlying call to ``std::adjacent_difference``.

   :requires: :samp:`{range}` must provide InputIterators.

.. function:: decay_t<OutputIt> partial_sum (Range&& range, OutputIt&& it)
              decay_t<OutputIt> partial_sum (\
                Range&& range,               \
                OutputIt&& it,               \
                BinaryOp&& op)

    Computes the partial sum of the slemenets in the subranges of
    :samp:`{range}`. It then writes these values to the range beginning at
    :samp:`{it}`. The first version uses ``operator +``, while the second uses
    the binary operation :samp:`{op}`.

   :requires: :samp:`{range}` must provide InputIterators.
