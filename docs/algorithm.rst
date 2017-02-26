Algorithm Component
===================

.. index:: algorithm

The algorithm component can be seen as a competitor to the Boost.Range
algorithm headers. There are a few small differences (namely that this
component relies on the :doc:`range`), however these differences are discussed
below.

The algorithm component can be found in the :file:`<core/{algorithm}.hpp>`
header.

.. namespace:: core

.. note:: All of the functions in this component that take a range take
   forwarding references in most cases. The reasons for this are:

    * there should be a minimal amount of overhead
    * we shouldn't add to this overhead
    * we have perfect forwarding

   In the author's opinion, causing additional parameters to be passed to the
   underlying functions by value is an unnecessary cost. The only additional
   cost that should be incurred is the one to :any:`make_range`.

   This does not apply for the custom algorithms provided by this component.

Differences with Boost.Range Algorithms
---------------------------------------

One of the primary differences with this component and the Boost.Range
module is the lack of composable function objects. The approach for this
component was to be a bit more lightweight than Boost, and not adding objects
that can be *piped* to compose higher order algorithms was a design decision.

Whether this was a correct decision or not is irrevelant given the current
state of this component, and it cannot, unfortunately, be changed without
breaking the current ABI.

.. _core-algorithm-component-non-modifying-sequence-operations:

.. index:: algorithm; non-range

Non-Range Based Operations
--------------------------

.. function:: constexpr T const& min (T const& lhs, T const& rhs, Compare comp)
              constexpr T const& min (T const& lhs, T const& rhs)

   .. versionadded:: 1.2

   :returns: The smaller of :samp:`{lhs}` and :samp:`{rhs}`.
             Uses :cxx:`operator <` to compare by default.

.. function:: constexpr T const& max (T const& lhs, T const& rhs, Compare comp)
              constexpr T const& max (T const& lhs, T const& rhs)

   .. versionadded:: 1.2

   :returns: The larger of :samp:`{lhs}` and :samp:`{rhs}`. Uses
             :cxx:`operator <` to compare by default.

.. function:: constexpr T const& clamp (\
                T const& value,         \
                T const& low,           \
                T const& high,          \
                Compare comp)
              constexpr T const& clamp (\
              T const& value,           \
              T const& low,             \
              T const& high)

   .. versionadded:: 1.2

   :returns: :samp:`{value}` if it is within the value boundaries of
             :samp:`{low}` and :samp:`{high}`. Otherwise it returns the
             :any:`max` of :samp:`{low}` and :samp:`{value}` or the
             :any:`min` of :samp:`{high}` and :samp:`{value}`.

   Clamps :samp:`{value}` to either :samp:`{low}` or :samp:`{high}` if
   :samp:`{comp}` returns false for its comparison operation. Uses
   :cxx:`operator <` by default.

.. function:: constexpr auto abs_diff (\
                T const& a,            \
                T const& b,            \
                Compare compare,       \
                Difference diff)
              constexpr auto abs_diff (T const& a, T const& b, Compare compare)
              constexpr auto abs_diff (T const& a, T const& b)

   .. versionadded:: 1.2

   An implementation of :wg21:`N4318`.

   This fixes an issue regarding the :cxx:`std::abs` function, where a call
   to :cxx:`std::abs(3u, 5u)` can result in a return value of 4294967294.
   This function insures that the correct value (2) is returned instead.

   The default :samp:`{Compare}` and :samp:`{Difference}` functions are
   :cxx:`operator <` and :cxx:`operator -` respectively

.. index:: algorithm; non-modifying

Non-Modifying Sequence Operations
---------------------------------

.. function:: bool all_of (Range&& range, UnaryPredicate&& up)

   :returns: :cxx:`true` if :samp:`{up}` returns :cxx:`true` for **all**
              elements in :samp:`{range}`.
   :requires: :samp:`{range}` must provide InputIterators.

   :example:

   .. code-block:: cpp

      std::vector<int> values { 1, 2, 3, 4, 5, 6 };
      assert(all_of(values, [](int v) { return v <= 6; }));


.. function:: bool any_of (Range&& range, UnaryPredicate&& up)

   :returns: :cxx:`true` if :samp:`{up}` returns :cxx:`true` for **any**
             elements in :samp:`{range}`.
   :requires: :samp:`{range}` must provide InputIterators.

   :example:

   .. code-block:: cpp

      std::vector<int> values { 8, 9, 1, 10, 15, 16 };
      assert(any_of(values, [](int v) { return v < 6; }));


.. function:: bool none_of (Range&& range, UnaryPredicate&& up)

   :returns: :cxx:`true` if :samp:`{up}` returns :cxx:`false` for **all**
             elements in :samp:`{range}`.
   :requires: :samp:`{range}` must provide InputIterators.

   :example:

   .. code-block:: cpp

      std::vector<int> values { 1, 2, 3, 4, 5, 6 };
      assert(none_of(values, [](int v) { return v > 6; }));

.. function:: UnaryFunction for_each (Range&& range, UnaryFunction&& f)

   Iterates over each element in :samp:`{range}` and invokes :samp:`{f}` on
   each element.

   :returns: :samp:`{f}` by value.
   :requires: :samp:`{range}` must provide InputIterators.

   :example:

   .. code-block:: cpp

      struct sum {
        sum () = default;
        void operator () (int n) { value += n; }
        int value { 0 };
      };

      std::vector<int> numbers { 1, 2, 3, 4 };
      auto s = for_each(numbers, sum());
      assert(s.value == 10);


.. function:: InputIt for_each_while (      \
                Range&& r,                  \
                UnaryFunction f,            \
                UnaryPredicate p)

   .. versionadded:: 1.2

   Iterates over a range :samp:`{r}`, and calls :samp:`{f}` on each element
   until the predicate :samp:`{p}` returns :cxx:`false`. This permits early
   termination of a :any:`for_each` on a given range, and can save some
   iteration time instead of performing a :any:`find_if` followed by
   :any:`for_each`.

   :returns: InputIterator to the first element where :samp:`{p}` returned
             :cxx:`false`, or the end of the range :samp:`{r}` if no element
             returned :cxx:`false`.
   :requires: :samp:`{r}` must provide InputIterators.

   :example:

   .. code-block:: cpp

      std::vector<int> numbers { 1, 2, 3, 4 };
      int count = 3;
      auto function = [&] (int v) { count -= v; };
      auto predicate = [&] (int) { return count; };
      for_each_while(numbers, function, predicate);

.. function:: InputIt for_each_until (      \
                Range&& r,                  \
                UnaryFunction f,            \
                T const& value)

   .. versionadded:: 1.2

   Iterates over a range :samp:`{r}`, and calls :samp:`{f}` on each element
   until an element is found equal to :samp:`{T}`. This permits early
   termination of a :any:`for_each` on a given range, and can save some
   iteration time instead of calling :any:`find`, followed by :any:`for_each`.

   :returns: InputIterator to the first element where :samp:`{f}` was not
             executed.
   :requires: :samp:`{r}` must provide InputIterators

   :example:

   .. code-block:: cpp

      std::vector<int> numbers { 1, 2, 3, 4, 5, 7 };
      auto function = [] (int x) { std::cout << x << std::endl; };
      for_each_until(numbers, function, 7);

.. function:: UnaryFunction for_each_if( \
                Range&& r,               \
                UnaryFunction f,         \
                UnaryPredicate up)

   .. versionadded:: 1.2

   Iterates over a range :samp:`{r}`, and calls :samp:`{f}` on an element if it
   satisfies the predicate :samp:`{up}`.

   :returns: :cxx:`std::move(f)`
   :requires: :samp:`{r}` must provide InputIterators

   :example:

   .. code-block:: cpp

      // prints only numbers not divisible by 2
      std::vector<int> numbers { 1, 2, 3, 4, 5, 6, 7, 8 };
      auto function = [] (int x) { std::cout << x << std::endl; }
      auto predicate = [] (int x) { return x % 2; }
      for_each_if(numbers, function, predicate);

.. function:: difference_type count (Range&& range, T const& value)
              difference_type count_if (Range&& range, UnaryPredicate&& up)

   :returns: Number of elements equal to :samp:`{value}` or times :samp:`{up}`
             returned true.
   :requires: :samp:`{range}` must provide InputIterators

.. function:: pair<InputIt1, InputIt2> mismatch (Range&& range, InputIt2&& it)
              pair<InputIt1, InputIt2> mismatch (\
                Range&& range,\
                InputIt2&& it,\
                BinaryPredicate&& bp)

   The first overload uses :cxx:`operator ==`, while the second uses
   :samp:`{bp}`.

   :returns: The first mismatching pair of elements from :samp:`{range}` and
             the range starting at :samp:`{it}`.
   :requires: :samp:`{range}` must provide InputIterators

.. function:: bool equal (InputIt1 first1, InputIt1 last1, \
                          InputIt2 first2, InputIt2 last2, \
                          BinaryPredicate bp)
              bool equal (InputIt1 first1, InputIt1 last1, \
                          InputIt2 first2, InputIt2 last2)

    .. versionadded:: 1.2

    :returns: :cxx:`true` if the range :samp:`[{first1}, {last1})` is equal
              to :samp:`[{first2}, {last2})`. Uses :cxx:`operator ==` if no
              :samp:`{bp}` is given.

.. function:: bool equal (Range&& range, InputIt&& it, BinaryPredicate&& bp)
              bool equal (Range&& range, InputIt&& it)
              
   :returns: :cxx:`true` if :samp:`{range}` and the elements in :samp:`{it}`
             are equal. Uses :cxx:`operator ==` if no :samp:`{bp}` is given.
   :requires: :samp:`{range}` must provide InputIterators

.. function:: bool equal (Range1&& r1, Range2&& r2, BinaryPredicate bp)
              bool equal (Range1&& r1, Range2&& r2)

   .. versionadded:: 1.2

   :returns: :cxx:`true` if the all the elements in range :samp:`{r1}` are
             equal to all the elements in the range :samp:`{r2}`. Uses
             :cxx:`operator ==` if no :samp:`{bp}` is given.

.. function:: InputIt find (Range&& range, T const& value)
              InputIt find_if (Range&& range, UnaryPredicate&& p)

   :returns: iterator to the item found in :samp:`{range}`. If no item is found
             or if :samp:`{p}` never returns true, the iterator is equal to the
             end of the range.
   :requires: :samp:`{range}` must provide InputIterators

.. function:: ForwardIt find_end (Range1&& range1, Range2&& range2)
              ForwardIt find_end (\
                Range1&& range1,\
                Range2&& range2,\
                BinaryPredicate&& bp)

   Searches for the last subsequence of elements in :samp:`{range2}` within
   :samp:`{range1}`. The first version uses `operator ==`. The second uses the
   provided binary predicate :samp:`{bp}`.

   :returns: Iterator to the beginning of the last subsequence in
             :samp:`{range1}`.
   :requires: Both :samp:`{range1}` and :samp:`{range2}` must provide
              ForwardIterators

.. function:: InputIt find_first_of (IRange&& irange, FRange&& frange)
              InputIt find_first_of (\
                IRange&& irange,\
                FRange&& frange,\
                BinaryPredicate&& bp)

   :returns: Iterator to the first element in :samp:`{irange}` that is also in
             :samp:`{frange}`. If no such element is found, the end of
             :samp:`{irange}` is returned.
   :requires: :samp:`{irange}` must provide InputIterators, :samp:`{frange}`
              must provide ForwardIterators.

.. function:: ForwardIt adjacent_find (Range&& range)
              ForwardIt adjacent_find (Range&& range, BinaryPredicate&& bp)

   Searches :samp:`{range}` for two consecutive identical elements. The first
   version uses :cxx:`operator ==` to compare the elements, the second version
   uses the given binary predicate :samp:`{bp}`.

   :returns: ForwardIterator to the first of the identical elements. If no
             such elements are found, the end of :samp:`{range}` is returned.
   :requires: :samp:`{range}` must provide ForwardIterators.

.. function:: ForwardIt search (Range1&& range1, Range2&& range2)
              ForwardIt search (\
                Range1&& range1,\
                Range2&& range2,\
                BinaryPredicate&& bp)

   Searches for the first occurrence of the subsequence of elements in
   :samp:`{range2}` in :samp:`{range1}`. :cxx:`operator ==` is used for the
   first version, while :samp:`{bp}` is utilized for the second.

   :returns: Forward iterator to the subsequence, if found. Otherwise the end
             of :samp:`{range1}`.
   :requires: :samp:`{range1}` and :samp:`{range2}` must provide
              ForwardIterators

.. function:: ForwardIt search_n (Range&& range, Size&& count, T const& value)
              ForwardIt search_n (\
                Range&& range,\
                Size&& count,\
                T const& value,\
                BinaryPredicate&& bp)

   Searches :samp:`{range}` for the first sequence of :samp:`{count}` identical
   elements equal to :samp:`{value}`. The first version uses
   :cxx:`operator ==`. The second uses the provided binary predicate
   :samp:`{bp}`.

   :returns: ForwardIterator to the start of the discovered sequence of the
             end of :samp:`{range}` if no such sequence was found.
   :requires: :samp:`{range}` must provide ForwardIterators

.. _core-alglorithm-component-modifying-sequence-operations:

.. index:: algorithm; modifying

Modifying Sequence Operations
-----------------------------

.. function:: OutputIt copy (Range&& range, OutputIt&& it)
              OutputIt copy_if (\
                Range&& range,\
                OutputIt&& it,\
                UnaryPredicate&& up)

   Copies the elements in :samp:`{range}` to :samp:`{it}`.

   :returns: Iterator to one past the last element written.
   :requires: :samp:`{range}` must provide InputIterators.

.. function:: BidirIt copy_backward(Range&& range, BidirIt&& it)

   Copies the elements from :samp:`{range}` to the range starting at
   :samp:`{it}`. The elements are copied in reverse order (the last element is
   copied first), but their relative order is preserved.

   :returns: Iterator to the last element copied.
   :requires: :samp:`{range}` must provide BidirectionalIterators.

.. function:: OutputIt move (Range&& range, OutputIt&& it)

   Moves the elements in :samp:`{range}` to another range starting at
   :samp:`{it}`. The elements in :samp:`{range}` are in a valid but null state
   after moving.

   :returns: Iterator to one past the last element written.
   :requires: :samp:`{range}` must provide InputIterators.

.. function:: BidirIt move_backward (Range&& range, BidirIt&& it)

   Moves the elements from :samp:`{range}` to another range starting at
   :samp:`{it}`. The elements are moved in reverse order (the last element is
   moved first), but their relative order is preserved.

   :returns: Iterator to the last element moved.
   :requires: :samp:`{range}` must provide BidirectionalIterators.

.. function:: void fill (Range&& range, T const& value)

   Fills :samp:`{range}` with a copy of :samp:`{value}`.

   :requires: :samp:`{range}` must provide ForwardIterators.

.. function:: OutputIt transform (\
                Range&& range,    \
                OutputIt&& it,    \
                UnaryOperation&& op)
              OutputIt transform (\
                Range1&& range1,  \
                Range2&& range2,  \
                OutputIt&& it,    \
                BinaryOperation&& op)
              OutputIt transform (\
                Range&& range,    \
                InputIt in,       \
                OutputIt it,      \
                BinaryOperation&& op)
              OutputIt transform_if (\
                Range&& range,       \
                OutputIt it,         \
                UnaryOperation op,   \
                UnaryPredicate up)
              OutputIt transform_if (\
                Range1&& range1,     \
                Range2&& range2,     \
                OutputIt it,         \
                BinaryOperation op,  \
                BinaryPredicate bp)

   Applies the given function to :samp:`{range}` and stores the result in
   another range, beginning at :samp:`{it}`. The first version applies the
   unary operation :samp:`{op}` to the elements in :samp:`{range}`. The second
   version applies the binary operation :samp:`{op}` to pairs of elements from
   :samp:`{range1}` and :samp:`{range2}`. The third version works in the same
   way as the second, but is for cases where the second sequence is simply an
   input iterator. The conditional versions do not perfectly forward their
   arguments as the algorithm is performed in situ. :any:`transform_if` can be
   considered a merging of :any:`copy_if` and :any:`transform`.

   :returns: Iterator to one past the last element transformed.
   :requires: :any:`transform` uses InputIterators. :any:`transform_if` uses
              ForwardIterators.

.. function:: ForwardIt remove (Range&& range, T const& value)
              ForwardIt remove_if (Range&& range, UnaryPredicate&& up)

   Removes all elements satisfying specific criteris from :samp:`{range}` and
   returns a past-the-end iterator for the new end of the range. The first
   version removes all elements that are equal to :samp:`{value}`, while the
   second version removes all eleents for which :samp:`{up}` returns
   :cxx:`true`.

   :requires: :samp:`{range}` must provide ForwardIterators.

.. function:: OutputIt remove_copy (\
                Range&& range,\
                OutputIt&& it,\
                T const& value)
              OutputIt remove_copy_if (\
                Range&& range,\
                OutputIt&& it,\
                UnaryPredicate&& up)

   Copies elements from :samp:`{range}` to another range beignning at
   :samp:`{it}`, omitting the elements which satisfy specific criteria. The
   first version ignores the elements equal to :samp:`{value}`. The second
   version ignores the elements for which :samp:`{up}` returns :cxx:`true`.

   :returns: Iterator to the element past the last element copied.
   :requires: :samp:`{range}` must provide InputIterators.

.. function:: void remove_erase (Range&& range, T const& val)
              void remove_erase_if (Range&& range, UnaryPredicate&& up)

   Calls :any:`remove_erase` (or :any:`remove_erase_if`), and then calls
   :samp:`::std::forward<{Range}>({range}).erase()` on the result. These two
   functions are provided because the remove -> erase idiom is extremely common
   when working with containers.

   :requires: The same requirements as :any:`remove` and :any:`remove_if`
              respectively.

.. function:: void replace (Range&& range, T const& old, T const& value)
              void replace_if (Range&& range, UnaryPred&& up, T const& value)
 
   Replaces all elements satisfying specific criteria with :samp:`{value}` in
   :samp:`{range}`. The first version replaces elements equal to :samp:`{old}`.
   The second version replaces elements for which :samp:`{up}` returns
   :cxx:`true`.

   :requires: :samp:`{range}` must provide ForwardIterators

.. function:: OutputIt replace_copy (\
                Range&& range,\
                OutputIt&& it,\
                T const& old,\
                T const& value)
              OutputIt replace_copy_if (\
                Range&& range,\
                OutputIt&& it,\
                UnaryPred&& up,\
                T const& value)

   Copies the elements from :samp:`{range}` to another range beginning at
   :samp:`{it}`. Elements satisfying specific criteria are replaced with
   :samp:`{value}`. The first version replaces elements equal to :samp:`{old}`.
   The second version replaces elements for which :samp:`{up}` returns
   :cxx:`true`. The source and destination ranges :samp:`{cannot}` overlap.

   :requires: :samp:`{range}` must provide InputIterators.

.. function:: ForwardIt swap_ranges (Range&& range, ForwardIt&& it)

   Exchanges elements between :samp:`{range}` and another range starting at
   :samp:`{it}`.

   :returns: Iterator to the element past the last element exchanged with range
             starting at :samp:`{it}`.
   :requires: :samp:`{range}` must provide ForwardIterators.

.. function:: void reverse (Range&& range)

   Reverses the order of the elements in :samp:`{range}`.

   :requires: :samp:`{range}` must provide BidirectionalIterators.

.. function:: OutputIt reverse_copy (Range&& range, OutputIt&& it)

   Copies the elements from :samp:`{range}` to another range starting at
   :samp:`{it}` where the elements in the new range are in reverse order.

   :returns: Output iterator to the element past the last element copied.
   :requires: :samp:`{range}` must provide BidirectionalIterators.

.. function:: void rotate (Range&& range, ForwardIt&& it)

   Performs a left rotation on a range of elements. Specifically, it swaps
   the elements in :samp:`{range}` in such a way that the element at
   :samp:`{it}` becomes the first element of the range.

   .. note:: Due to an incorrect interface in libstdc++, this form of rotate
             returns :cxx:`void`. Technically it is required to return a
             ForwardIterator, however this is ignored to take the path of least
             resistance.

   :requires: :samp:`{range}` must provide ForwardIterators.

.. function:: OutputIt rotate_copy (\
                Range&& range,\
                ForwardIt&& it,\
                OutputIt&& ot)

   Copies the elements from :samp:`{range}` to another range starting at
   :samp:`{ot}` where :samp:`{it}` will be the first element of the new range,
   and :samp:`{it} - 1` becomes the last.

   :returns: Output iterator to the element past the last element copied.
   :requires: :samp:`{range}` must provide ForwardIterators.

.. function:: void shuffle (Range&& range, URNG&& g)

   Reorders elements in :samp:`{range}` so that each possible permutation of those
   elements has equal probablity of appearance. The random number generator 
   is the function object :samp:`{g}`.

   .. note:: As you may have noticed, :cxx:`random_shuffle` does not make an
             appearance. This is due to the C++14 standard deprecating
             :samp:`random_shuffle`.

   :requires: :samp:`{range}` must provide RandomAccessIterators.

.. function:: ForwardIt unique (Range&& range)
              ForwardIt unique (Range&& range, BinaryPredicate&& bp)

   Removes all consecutive duplicate elements from :samp:`{range}` and returns
   a past-the-end iterator for the new logical end of the range. The first
   version uses :cxx:`operator ==`. The second version uses the predicate
   :samp:`{bp}`.

   :requires: :samp:`{range}` must provide ForwardIterators.

.. function:: OutputIt unique_copy (Range&& range, OutputIt&& it)
              OutputIt unique_copy (\
                Range&& range,\
                OutputIt&& it,\
                BinaryPred&& bp)

   Copies the elements from :samp:`{range}` to another range beginning at
   :samp:`{it}` so that no consecutive equal elements exist. The first version
   uses :cxx:`operator ==` to compare elements. The second version uses the
   predicate :samp:`{bp}`.

   :requires: :samp:`{range}` must provide InputIterators.

.. _core-algorithm-component-partitioning-operations:

.. index:: algorithm; partitioning

Partitioning Operations
-----------------------

.. function:: bool is_partitioned (Range&& range, UnaryPredicate&& up)

   :returns: :cxx:`true` if all the elements in :samp:`{range}` that satisfy
             predicate :samp:`{up}` appear before all the elements that don't
             or if :samp:`{range}` is empty.
   :requires: :samp:`{range}` must provide InputIterators.

.. function:: ForwardIt partition (Range&& range, UnaryPredicate&& up)

   Reorders elements in :samp:`{range}` such that all elements for which
   :samp:`{up}` return :cxx:`true` come before the elements where :samp:`{up}`
   returns ``false``. Relative order is *not* preserved.

   :requires: :samp:`{range}` must provide ForwardIterators.

.. function:: partition_copy (\
                Range&& range,\
                OutputTrue&& ot,\
                OutputFalse&& of,\
                UnaryPredicate&& up)

   Copies the elements from :samp:`{range}` to different ranges depending on
   the result of :samp:`{up}`. The elements that cause :samp:`{up}` to return
   :cxx:`true` are copied to the range starting at :samp:`{ot}`, and those that
   return :cxx:`false` are copied to the range starting at :samp:`{of}`.

   It is undefined behavior to have the input range overlap :samp:`{ot}` or
   :samp:`{of}`.

   :returns: :samp:`std::pair<decay_t<{OutputTrue}>, decay_t<{OutputFalse}>>`
   :requires: :samp:`{range}` must provide InputIterators.

.. function:: BidirIt stable_partition (Range&& range, UnaryPredicate&& up)

   Reorders the elements in :samp:`{range}` in the same way as
   :any:`partition`. Unlike :any:`partition`, the order of elements is
   preserved.

   :requires: :samp:`{range}` must provide BidirectionalIterators.

.. function:: ForwardIt partition_point (Range&& range, UnaryPredicate&& up)

   Examines :samp:`{range}` and locates the end of the first partition (i.e.,
   the first element in :samp:`{range}` that does not satisfy :samp:`{up}`. If
   all elements satisfy :samp:`{up}`, the end of :samp:`{range}` is returned.

   :requires: :samp:`{range}` must provide ForwardIterators.

.. _core-algorithm-component-sorting-operations:

.. index:: algorithm; sorting

Sorting Operations
------------------

.. function:: bool is_sorted (Range&& range)
              bool is_sorted (Range&& range, Compare&& comp)

   Checks if the elements in :samp:`{range}` are sorted in ascending order. The
   first version uses :cxx:`operator <` to compare elements. The second uses
   the comparison function :samp:`{comp}`.

   :requires: :samp:`{range}` must provide ForwardIterators.

.. function:: ForwardIt is_sorted_until (Range&& range)
              ForwardIt is_sorted_until (Range&& range, Compare&& comp)

   Inspects :samp:`{range}` and finds the largest sub range in which elements
   are sorted in ascending order. The first version uses :cxx:`operator <`. The
   second version uses the given comparison function :samp:`{comp}`.

   :requires: :samp:`{range}` must provide ForwardIterators.

.. function:: void sort (Range&& range)
              void sort (Range&& range, Compare&& comp)

   Sorts the elements in :samp:`{range}` in ascending order. The order of
   elements equal to each other is no guaranteed to be preserved. The first
   version uses :cxx:`operator <`. The second version uses the given comparison
   function :samp:`{comp}`.

   :requires: :samp:`{range}` must provide RandomAccessIterators.

.. function:: void partial_sort (Range&& range, RandomIt&& it)
              void partial_sort (Range&& range, RandomIt&& it, Compare&& cmp)

   Rearranges elements in :samp:`{range}` so that the range contains the sorted
   :samp:`it - {range}.begin()` smallest elements.

   The order of elements equal to each other is not guaranteed to be preserved.
   The order of the remaining elements in :samp:`range}` is unspecified. The
   first version uses :cxx:`operator <`. The second version uses the provided
   comparison function :samp:`{comp}`.

   :requires: :samp:`{range}` must provide RandomAccessIterators.

.. function:: RandomIt partial_sort_copy (IRange&& irange, RRange&& rrange)
              RandomIt partial_sort_copy (\
                IRange&& irange,\
                RRange&& rrange,\
                Compare&& cmp)

   Sorts the elements in :samp:`{irange}` in ascending order, storing the
   result in :samp:`{rrange}`. The order of elements which are equal is not
   guaranteed to be preserved. The first version uses :cxx:`operator <`. The
   second uses the comparison function :samp:`{comp}`.

   :requires: :samp:`{irange}` must provide InputIterators, :samp:`{rrange}`
              must provide RandomAccessIterators.

.. function:: void stable_sort (Range&& range)
              void stable_sort (Range&& range, Compare&& cmp)

   Sorts elements in :samp:`{range}` in the same way as :any:`sort`, with the
   exception that the order of equal elements is guaranteed to be preserved.

   :requires: :samp:`{range}` must provide RandomAccessIterators.

.. function:: void nth_element (Range&& range, RandomIt&& it)
              void nth_element (Range&& range, RandomIt&& it, Compare&& cmp)

   Partial sorting algorithm that rearranges elements in :samp:`{range}` such
   that the element pointed at by :samp:`{it}` is changed to whatever element
   would occur in that position if :samp:`{range}` was sorted and all of the
   elements before this new element at :samp:`{it}` are less than or equal to
   the elements after :samp:`{it}`.

   If :samp:`{it}` is the end iterator of :samp:`{range}`, this function has no
   effect.

   :requires: :samp:`{range}` must provide RandomAccessIterators.

.. _core-algorithm-component-binary-search-operations:

.. index:: algorithm; binary search

Binary Search Operations
------------------------

.. note:: These operations are intended for sorted/partitioned ranges *only*.

.. function:: ForwardIt lower_bound (Range&& range, T const& value)
              ForwardIt lower_bound (\
                Range&& range,\
                T const& value,\
                Compare&& cmp)

   Returns an iterator pointing to the first element in :samp:`{range}` that is
   *not less than* :samp:`{value}`. The range must be partially ordered. A
   fully sorted range or a range resulting from :any:`partition` meets this
   criteria. The first version uses :cxx:`operator <` to compare elements,
   while the second uses the given function :samp:`{cmp}`.

   :requires: :samp:`{range}` must provide ForwardIterators.

.. function:: ForwardIt upper_bound (Range&& range, T const& value)
              ForwardIt upper_bound (\
                Range&& range,\
                T const& value,\
                Compare&& cmp)

   Returns an iterator pointing to the first element in :samp:`{range}` that is
   :samp:`{greater}` than value. The same ordering restructions in
   :any:`lower_bound` apply. The first version uses :cxx:`operator <`. The
   second uses the comparison function :samp:`{cmp}`.

   :requires: :samp:`{range}` must provide ForwardIterators.

.. function:: bool binary_search (Range&& range, T const& value)
              bool binary_search (Range&& range, T const& value, Compare&& cmp)

   Checks if an element equal to :samp:`{value}` resides within
   :samp:`{range}`. Requires that :samp:`{range}` be partitioned. The first
   version uses :cxx:`operator <`. The second
   uses the given function :samp:`{cmp}`.

   :requires: :samp:`{range}` must provide ForwardIterators.

.. function:: range<ForwardIt> equal_range (Range&& range, T const& value)
              range<ForwardIt> equal_range (\
                Range&& range,\
                T const& value,\
                Compare&& cmp)

   Returns a range containing all elements equivalent to :samp:`{value}` in 
   :samp:`{range}`. The first version uses :cxx:`operator <`. The second uses
   the given comparison function :samp:`{cmp}`.

   :requires: :samp:`{range}` must provide ForwardIterators and must be correctly
              partitioned.

.. _core-algorithm-component-set-operations:

.. index:: algorithm; set

Set Operations
--------------

.. function:: OutputIt merge (\
                Range1&& range1,\
                Range2&& range2,\
                OutputIt&& it)
              OutputIt merge (\
                Range1&& range1,\
                Range2&& range2,\
                OutputIt&& it,\
                Compare&& cmp)

   Merges sorted :samp:`{range1}` and sorted :samp:`{range2}` into one sorted
   range beginning at :samp:`{it}`. The first version uses :cxx:`operator <` to
   compare elements. The second uses the comparison function :samp:`{cmp}`. The
   relative order of elements is preserved. If the destination range overlaps
   either :samp:`{range1}` or :samp:`{range2}`, the resulting behavior is
   undefined. (It is ok if :samp:`{range1}` and :samp:`{range2}` are
   overlapping)

   :requires: :samp:`{range1}` and :samp:`{range2}` must provide
              InputIterators.

.. function:: void inplace_merge (Range&& range, Bidir&& it)
              void inplace_merge (Range&& range, Bidir&& it, Compare&& cmp)

   Merges two consecutive sorted ranges (:samp:`[{range}.begin(), {it})` and
   :samp:`[{it}, {range}.end())`) into one sorted range. The order of equal
   elements is preserved. The first version uses :cxx:`operator <`. The second
   version uses the comparison function :samp:`{cmp}`.

   :requires: :samp:`{range}` must provide BidirectionalIterators

.. function:: bool includes (Range1&& range1, Range2&& range2)
              bool includes (Range1&& range1, Range2&& range2, Compare&& cmp)

   Checks if every element from the sorted range :samp:`{range2}` is found
   within the sorted range :samp:`{range1}`. The first overload uses
   :cxx:`operator <`, while the second overload uses :samp:`{cmp}` as a
   comparison function.

   :returns: :cxx:`true` if every element from :samp:`{range2}` is found within
             :samp:`{range1}` or if :samp:`{range2}.empty()` is :cxx:`true`.

   :requires: :samp:`{range1}` and :samp:`{range2}` must provide
              InputIterators.

   :example:

   .. code-block:: cpp

      std::vector<char> v1 { 'a', 'b', 'c', 'f', 'h', 'z' };
      std::vector<char> v2 { 'a', 'c' };
      std::vector<char> v3 { 'g' };
      std::vector<char> v4 { 'A', 'B', 'C' };
      auto const nocase = [] (char a, char b) {
        return std::tolower(a) < std::tolower(b);
      };

      assert(includes(v1, v2));
      assert(not includes(v1, v3));
      assert(includes(v1, v4, nocase));

.. function:: OutputIt set_difference (\
                Range1&& range1,\
                Range2&& range2,\
                OutputIt&& it)
              OutputIt set_difference (\
                Range1&& range1,\
                Range2&& range2,\
                OutputIt&& it,\
                Compare&& cmp)

   Copies the elements from :samp:`{range1}` which are not found in
   :samp:`{range2}` to the range beginning at :samp:`{it}`. The first version
   uses :cxx:`operator <`. The second uses :samp:`{cmp}` as a comparison
   function.

   :requires: :samp:`{range1}` and :samp:`{range2}` must provide
              InputIterators.

.. function:: OutputIt set_intersection (\
                Range1&& range1,\
                Range2&& range2,\
                OutputIt&& it)
              OutputIt set_intersection (\
                Range1&& range1,\
                Range2&& range2,\
                OutputIt&& it,\
                Compare&& cmp)

   Constructs a sorted range beginning at :samp:`{it}` consisting of elements
   that are found in both :samp:`{range1}` and :samp:`{range2}`. The first
   version expects :samp:`{range1}` and :samp:`{range2}` to be sorted with 
   :cxx:`operator <`. The second version expects them to be sorted by
   :samp:`{cmp}`.

   :requires: :samp:`{range1}` and :samp:`{range2}` must provide
              InputIterators.

.. function:: OutputIt set_symmetric_difference (\
                Range1&& range1,\
                Range2&& range2,\
                OutputIt&& it)
              OutputIt set_symmetric_difference (\
                Range1&& range1,\
                Range2&& range2,\
                OutputIt&& it,\
                Compare&& cmp)

   Copies the symmetric difference of :samp:`{range1}` and :samp:`{range2}`
   (i.e., the elements found in either of the ranges but not both) to a range
   starting at :samp:`{it}`. The result is also sorted. The first version
   expects :samp:`{range1}` and :samp:`{range2}` to be sorted with
   :cxx:`operator <`. The second version expects them to be sorted with
   :samp:`{cmp}`.

   :requires: :samp:`{range1}` and :samp:`{range2}` must provide
              InputIterators.

.. function:: OutputIt set_union (\
                Range1&& range1,\
                Range2&& range2,\
                OutputIt&& it)
              OutputIt set_union (\
                Range1&& range1,\
                Range2&& range2,\
                OutputIt&& it,\
                Compare&& cmp)

   Constructs a sorted range starting at :samp:`{it}` consisting of all
   elements present in one or both :samp:`{range1}` and :samp:`{range2}`. The
   resulting range cannot overlap with either :samp:`{range1}` or
   :samp:`{range2}`. The first version expects both ranges to be sorted with
   :cxx:`operator <`. The second version expects them to be sorted via
   :samp:`{cmp}`.

   :requires: :samp:`{range1}` and :samp:`{range2}` must provide InputIterators.

.. _core-algorithm-component-heap-operations:

.. index:: algorithm; heap

Heap Operations
---------------

.. function:: bool is_heap (Range&& range)
              bool is_heap (Range&& range, Compare&& compare)

   Checks if the elements in :samp:`{range}` are a max heap. Uses
   :cxx:`operator <` or :samp:`{cmp}` as a comparison function.

   :require: :samp:`{range}` must provide RandomAccessIterators.

.. function:: RandomIt is_heap_until (Range&& range)
              RandomIt is_heap_until (Range&& range, Compare&& compare)

   Find the largest subrange within :samp:`{range}` which is a max heap. Uses
   :cxx:`operator <` or :samp:`{compare}` as the comparison function.

   :require: :samp:`{range}` must provide RandomAccessIterators.

.. function:: void make_heap (Range&& range)
              void make_heap (Range&& range, Compare&& compare)

   Constructs a max heap in :samp:`{range}`. Uses :cxx:`operator <` or
   :samp:`{compare}` as the comparison function.

   :requires: :samp:`{range}` must provide RandomAccessIterators.

.. function:: void push_heap (Range&& range)
              void push_heap (Range&& range, Compare&& compare)

   Inserts the element at :samp:`{range}.end() - 1` into the max heap defined
   by :samp:`[{range}.begin(), {range}.end() - 1)`. Uses `operator <` or
   :samp:`{compare}` as the comparison function.

   :requires: :samp:`{range}` must provide RandomAccessIterators.

.. function:: void pop_heap (Range&& range)
              void pop_heap (Range&& range, Compare&& compare)

   Swaps the value at :samp:`{range}.begin()` and the value in
   :samp:`{range}.end() - 1` and turns this subrange into a max heap. Uses
   :cxx:`operator <` or :samp:`{compare}` as the comparison function.

   :requires: :samp:`{range}` must provide RandomAccessIterators.

.. function:: void sort_heap (Range&& range)
              void sort_heap (Range&& range, Compare&& compare)

   Converts a max heap (:samp:`{range}`) into a sorted range in ascending
   order. The resulting range is no longer a heap. Uses :cxx:`operator <` or
   :samp:`{compare}` as the comparison function.

   :requires: :samp:`{range}` must provide RandomAccessIterators.

.. _core-algorithm-component-min-max-operations:

.. index:: algorithm; min/max

Min/Max Operations
------------------

.. function:: ForwardIt max_element (Range&& range)
              ForwardIt max_element (Range&& range, Compare&& compare)

   Finds the greatest element in :samp:`{range}`. Uses :cxx:`operator <` or 
   :samp:`{compare}` as the comparison function.

   :requires: :samp:`{range}` must provide ForwardIterators.

.. function:: ForwardIt min_element (Range&& range)
              ForwardIt min_element (Range&& range, Compare&& compare)

   Finds the smallest element in :samp:`{range}`. Uses :cxx:`operator <` or
   :samp:`{compare}` as the comparison function.

   :requires: :samp:`{range}` must provide ForwardIterators.

.. function:: std::pair<ForwardIt, ForwardIt> minmax_element (Range&& range)
              std::pair<ForwardIt, ForwardIt> minmax_element (\
                Range&& range,\
                Compare&& compare)

   Finds the greatest and smallest element in :samp:`{range}`. Uses
   :cxx:`operator <` or :samp:`{compare}` as the comparison function.

   :requires: :samp:`{range}` must provide ForwardIterators.

.. function:: bool lexicographical_compare (Range1&& range1, Range2&& range2)
              bool lexicographical_compare (\
                Range1&& range1,\
                Range2&& range2,\
                Compare&& compare)

   Checks if :samp:`{range1}` is lexicographically less than :samp:`{range2}`.
   Uses :cxx:`operator <` or :samp:`{compare}` as the comparison function.

   :requires: :samp:`{range1}` and :samp:`{range2}` must provide
              InputIterators.

.. function:: is_permutation (Range1&& range1, Range2&& range2)
              is_permutation (\
                Range1&& range1,\
                Range2&& range2,\
                BinaryPredicate&& bp)

   Returns :cxx:`true` if there exists a permutation of the elements in
   :samp:`{range}` that makes it equal to :samp:`{range2}`. The first version
   uses :cxx:`operator ==`. The second version uses the given binary predicate
   :samp:`{bp}`.


   :requires: :samp:`{range1}` and :samp:`{range2}` must provide ForwardIterators.

.. function:: bool next_permutation (Range&& range)
              bool next_permutation (Range&& range, Compare&& compare)

   Transforms :samp:`{range}` into the next permutation from the set of all
   permutations that are lexicographically ordered. The first version
   uses :cxx:`operator <`. The second version uses :samp:`{compare}`.

   :returns: :cxx:`true` if such permutation exists otherwise transforms
             :samp:`{range}` into the first permutation and returns
             :cxx:`false`.
   :requires: :samp:`{range}` must provide BidirectionalIterators.

.. function:: bool prev_permutation (Range&& range)
              bool prev_permutation (Range&& range, Compare&& compare)

   Transforms :samp:`{range}` into the previous permutation from the set of all
   permutations that are lexicographically ordered. The first version
   uses :cxx:`operator <`. The second version uses :samp:`{compare}`.

   :returns: :cxx:`true` if such permutation exists otherwise transforms
             :samp:`{range}` into the first permutation and returns
             :cxx:`false`.

   :requires: :samp:`{range}` must provide BidirectionalIterators.

   :example:

   .. code-block:: cpp

      std::string s { "cba" };
      assert(prev_permutation(s) and s == "cab");
      assert(prev_permutation(s) and s == "bca");
      assert(prev_permutation(s) and s == "bac");
      assert(prev_permutation(s) and s == "acb");
      assert(prev_permutation(s) and s == "abc");
      assert(not prev_permutation(s) and s == "cba");
