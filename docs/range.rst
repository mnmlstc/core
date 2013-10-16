Range Component
===============

.. default-domain:: cpp

Ranges are a common concept in the C++ standard, and in the case of all the
algorithms available, one usually has to supply a start and end to a range as
adjacent arguments. This can be tiresome, and in the common case, unnecessary.

The :class:`range\<T>` component works as a high level view. Rather than
holding onto the container, it simply holds the start and end iterators to
represent a range. The :class:`range<T>` component implements the interface and
behavior discussed within N3350_. Some decisions pertaining to open questions
were made, and these are discussed below.

.. namespace:: cpp

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
:class:`range\<T>` to be constructed implicitly with a std::pair.

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

It was decided that this is unnecessary, and if it is truly desired by a user
they can write their own range component.

.. _N3350: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3350.html
