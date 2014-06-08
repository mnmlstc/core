.. _core-range-component:

Range Component
===============

.. default-domain:: cpp

.. |range| replace:: :class:`range <range\<T>>`

Ranges are a common concept in the C++ standard, and in the case of all the
algorithms available, one usually has to supply a start and end to a range as
adjacent arguments. This can be tiresome, and in the common case, unnecessary.

The |range| component works as a high level view. Rather than holding onto the
container, it simply holds the start and end iterators to represent a range.
The |range| component implements the interface and behavior discussed within
N3350_. Some decisions pertaining to open questions were made, and these are
discussed below.

The range component resides in the ``<core/range.hpp>``.

.. namespace:: cpp

.. class:: is_range<R>

   Type trait for determining if a type can return a value when ``std::begin``
   and ``std::end`` are called on it.

.. class:: range<T>


   Represents an open-ended range of [begin, end). The category of range
   depends on the category of its :type:`iterator`. All the type aliases
   within the |range| depend on the use of ``std::iterator_traits<T>``.

   The type *T* is actually an Iterator type.

   .. type:: iterator_category

      Represents ``std::iterator_traits<T>::iterator_category``

   .. type:: difference_type

      Represents ``std::iterator_traits<T>::difference_type``

   .. type:: value_type

      Represents ``std::iterator_traits<T>::value_type``

   .. type:: reference

      Represents ``std::iterator_traits<T>::reference``

   .. type:: pointer

      Represents ``std::iterator_traits<T>::pointer``

   .. type:: iterator

      Represents *T*.

   .. function:: range (std::pair<iterator, iterator> pair) noexcept

      Constructs a |range| with the first and second members of the pair to be
      the begin and end of the |range| respectively.

   .. function:: range (iterator begin, iterator end) noexcept

      Constructs a |range| with the given iterators.

   .. function:: range (range const& that)

      Constructs a |range| with a copy of the iterators stored in *that*.

   .. function:: range (range&& that) noexcept

      Constructs a |range| by moving the iterators stored in *that*.

   .. function:: range ()

      Constructs a |range| by default constructing both its begin and end
      iterators. The resulting range will be empty.

      :postcondition: ``begin() == end()``

   .. function:: range& operator = (range const&)
                 range& operator = (range&&)

      Assigns the contents of the incoming |range| to ``*this``.

   .. function:: reference operator[](difference_type idx) const

      :requires: :type:`iterator_category` be ``random_access_iterator_tag``.

   .. function:: iterator begin () const

      :returns: beginning of the range

   .. function:: iterator end () const

      :returns: end of the range.

   .. function:: reference front () const

      :returns: the value returned by dereferencing :func:`begin`

   .. function:: reference back () const

      :requires: :type:`iterator_category` be ``bidirectional_iterator_tag``.
      :returns: the value returned by dereferencing the iterator before
                :func:`end`
   .. function:: bool empty () const

      :returns: :func:`begin` == :func:`end`

   .. function:: difference_type size () const

      Will return the number of elements between :func:`begin` and :func:`end`.

      :requires: :type:`iterator_category` be ``forward_iterator_tag``
      :returns: ``std::distance(begin(), end())``

   .. function:: range slice (difference_type start, difference_type stop) const

      Slicing a |range| has the most complex behavior out of all the |range|
      member functions. This is due to the behavior mimicking the slice
      behavior exhibited by the python language's slicing syntax.

      If *start* is negative, the begin marker is :func:`end` - *start*.
      If *stop* is negative, the end marker is :func:`end` - *stop*.
      If *start* is positive, the begin marker is :func:`begin` + *start*.
      If *stop* is positive, the end marker is :func:`begin` + *stop*.

      If *start* and *stop* are positive, and *stop* is less than or equal to
      *start*, an empty |range| is returned.

      If *start* and *stop* are negative and *stop* is less than or equal to
      *start*, an empty |range| is returned.

      If *start* is positive and *stop* is negative and ``abs(stop)`` + *start*
      is greater or equal to :func:`size`, an empty |range| is returned.

      If *start* is negative and *stop* is positive and :func:`size` + *start*
      is greater or equal to *stop*, an empty range is returned.

      These first two conditions can be computed cheaply, while the third and
      fourth are a tad more expensive. However they *are* required in all
      computations, no matter the :type:`iterator_category`. :func:`slice` does
      not compute :func:`size` until after checking the first two conditions.

      Some optimizations are taken to insure that finding the begin and end
      iterators is at most an O(N) operation, rather than O(2N), as it *could*
      be in some cases.

      :requires: :type:`iterator_category` be ``forward_iterator_tag``.

   .. function:: range slice (difference_type start) const

      :requires: :type:`iterator_category` be ``forward_iterator_tag``.
      :returns: An open ended range of [:func:`begin` + *start*, :func:`end`).

   .. function:: std::pair<range, range> split (difference_type idx) const

      :requires: :type:`iterator_category` be ``forward_iterator_tag``.

   .. function:: void pop_front (difference_type n)
                 void pop_front ()

      Moves the start of the range 'forward' by *n*, via ``std::advance``. The
      overload which takes no arguments moves the range forward by 1.

   .. function:: void pop_back (difference_type n)
                 void pop_back ()

      :requires: :type:`iterator_category` be ``bidirectional_iterator_tag``.

   .. function:: void pop_front_upto (difference_type n)

      Moves the start of the range by *n* elements. A negative argument causes
      no change.

   .. function:: void pop_back_upto (difference_type n)

      Moves the end of the range backwards by *n* elements. A negative argument
      causes no change.

      :requires: :type:`iterator_category` be ``bidirectional_iterator_tag``.

   .. function:: void swap (range& that) noexcept

      Swaps the begin and end of ``*this``, with *that*.

.. function:: range<T> make_range (T begin, T end)

   Creates a |range| from the iterators *begin* and *end*.

.. function:: range<T> make_range(Range&&)

   Constructs a |range| from the given type by calling ``std::begin`` and
   ``std::end``.

.. function:: range<std::istream_iterator<T, CharT, Traits>> make_range \
              (std::basic_istream<CharT, Traits>& stream)

   Constructs a |range| for iterating an istream. An example of usage is::

      auto istream_range = make_range<double>(stream);

.. function:: range<std::istreambuf_iterator<CharT, Traits>> make_range \
              (std::basic_streambuf<CharT, Traits>* buffer)

   Constructs a |range| for iterating a streambuf. An example of usage is::

      auto streambuf_range = make_range(stream.rdbuf());

Specialization
--------------

.. namespace:: std

.. function:: void swap (range<Iterator>& lhs, range<Iterator>& rhs)

   :noexcept: ``lhs.swap(rhs)``

Answers to Open Questions
-------------------------

There are several questions raised in N3350_. The decisions related to
these are discussed below.

make_range
^^^^^^^^^^

The author of N3350_ mentions that they would like to be able to pass a single
argument to a :func:`make_range` that is just the beginning of a range, where
the end of the range is a default constructed iterator. This would make it
helpful with iterators such as ``istream_iterator``. The author is concerned
that this will make the use of :func:`make_range` potentially confusing.

It was decided that the author's concerns are valid, however the ability to
create an ``istream_iterator`` range (and ``istreambuf_iterator`` range) is
a desirable feature. An overload for make_range is provided to take an
``istream`` and ``istreambuf`` to create a proper range.

Inherit from std::pair<Iterator, Iterator>
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The author of N3350_ mentions inheriting from ``std::pair<Iterator, Iterator>``.
Rather than rely on inheritance to represent a range, it was decided to allow a
|range| to be constructed implicitly with a std::pair.

Remove member functions and replace as free algorithms
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The author of N3350_ mentions that it might be worth placing some member
functions such as ``pop_*`` and :func:`range\<T>::slice` as non member
functions, at the cost of some Iterator copying.

It was decided that this was unnecessary, and the member functions will stay.

Range Category
^^^^^^^^^^^^^^

The author of N3350_ proposes defining range categories.

It was decided that these add nothing of use.

Split takes arbitrary number of indices
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The author of N3350_ mentions taking an arbitrary number o indices and
returning an N + 1 element ``tuple<>``. The author mentions that this would be
tricky with negative indices and bidirectional iterators.

It was decided that this is an unnecessary complication of the range
component's internals.

.. _N3350: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3350.html
