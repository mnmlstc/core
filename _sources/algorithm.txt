.. _core-algorithm-component:

Algorithm Component
===================

.. default-domain:: cpp
.. highlight:: cpp

The algorithm component can be seen as a competitor to the Boost.Range
algorithm headers. There are a few small differences (namely that this
component relies on the :ref:`core-range-component`), however these differences
are discussed below.

The algorithm component can be found in the ``<core/algorithm.hpp>`` header.

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

   Removes all elements satisfying specific criteris from *range* and returns
   a past-the-end iterator for the new end of the range. The first version
   removes all elements that are equal to *value*, while the second version
   removes all eleents for which *up* returns ``true``.

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

   Copies elements from *range* to another range beignning at *it*, omitting
   the elements which satisfy specific criteria. The first version ignores the
   elements equal to *value*. The second version ignores the elements for which
   *up* returns ``true``.

   :returns: Iterator to the element past the last element copied.
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
 
   Replaces all elements satisfying specific criteria with *value* in *range*.
   The first version replaces elements equal to *old*. The second version
   replaces elements for which *up* returns ``true``.

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

   Copies the elements from *range* to another range beginning at *it*.
   Elements satisfying specific criteria are replaced with *value*. The first
   version replaces elements equal to *old*. The second version replaces
   elements for which *up* returns ``true``. The source and destination
   ranges *cannot* overlap.

   :requires: *range* must provide InputIterators.

.. function:: decay_t<ForwardIt> swap_ranges (Range&& range, ForwardIt&& it)

   Exchanges elements between *range* and another range starting at *it*.

   :returns: Iterator to the element past the last element exchanged with range
             starting at *it*.
   :requires: *range* must provide ForwardIterators.

.. function:: void reverse (Range&& range)

   Reverses the order of the elements in *range*.

   :requires: *range* must provide BidirectionalIterators.

.. function:: decay_t<OutputIt> reverse_copy (Range&& range, OutputIt&& it)

   Copies the elements from *range* to another range starting at *it* where the
   elements in the new range are in reverse order.

   :returns: Output iterator to the element past the last element copied.
   :requires: *range* must provide BidirectionalIterators.

.. function:: void rotate (Range&& range, ForwardIt&& it)

   Performs a left rotation on a range of elements. Specifically, it swaps
   the elements in *range* in such a way that the element at *it* becomes the
   first element of the range.

   .. note:: Due to an incorrect interface in libstdc++, this form of rotate
             returns ``void``. Technically it is required to return a
             ForwardIterator, however this is ignored to take the path of least
             resistance.

   :requires: *range* must provide ForwardIterators.

.. function:: decay_t<OutputIt> rotate_copy (\
                Range&& range,\
                ForwardIt&& it,\
                OutputIt&& ot\
              )

   Copies the elements from *range* to another range starting at *ot* where
   *it* will be the first element of the new range, and *it* - 1 becomes the
   last.

   :returns: Output iterator to the element past the last element copied.
   :requires: *range* must provide ForwardIterators.

.. function:: void shuffle (Range&& range, URNG&& g)

   Reorders elements in *range* so that each possible permutation of those
   elements has equal probablity of appearance. The random number generator 
   is the function object *g*.

   .. note:: As you may have noticed, ``random_shuffle`` does not make an
             appearance. This is due to the C++14 standard deprecating
             ``random_shuffle``.

   :requires: *range* must provide RandomAccessIterators.

.. function:: ForwardIt unique (Range&& range)
              ForwardIt unique (Range&& range, BinaryPredicate&& bp)

   Removes all consecutive duplicate elements from *range* and returns a
   past-the-end iterator for the new logical end of the range. The first
   version uses ``operator ==``. The second version uses the predicate *bp*.

   :requires: *range* must provide ForwardIterators.

.. function:: decay_t<OutputIt> unique_copy (Range&& range, OutputIt&& it)
              decay_t<OutputIt> unique_copy (\
                Range&& range,\
                OutputIt&& it,\
                BinaryPred&& bp\
              )

   Copies the elements from *range* to another range beginning at *it* so
   that no consecutive equal elements exist. The first version uses
   ``operator ==`` to compare elements. The second version uses the predicate
   *bp*.

   :requires: *range* must provide InputIterators.

.. _core-algorithm-component-partitioning-operations:

Partitioning Operations
-----------------------

.. function:: bool is_partitioned (Range&& range, UnaryPredicate&& up)

   :returns: ``true`` if all the elements in *range* that satisfy predicate
             *up* appear before all the elements that don't or if *range* is
             empty.
   :requires: *range* must provide InputIterators.

.. function:: ForwardIt partition (Range&& range, UnaryPredicate&& up)

   Reorders elements in *range* such that all elements for which *up* return
   ``true`` come before the elements where *up* returns ``false``. Relative
   order is *not* preserved.

   :requires: *range* must provide ForwardIterators.

.. function:: partition_copy (\
                Range&& range,\
                OutputTrue&& ot,\
                OutputFalse&& of,\
                UnaryPredicate&& up\
              )

   Copies the elements from *range* to different ranges depending on the result
   of *up*. The elements that cause *up* to return ``true`` are copied to the
   range starting at *ot*, and those that return ``false`` are copied to the
   range starting at *of*.

   It is undefined behavior to have the input range overlap *ot* or *of*.

   :returns: ``std::pair<decay_t<OutputTrue>, decay_t<OutputFalse>>``
   :requires: *range* must provide InputIterators.

.. function:: BidirIt stable_partition (Range&& range, UnaryPredicate&& up)

   Reorders the elements in *range* in the same way as :func:`partition`.
   Unlike :func:`partition`, the order of elements is preserved.

   :requires: *range* must provide BidirectionalIterators.

.. function:: ForwardIt partition_point (Range&& range, UnaryPredicate&& up)

   Examines *range* and locates the end of the first partition (i.e., the first
   element in *range* that does not satisfy *up*. If all elements satisfy
   *up*, the end of *range* is returned.

   :requires: *range* must provide ForwardIterators.

.. _core-algorithm-component-sorting-operations:

Sorting Operations
------------------

.. function:: bool is_sorted (Range&& range)
              bool is_sorted (Range&& range, Compare&& comp)

   Checks if the elements in *range* are sorted in ascending order. The first
   version uses ``operator <`` to compare elements. The second uses the
   comparison function *comp*.

   :requires: *range* must provide ForwardIterators.

.. function:: ForwardIt is_sorted_until (Range&& range)
              ForwardIt is_sorted_until (Range&& range, Compare&& comp)

   Inspects *range* and finds the largest sub range in which elements
   are sorted in ascending order. The first version uses ``operator <``. The
   second version uses the given comparison function *comp*.

   :requires: *range* must provide ForwardIterators.

.. function:: void sort (Range&& range)
              void sort (Range&& range, Compare&& comp)

   Sorts the elements in *range* in ascending order. The order of elements
   equal to each other is no guaranteed to be preserved. The first version
   uses ``operator <``. The second version uses the given comparison function
   *comp*.

   :requires: *range* must provide RandomAccessIterators.

.. function:: void partial_sort (Range&& range, RandomIt&& it)
              void partial_sort (Range&& range, RandomIt&& it, Compare&& cmp)

   Rearranges elements in *range* so that the range contains the sorted
   ``it - range.begin()`` smallest elements.

   The order of elements equal to each other is not guaranteed to be preserved.
   The order of the remaining elements in *range* is unspecified. The first
   version uses ``operator <``. The second version uses the provided comparison
   function *comp*.

   :requires: *range* must provide RandomAccessIterators.

.. function:: RandomIt partial_sort_copy (IRange&& irange, RRange&& rrange)
              RandomIt partial_sort_copy (\
                IRange&& irange,\
                RRange&& rrange,\
                Compare&& cmp\
              )

   Sorts the elements in *irange* in ascending order, storing the result in
   *rrange*. The order of elements which are equal is not guaranteed to be
   preserved. The first version uses ``operator <``. The second uses the
   comparison function *comp*.

   :requires: *irange* must provide InputIterators, *rrange* must provide
              RandomAccessIterators.

.. function:: void stable_sort (Range&& range)
              void stable_sort (Range&& range, Compare&& cmp)

   Sorts elements in *range* in the same way as :func:`sort`, with the exception
   that the order of equal elements is guaranteed to be preserved.

   :requires: *range* must provide RandomAccessIterators.

.. function:: void nth_element (Range&& range, RandomIt&& it)
              void nth_element (Range&& range, RandomIt&& it, Compare&& cmp)

   Partial sorting algorithm that rearranges elements in *range* such that
   the element pointed at by *it* is changed to whatever element would occur in
   that position if *range* was sorted and al of the elements before this new
   element at *it* are less than or equal to the elements after *it*.

   If *it* is the end iterator of *range*, this function has no effect.

   :requires: *range* must provide RandomAccessIterators.

.. _core-algorithm-component-binary-search-operations:

Binary Search Operations
------------------------

.. note:: These operations are intended for sorted/partitioned ranges *only*.

.. function:: ForwardIt lower_bound (Range&& range, T const& value)
              ForwardIt lower_bound (\
                Range&& range,\
                T const& value,\
                Compare&& cmp\
              )

   Returns an iterator pointing to the first element in *range* that is **not
   less than** *value*. The range must be partially ordered. A fully sorted
   range or a range resulting from :func:`partition` meets this criteria. The
   first version uses ``operator <`` to compare elements, while the second uses
   the given function *cmp*.

   :requires: *range* must provide ForwardIterators.

.. function:: ForwardIt upper_bound (Range&& range, T const& value)
              ForwardIt upper_bound (\
                Range&& range,\
                T const& value,\
                Compare&& cmp\
              )

   Returns an iterator pointing to the first element in *range* that is
   *greater* than value. The same ordering restructions in :func:`lower_bound`
   apply. The first version uses ``operator <``. The second uses the
   comparison function *cmp*.

   :requires: *range* must provide ForwardIterators.

.. function:: bool binary_search (Range&& range, T const& value)
              bool binary_search (Range&& range, T const& value, Compare&& cmp)

   Checks if an element equal to *value* resides within *range*. Requires that
   *range* be partitioned. The first version uses ``operator <``. The second
   uses the given function *cmp*.

   :requires: *range* must provide ForwardIterators.

.. function:: range<ForwardIt> equal_range (Range&& range, T const& value)
              range<ForwardIt> equal_range (\
                Range&& range,\
                T const& value,\
                Compare&& cmp\
              )

   Returns a range containing all elements equivalent to *value* in *range*.
   The first version uses ``operator <``. The second uses the given comparison
   function *cmp*.

   :requires: *range* must provide ForwardIterators and must be correctly
              partitioned.

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

   Merges sorted *range1* and sorted *range2* into one sorted range beginning
   at *it*. The first version uses ``operator <`` to compare elements. The
   second uses the comparison function *cmp*. The relative order of elements
   is preserved. If the destination range overlaps either *range1* or *range2*,
   the resulting behavior is undefined. (It is ok if *range1* and *range2*
   are overlapping)

   :requires: *range1* and *range2* must provide InputIterators.

.. function:: void inplace_merge (Range&& range, Bidir&& it)
              void inplace_merge (Range&& range, Bidir&& it, Compare&& cmp)

   Merges two consecutive sorted ranges (``[range.begin(), it)`` and
   ``[it, range.end())``) into one sorted range. The order of equal elements
   is preserved. The first version uses ``operator <``. The second version uses
   the comparison function *cmp*.

   :requires: *range* must provide BidirectionalIterators

.. function:: bool includes (Range1&& range1, Range2&& range2)
              bool includes (Range1&& range1, Range2&& range2, Compare&& cmp)

   Returns ``true`` if every element from *range2* is found within the bounds
   of *range1* or if *range2* is empty. The first version uses ``operator <``.
   The second uses *cmp* as a comparison function.

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

   Copies the elements from *range1* which are not found in *range2* to the
   range beginning at *it*. The first version uses ``operator <``. The second
   uses *cmp* as a comparison function.

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

   Constructs a sorted range beginning at *it* consisting of elements that are
   found in both *range1* and *range2*. The first version expects *range1*
   and *range2* to be sorted with ``operator <``. The second version expects
   them to be sorted by *cmp*.

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

   Copies the symmetric difference of *range1* and *range2* (i.e., the elements
   found in either of the ranges but not both) to a range starting at *it*. The
   result is also sorted. The first version expects *range1* and *range2* to be
   sorted with ``operator <``. The second version expects them to be sorted
   with *cmp*.

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

   Constructs a sorted range starting at *it* consisting of all elements
   present in one or both *range1* and *range2*. The resulting range cannot
   overlap with either *range1* or *range2*. The first version expects both
   ranges to be sorted with ``operator <``. The second version expects them
   to be sorted via *cmp*.

   :requires: *range1* and *range2* must provide InputIterators.

.. _core-algorithm-component-heap-operations:

Heap Operations
---------------

.. function:: bool is_heap (Range&& range)
              bool is_heap (Range&& range, Compare&& compare)

   Checks if the elements in *range* are a max heap. Uses ``operator <`` or
   *cmp* as a comparison function.

   :require: *range* must provide RandomAccessIterators.

.. function:: RandomIt is_heap_until (Range&& range)
              RandomIt is_heap_until (Range&& range, Compare&& compare)

   Find the largest subrange within *range* which is a max heap. Uses
   ``operator <`` or *compare* as the comparison function.

   :require: *range* must provide RandomAccessIterators.

.. function:: void make_heap (Range&& range)
              void make_heap (Range&& range, Compare&& compare)

   Constructs a max heap in *range*. Uses ``operator <`` or *compare*
   as the comparison function.

   :requires: *range* must provide RandomAccessIterators.

.. function:: void push_heap (Range&& range)
              void push_heap (Range&& range, Compare&& compare)

   Inserts the element at ``range.end() - 1`` into the max heap defined by
   ``[range.begin(), range.end() - 1)``. Uses ``operator <`` or *compare*
   as the comparison function.

   :requires: *range* must provide RandomAccessIterators.

.. function:: void pop_heap (Range&& range)
              void pop_heap (Range&& range, Compare&& compare)

   Swaps the value at ``range.begin()`` and the value in ``range.end() - 1``
   and turns this subrange into a max heap. Uses ``operator <`` or *compare*
   as the comparison function.

   :requires: *range* must provide RandomAccessIterators.

.. function:: void sort_heap (Range&& range)
              void sort_heap (Range&& range, Compare&& compare)

   Converts a max heap (*range*) into a sorted range in ascending order.
   The resulting range is no longer a heap. Uses ``operator <`` or *compare*
   as the comparison function.

   :requires: *range* must provide RandomAccessIterators.

.. _core-algorithm-component-min-max-operations:

Min/Max Operations
------------------

.. function:: ForwardIt max_element (Range&& range)
              ForwardIt max_element (Range&& range, Compare&& compare)

   Finds the greatest element in *range*. Uses ``operator <`` or *compare* as
   the comparison function.

   :requires: *range* must provide ForwardIterators.

.. function:: ForwardIt min_element (Range&& range)
              ForwardIt min_element (Range&& range, Compare&& compare)

   Finds the smallest element in *range*. Uses ``operator <`` or *compare* as
   the comparison function.

   :requires: *range* must provide ForwardIterators.

.. function:: std::pair<ForwardIt, ForwardIt> minmax_element (Range&& range)
              std::pair<ForwardIt, ForwardIt> minmax_element (\
                Range&& range,\
                Compare&& compare\
              )

   Finds the greatest and smallest element in *range*. Uses ``operator <`` or
   *compare* as the comparison function.

   :requires: *range* must provide ForwardIterators.

.. function:: bool lexicographical_compare (Range1&& range1, Range2&& range2)
              bool lexicographical_compare (\
                Range1&& range1,\
                Range2&& range2,\
                Compare&& compare\
              )

   Checks if *range1* is lexicographically less than *range2*. Uses
   ``operator <`` or *compare* as the comparison function.

   :requires: *range1* and *range2* must provide InputIterators.

.. function:: is_permutation (Range1&& range1, Range2&& range2)
              is_permutation (\
                Range1&& range1,\
                Range2&& range2,\
                BinaryPredicate&& bp\
              )

   Returns ``true`` if there exists a permutation of the elements in *range*
   that makes it equal to *range2*. The first version uses ``operator ==``.
   The second version uses the given binary predicate *bp*.


   :requires: *range1* and *range2* must provide ForwardIterators.

.. function:: bool next_permutation (Range&& range)
              bool next_permutation (Range&& range, Compare&& compare)

   Transforms *range* into the next permutation from the set of all
   permutations that are lexicographically ordered. The first version
   uses ``operator <``. The second version uses *compare*.

   :returns: ``true`` if such permutation exists otherwise transforms *range*
             into the first permutation and returns ``false``.
   :requires: *range* must provide BidirectionalIterators.

.. function:: bool prev_permutation (Range&& range)
              bool prev_permutation (Range&& range, Compare&& compare)

   Transforms *range* into the previous permutation from the set of all
   permutations that are lexicographically ordered. The first version
   uses ``operator <``. The second version uses *compare*.

   :returns: ``true`` if such permutation exists otherwise transforms *range*
             into the first permutation and returns ``false``.

   :requires: *range* must provide BidirectionalIterators.
