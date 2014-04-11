.. _core-algorithm-component:

Algorithm Component
===================

.. default-domain:: cpp
.. highlight:: cpp

The algorithm component can be seen as a competitor to the Boost.Range
algorithm headers. There are a few small differences (namely that this
component relies on the :ref:`core-range-component`), however these differences
are discussed below.

.. note:: All of the functions in this component take universal references in
   most cases. The reasons for this are:

    * there should be a minimal amount of overhead
    * we shouldn't add to this overhead
    * we have perfect forwarding

   In the author's opinion, causing additional parameters to be passed to the
   underlying functions by value is an unnecessary cost. The only cost that
   should be incurred is the one to :func:`make_range`.

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

.. function:: decay_t<UnaryFunction> for_each (Range&& range, UnaryFunction&& f)

   :returns: *f* by value.
   :requires: *range* must provide InputIterators.

.. function:: difference_type count (Range&& range, T const& value)
              difference_type count_if (Range&& range, UnaryPredicate&& up)

   :returns: Number of elements equal to *value* or times *up* returned true.
   :requires: *range* must provide InputIterators

.. function:: pair<InputIt1, InputIt2> mismatch (Range&& range, InputIt2&& it)
              pair<InputIt1, InputIt2> mismatch (\
                Range&& range,\
                InputIt2&& it,\
                BinaryPredicate&& bp\
              )

   The first overload uses ``operator ==``, while the second uses *bp*.

   :returns: The first mismatching pair of elements from *range* and the
             range starting at *it*.
   :requires: *range* must provide InputIterators

.. function:: bool equal (Range&& range, InputIt&& it)
              bool equal (Range&& range, InputIt&& it, BinaryPredicate&& bp)

   :returns: ``true`` if *range* and the elements in *it* are equal. The
             first version uses ``operator ==``. The second uses *bp*.
   :requires: *range* must provide InputIterators

.. function:: InputIt find (Range&& range, T const& value)
              InputIt find_if (Range&& range, UnaryPredicate&& p)

   :returns: iterator to the item found in *range*. If no item is found or if
             *p* never returns true, the iterator is equal to the end of the
             range.
   :requires: *range* must provide InputIterators

.. function:: ForwardIt find_end (Range1&& range1, Range2&& range2)
              ForwardIt find_end (\
                Range1&& range1,\
                Range2&& range2,\
                BinaryPredicate&& bp\
              )

   Searches for the last subsequence of elements in *range2* within *range1*.
   The first version uses ``operator ==``. The second uses the provided
   binary predicate *bp*.

   :returns: Iterator to the beginning of the last subsequence in *range1*.
   :requires: Both *range1* and *range2* must provide ForwardIterators

.. function:: InputIt find_first_of (IRange&& irange, FRange&& frange)
              InputIt find_first_of (\
                IRange&& irange,\
                FRange&& frange,\
                BinaryPredicate&& bp\
              )

   :returns: Iterator to the first element in *irange* that is also in *frange*.
             If no such element is found, the end of *irange* is returned.
   :requires: *irange* must provide InputIterators, *frange* must provide
              ForwardIterators.

.. function:: ForwardIt adjacent_find (Range&& range)
              ForwardIt adjacent_find (Range&& range, BinaryPredicate&& bp)

   Searches *range* for two consecutive identical elements. The first version
   uses ``operator ==`` to compare the elements, the second version uses the
   given binary predicate *bp*.

   :returns: Forward iterator to the first of the identical elements. If no
             such elements are found, the end of *range* is returned.
   :requires: *range* must provide ForwardIterators.

.. function:: ForwardIt search (Range1&& range1, Range2&& range2)
              ForwardIt search (\
                Range1&& range1,\
                Range2&& range2,\
                BinaryPredicate&& bp\
              )

   Searches for the first occurrence of the subsequence of elements in *range2*
   in *range1*. ``operator ==`` is used for the first version, while *bp* is
   utilized for the second.

   :returns: Forward iterator to the subsequence, if found. Otherwise the end
             of *range1*.
   :requires: *range1* and *range2* must provide ForwardIterators

.. function:: ForwardIt search_n (Range&& range, Size&& count, T const& value)
              ForwardIt search_n (\
                Range&& range,\
                Size&& count,\
                T const& value,\
                BinaryPredicate&& bp\
              )

   Searches *range* for the first sequence of *count* identical elements equal
   to *value*. The first version uses ``operator ==``. The second uses the
   provided binary predicate *bp*.

   :returns: Forward iterator to the start of the discovered sequence of the
             end of *range* if no such sequence was found.
   :requires: *range* must provide ForwardIterators

Modifying Sequence Operations
-----------------------------

.. function:: decay_t<OutputIt> copy (Range&& range, OutputIt&& it)
              decay_t<OutputIt> copy_if (\
                Range&& range,\
                OutputIt&& it,\
                UnaryPredicate&& up\
              )
   Copies the elements in *range* to *it*.

   :returns: Iterator pointing to the last element written.
   :requires: *range* must provide InputIterators

