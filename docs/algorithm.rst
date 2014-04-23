.. _core-algorithm-component:

Algorithm Component
===================

.. default-domain:: cpp
.. highlight:: cpp

The algorithm component can be seen as a competitor to the Boost.Range
algorithm headers. There are a few small differences (namely that this
component relies on the :ref:`core-range-component`), however these differences
are discussed below.

.. note:: All of the functions in this component that take a range take
   universal references in most cases. The reasons for this are:

    * there should be a minimal amount of overhead
    * we shouldn't add to this overhead
    * we have perfect forwarding

   In the author's opinion, causing additional parameters to be passed to the
   underlying functions by value is an unnecessary cost. The only cost that
   should be incurred is the one to :func:`make_range`.

.. _core-algorithm-component-non-modifying-sequence-operations:

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

   :returns: ForwardIterator to the first of the identical elements. If no
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

   :returns: ForwardIterator to the start of the discovered sequence of the
             end of *range* if no such sequence was found.
   :requires: *range* must provide ForwardIterators

.. _core-alglorithm-component-modifying-sequence-operations:

Modifying Sequence Operations
-----------------------------

.. function:: decay_t<OutputIt> copy (Range&& range, OutputIt&& it)
              decay_t<OutputIt> copy_if (\
                Range&& range,\
                OutputIt&& it,\
                UnaryPredicate&& up\
              )

   Copies the elements in *range* to *it*.

   :returns: Iterator to one past the last element written.
   :requires: *range* must provide InputIterators.

.. function:: decay_t<BidirIt> copy_backward(Range&& range, BidirIt&& it)

   Copies the elements from *range* to the range starting at *it*.
   The elements are copied in reverse order (the last element is copied first),
   but their relative order is preserved.

   :returns: Iterator to the last element copied.
   :requires: *range* must provide BidirectionalIterators.


.. function:: decay_t<OutputIt> move (Range&& range, OutputIt&& it)

   Moves the elements in *range* to another range starting at *it*. The
   elements in *range* are in a valid but null state after moving.

   :returns: Iterator to one past the last element written.
   :requires: *range* must provide InputIterators.

.. function:: decay_t<BidirIt> move_backward (Range&& range, BidirIt&& it)

   Moves the elements from *range* to another range starting at *it*.
   The elements are moved in reverse order (the last element is moved first),
   but their relative order is preserved.

   :returns: Iterator to the last element moved.
   :requires: *range* must provide BidirectionalIterators.

.. function:: void fill (Range&& range, T const& value)

   Fills *range* with a copy of *value*.

   :requires: *range* must provide ForwardIterators.

.. function:: decay_t<OutputIt> transform (\
                Range&& range,\
                OutputIt&& it,\
                UnaryOperation&& op\
              )
              decay_t<OutputIt> transform (\
                Range1&& range1,\
                Range2&& range2,\
                OutputIt&& it,\
                BinaryOperation&& op\
              )
              OutputIt transform_if (\
                Range&& range,\
                OutputIt it,\
                UnaryOperation op,\
                UnaryPredicate up\
              )
              OutputIt transform_if (\
                Range1&& range1,\
                Range2&& range2,\
                OutputIt it,\
                BinaryOperation op,\
                BinaryPredicate bp\
              )

   Applies the given function to *range* and stores the result in another
   range, beginning at *it*. The first version applies the unary operation *op*
   to the elements in *range*. The second version applies the binary operation
   *op* to pairs of elements from *range1* and *range2*. The conditional
   versions do not perfectly forward their arguments as the algorithm is
   performed in situ. :func:`transform_if` can be considered a merging of
   :func:`copy_if` and :func:`transform`.

   :returns: Iterator to one past the last element transformed.
   :requires: :func:`transform` uses InputIterators. :func:`transform_if` uses
              ForwardIterators.

.. function:: ForwardIt remove (Range&& range, T const& value)
              ForwardIt remove_if (Range&& range, UnaryPredicate&& up)

   :requires: *range* must provide ForwardIterators.

.. function:: decay_t<OutputIt> remove_copy (\
                Range&& range,\
                OutputIt&& it,\
                T const& value\
              )
              decay_t<OutputIt> remove_copy_if (\
                Range&& range,\
                OutputIt&& it,\
                UnaryPredicate&& up\
              )

   :requires: *range* must provide InputIterators.

.. function:: void remove_erase (Range&& range, T const& val)
              void remove_erase_if (Range&& range, UnaryPredicate&& up)

   Calls :func:`remove_erase` (or :func:`remove_erase_if`), and then calls
   ``::std::forward<Range>(range).erase()`` on the result. These two functions
   are provided because the remove -> erase idiom is extremely common when
   working with containers.

   :requires: The same requirements as :func:`remove` and :func:`remove_if`
              respectively.

.. function:: void replace (Range&& range, T const& old, T const& value)
              void replace_if (Range&& range, UnaryPred&& up, T const& value)

   :requires: *range* must provide ForwardIterators

.. function:: decay_t<OutputIt> replace_copy (\
                Range&& range,\
                OutputIt&& it,\
                T const& old,\
                T const& value\
              )
              decay_t<OutputIt> replace_copy_if (\
                Range&& range,\
                OutputIt&& it,\
                UnaryPred&& up,\
                T const& value\
              )

   :requires: *range* must provide InputIterators.

.. function:: decay_t<ForwardIt> swap_ranges (Range&& range, ForwardIt&& it)

   :requires: *range* must provide ForwardIterators.

.. function:: void reverse (Range&&)

   :requires: *range* must provide BidirectionalIterators.

.. function:: decay_t<OutputIt> reverse_copy (Range&& range, OutputIt&& it)

   :requires: *range* must provide BidirectionalIterators.

.. function:: void rotate (Range&& range, ForwardIt&& it)

   :requires: *range* must provide ForwardIterators.

.. function:: decay_t<OutputIt> rotate_copy (\
                Range&& range,\
                ForwardIt&& it,\
                OutputIt&& ot\
              )

   :requires: *range* must provide ForwardIterators.

.. function:: void shuffle (Range&& range, URNG&& g)

   :requires: *range* must provide RandomAccessIterators.

.. function:: ForwardIt unique (Range&& range)
              ForwardIt unique (Range&& range, BinaryPredicate&& bp)

   :requires: *range* must provide ForwardIterators.

.. function:: decay_t<OutputIt> unique_copy (Range&& range, OutputIt&& it)
              decay_t<OutputIt> unique_copy (\
                Range&& range,\
                OutputIt&& it,\
                BinaryPred&& bp\
              )

   :requires: *range* must provide InputIterators.

.. _core-algorithm-component-partitioning-operations:

Partitioning Operations
-----------------------

.. function:: bool is_partitioned (Range&& range, UnaryPredicate&& up)

   :requires: *range* must provide InputIterators.

.. function:: ForwardIt partition (Range&& range, UnaryPredicate&& up)

   :requires: *range* must provide ForwardIterators.

.. function:: partition_copy (\
                Range&& range,\
                OutputTrue&& ot,\
                OutputFalse&& of,\
                UnaryPredicate&& up\
              )

   :returns: ``std::pair<decay_t<OutputTrue>, decay_t<OutputFalse>>``
   :requires: *range* must provide InputIterators.

.. function:: BidirIt stable_partition (Range&& range, UnaryPredicate&& up)

   :requires: *range* must provide BidirectionalIterators.

.. function:: ForwardIt partition_point (Range&& range, UnaryPredicate&& up)

   :requires: *range* must provide ForwardIterators.

.. _core-algorithm-component-sorting-operations:

Sorting Operations
------------------

.. function:: bool is_sorted (Range&& range)
              bool is_sorted (Range&& range, Compare&& comp)

   :requires: *range* must provide ForwardIterators.

.. function:: ForwardIt is_sorted_until (Range&& range)
              ForwardIt is_sorted_until (Range&& range, Compare&& comp)

   :requires: *range* must provide ForwardIterators.

.. function:: void sort (Range&& range)
              void sort (Range&& range, Compare&& comp)

   :requires: *range* must provide RandomAccessIterators.

.. function:: void partial_sort (Range&& range, RandomIt&& it)
              void partial_sort (Range&& range, RandomIt&& it, Compare&& cmp)

   :requires: *range* must provide RandomAccessIterators.

.. function:: RandomIt partial_sort_copy (IRange&& irange, RRange&& rrange)
              RandomIt partial_sort_copy (\
                IRange&& irange,\
                RRange&& rrange,\
                Compare&& cmp\
              )

   :requires: *irange* must provide InputIterators, *rrange* must provide
              RandomAccessIterators.

.. function:: void stable_sort (Range&& range)
              void stable_sort (Range&& range, Compare&& cmp)

   :requires: *range* must provide RandomAccessIterators.

.. function:: void nth_element (Range&& range, RandomIt&& it)
              void nth_element (Range&& range, RandomIt&& it, Compare&& cmp)

   :requires: *range* must provide RandomAccessIterators.

.. _core-algorithm-component-binary-search-operations:

Binary Search Operations
------------------------

.. note:: These operations are intended for sorted ranges *only*.

.. function:: ForwardIt lower_bound (Range&& range, T const& value)
              ForwardIt lower_bound (\
                Range&& range,\
                T const& value,\
                Compare&& cmp\
              )

   :requires: *range* must provide ForwardIterators.

.. function:: ForwardIt upper_bound (Range&& range, T const& value)
              ForwardIt upper_bound (\
                Range&& range,\
                T const& value,\
                Compare&& cmp\
              )

   :requires: *range* must provide ForwardIterators.

.. function:: bool binary_search (Range&& range, T const& value)
              bool binary_search (Range&& range, T const& value, Compare&& cmp)

   :requires: *range* must provide ForwardIterators.

.. function:: range<ForwardIt> equal_range (Range&& range, T const& value)
              range<ForwardIt> equal_range (\
                Range&& range,\
                T const& value,\
                Compare&& cmp\
              )

   :requires: *range* must provide ForwardIterators.

.. _core-algorithm-component-set-operations:

Set Operations
--------------

.. function:: decay_t<OutputIt> merge (\
                Range1&& range1,\
                Range2&& range2,\
                OutputIt&& it\
              )
              decay_t<OutputIt> merge (\
                Range1&& range1,\
                Range2&& range2,\
                OutputIt&& it,\
                Compare&& cmp\
              )

   :requires: *range1* and *range2* must provide InputIterators.

.. function:: void inplace_merge (Range&& range, Bidir&& it)
              void inplace_merge (Range&& range, Bidir&& it, Compare&& cmp)

   :requires: *range* must provide BidirectionalIterators

.. function:: bool includes (Range1&& range1, Range2&& range2)
              bool includes (Range1&& range1, Range2&& range2, Compare&& cmp)

   :requires: *range1* and *range2* must provide InputIterators.

.. function:: decay_t<OutputIt> set_difference (\
                Range1&& range1,\
                Range2&& range2,\
                OutputIt&& it\
              )
              decay_t<OutputIt> set_difference (\
                Range1&& range1,\
                Range2&& range2,\
                OutputIt&& it,\
                Compare&& cmp\
              )

   :requires: *range1* and *range2* must provide InputIterators.

.. function:: decay_t<OutputIt> set_intersection (\
                Range1&& range1,\
                Range2&& range2,\
                OutputIt&& it\
              )
              decay_t<OutputIt> set_intersection (\
                Range1&& range1,\
                Range2&& range2,\
                OutputIt&& it,\
                Compare&& cmp\
              )

   :requires: *range1* and *range2* must provide InputIterators.

.. function:: decay_t<OutputIt> set_symmetric_difference (\
                Range1&& range1,\
                Range2&& range2,\
                OutputIt&& it\
              )
              decay_t<OutputIt> set_symmetric_difference (\
                Range1&& range1,\
                Range2&& range2,\
                OutputIt&& it,\
                Compare&& cmp\
              )

   :requires: *range1* and *range2* must provide InputIterators.

.. function:: decay_t<OutputIt> set_union (\
                Range1&& range1,\
                Range2&& range2,\
                OutputIt&& it\
              )
              decay_t<OutputIt> set_union (\
                Range1&& range1,\
                Range2&& range2,\
                OutputIt&& it,\
                Compare&& cmp\
              )

   :requires: *range1* and *range2* must provide InputIterators.

.. _core-algorithm-component-heap-operations:

Heap Operations
---------------

.. function:: bool is_heap (Range&& range)
              bool is_heap (Range&& range, Compare&& compare)

   :require: *range* must provide RandomAccessIterators.

.. function:: RandomIt is_heap_until (Range&& range)
              RandomIt is_heap_until (Range&& range, Compare&& compare)

   :require: *range* must provide RandomAccessIterators.

.. function:: void make_heap (Range&& range)
              void make_heap (Range&& range, Compare&& compare)

   :requires: *range* must provide RandomAccessIterators.

.. function:: void push_heap (Range&& range)
              void push_heap (Range&& range, Compare&& compare)

   :requires: *range* must provide RandomAccessIterators.

.. function:: void pop_heap (Range&& range)
              void pop_heap (Range&& range, Compare&& compare)

   :requires: *range* must provide RandomAccessIterators.

.. function:: void sort_heap (Range&& range)
              void sort_heap (Range&& range, Compare&& compare)

   :requires: *range* must provide RandomAccessIterators.

.. _core-algorithm-component-min-max-operations:

Min/Max Operations
------------------

.. function:: ForwardIt max_element (Range&& range)
              ForwardIt max_element (Range&& range, Compare&& compare)

   :requires: *range* must provide ForwardIterators.

.. function:: ForwardIt min_element (Range&& range)
              ForwardIt min_element (Range&& range, Compare&& compare)

   :requires: *range* must provide ForwardIterators.

.. function:: std::pair<ForwardIt, ForwardIt> minmax_element (Range&& range)
              std::pair<ForwardIt, ForwardIt> minmax_element (\
                Range&& range,\
                Compare&& compare\
              )

   :requires: *range* must provide ForwardIterators.

.. function:: bool lexicographical_compare (Range1&& range1, Range2&& range2)
              bool lexicographical_compare (\
                Range1&& range1,\
                Range2&& range2,\
                Compare&& compare\
              )

   :requires: *range1* and *range2* must provide InputIterators.

.. function:: is_permutation (Range1&& range1, Range2&& range2)
              is_permutation (\
                Range1&& range1,\
                Range2&& range2,\
                BinaryPredicate&& bp\
              )

   :requires: *range1* and *range2* must provide ForwardIterators.

.. function:: bool next_permutation (Range&& range)
              bool next_permutation (Range&& range, Compare&& compare)

   :requires: *range* must provide BidirectionalIterators.

.. function:: bool prev_permutation (Range&& range)
              bool prev_permutation (Range&& range, Compare&& compare)

   :requires: *range* must provide BidirectionalIterators.
