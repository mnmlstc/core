Range Component
===============

.. namespace:: core

Ranges are a common concept in the C++ standard, and in the case of all the
algorithms available, one usually has to supply a start and end to a range as
adjacent arguments. This can be tiresome, and in the common case, unnecessary.

The :any:`range` component works as a high level view. Rather than holding onto
the container, it simply holds the start and end iterators to represent a
range. The :any:`range` component implements the interface and behavior
discussed within :wg21:`N3350`. Some decisions pertaining to open questions
were made, and these are discussed below.

The range component resides in the :file:`<core/{range}.hpp>`.

Writing Custom Algorithms
-------------------------

One of the nice features of this component is the ease with which one can write
new and custom algorithms in the style of the C++ standard library. For
instance, if we wanted to have an *early return* :any:`copy` function, we
could implement it in the following way:

.. code-block:: cpp

   template <class R, class OutputIt, class T>
   OutputIt copy_until (R&& r, OutputIt it, T const& value) {
     auto range = make_range(::std::forward<R>(r));
     static constexpr auto is_input = decltype(range)::is_input;
     static_assert(is_input, "copy_until requires InputIterators");
     while (not range.empty()) {
       if (range.front() == value) { break; }
       *it++ = range.front();
       range.pop_front();
     }
     return it;
   }

This kind of function looks quite different from the typical way in which
algorithms are written in the standard library. The only thing we need to worry
about is creating our range and making sure that we consume a forwarding
reference to a range-like object from which we can create our :any:`range`.


Component Interface
-------------------

.. class:: template <class R> is_range

   Type trait for determining if a type can return a value when :cxx:`begin`
   and :cxx:`end` are called on it.

.. class:: template <class T> range


   Represents an open-ended range of :cxx:`[begin, end)`. The category of range
   depends on the category of its :any:`iterator`. All the type aliases
   within the :any:`range` depend on the use of
   :samp:`std::iterator_traits<{T}>`.

   The type :samp:`{T}` is actually an Iterator type.

   .. type:: iterator_category

      Represents :samp:`std::iterator_traits<{T}>::iterator_category`

   .. type:: difference_type

      Represents :samp:`std::iterator_traits<{T}>::difference_type`

   .. type:: value_type

      Represents :samp:`std::iterator_traits<{T}>::value_type`

   .. type:: reference

      Represents :samp:`std::iterator_traits<{T}>::reference`

   .. type:: pointer

      Represents :samp:`std::iterator_traits<{T}>::pointer`

   .. type:: iterator

      Represents :samp:`{T}`.

   .. function:: range (std::pair<iterator, iterator> pair) noexcept

      Constructs a :any:`range` with the first and second members of the pair
      to be the begin and end of the :any:`range` respectively.

   .. function:: range (iterator begin, iterator end) noexcept

      Constructs a :any:`range` with the given iterators.

   .. function:: range (range const& that)

      Constructs a :any:`range` with a copy of the iterators stored in
      :samp:`{that}`.

   .. function:: range (range&& that) noexcept

      Constructs a :any:`range` by moving the iterators stored in
      :samp:`{that}`.

   .. function:: range ()

      Constructs a :any:`range` by default constructing both its begin and end
      iterators. The resulting range will be empty.

      :postcondition: :samp:`{begin}() == {end}()`

   .. function:: range& operator = (range const& that)
                 range& operator = (range&& that)

      Assigns the contents of the :samp:`{that}` to the :any:`range`.

   .. function:: reference operator[](difference_type idx) const

      :requires: :any:`iterator_category` be :cxx:`random_access_iterator_tag`.

   .. function:: iterator begin () const

      :returns: beginning of the range

   .. function:: iterator end () const

      :returns: end of the range.

   .. function:: reference front () const

      :returns: the value returned by dereferencing :any:`begin`

   .. function:: reference back () const

      :requires: :any:`iterator_category` be :cxx:`bidirectional_iterator_tag`.
      :returns: the value returned by dereferencing the iterator before
                :any:`end`
   .. function:: bool empty () const

      :returns: :any:`begin` == :any:`end`

   .. function:: difference_type size () const

      Will return the number of elements between :any:`begin` and :any:`end`.

      :requires: :any:`iterator_category` be :cxx:`forward_iterator_tag`
      :returns: :samp:`std::distance({begin}(), {end}())`

   .. function:: range slice (difference_type start, difference_type stop) const

      Slicing a :any:`range` has the most complex behavior out of all the
      :any:`range` member functions. This is due to the behavior mimicking the
      slice behavior exhibited by the python language's slicing syntax.

      If :samp:`start` is *negative*, the begin marker is
      :samp:`{end}() - {start}`

      If :samp:`stop` is *negative*, the end marker is
      :samp:`{end}() - {stop}`.

      If :samp:`start` is *positive*, the begin marker is
      :samp:`{begin}() + {start}`.

      If :samp:`stop` is *positive*, the end marker is
      :samp:`{begin}() + {stop}`.

      If :samp:`{start}` and :samp:`{stop}` are *positive*, and :samp:`{stop}`
      is *less than or equal* to :samp:`{start}`, an empty :any:`range` is
      returned.

      If :samp:`{start}` and :samp:`{stop}` are negative and :samp:`{stop}` is
      *less than or equal* to :samp:`{start}`, an empty :any:`range` is
      returned.

      If :samp:`{start}` is positive and :samp:`{stop}` is negative and
      :samp:`abs({stop}) + {start}`
      is *greater or equal* to :any:`size`, an empty :any:`range` is returned.

      If :samp:`{start}` is negative and :samp:`{stop}` is positive and
      :samp:`{size}() + {start}` is *greater or equal* to :samp:`{stop}`, an
      empty range is returned.

      These first two conditions can be computed cheaply, while the third and
      fourth are a tad more expensive. However they *are* required in all
      computations, no matter the :any:`iterator_category`. :any:`slice` does
      not compute :any:`size` until after checking the first two conditions.

      :requires: :any:`iterator_category` be :cxx:`forward_iterator_tag`.

   .. function:: range slice (difference_type start) const

      :requires: :any:`iterator_category` be :cxx:`forward_iterator_tag``.
      :returns: An open ended range of :samp:`[{begin}() + {start}, {end}())`.

   .. function:: std::pair<range, range> split (difference_type idx) const

      :requires: :any:`iterator_category` be :cxx:`forward_iterator_tag`.

   .. function:: void pop_front (difference_type n)
                 void pop_front ()

      Moves the start of the range forward by :samp:`{n}`, via
      :cxx:`std::advance`. The overload which takes no arguments moves the
      range forward by 1.

      No guarantees are made that the end of the range will not go beyond the
      end of the range.

   .. function:: void pop_back (difference_type n)
                 void pop_back ()

      Moves the end of the range backward by :samp:`{n}`, via
      :cxx:`std::advance`. The overload which takes no arguments moves the
      range backward by 1.

      No guarantees are made that the end of the range will not stop at the
      beginning of the range.

      :requires: :any:`iterator_category` be `bidirectional_iterator_tag`.

   .. function:: void pop_front_upto (difference_type n)

      Moves the start of the range by :samp:`{n}` elements. A negative argument
      causes no change.

   .. function:: void pop_back_upto (difference_type n)

      Moves the end of the range backwards by :samp:`{n}` elements. A negative
      argument causes no change.

      :requires: :any:`iterator_category` be ``bidirectional_iterator_tag``.

   .. function:: void swap (range& that) noexcept

      Swaps the begin and end of :samp:`{that}` with the any:`range`.

.. function:: range<number_iterator<T>> make_number_range(\
                T start, T stop, T step) noexcept
              range<number_iterator<T>> make_number_range(T start, T stop)

   .. versionadded:: 1.2

   Creates a :any:`range` with :any:`number_iterator` as its :any:`iterator`.

.. function:: range<T> make_range (T begin, T end)

   Creates a :any:`range` from the iterators :samp:`{begin}` and :samp:`{end}`.

.. function:: range<T> make_range(Range&&)

   Constructs a :any:`range` from the given type by calling :cxx:`begin` and
   :cxx:`end`.

.. function:: range<std::istream_iterator<T, CharT, Traits>> make_range \
              (std::basic_istream<CharT, Traits>& stream)

   Constructs a :any:`range` for iterating an :cxx:`std::istream`.


   :example:

    .. code-block:: cpp

       auto istream_range = make_range<double>(stream);

.. function:: range<std::istreambuf_iterator<CharT, Traits>> make_range \
              (std::basic_streambuf<CharT, Traits>* buffer)

   Constructs a :any:`range` for iterating a :cxx:`std::istreambuf`.

   :example:

    .. code-block:: cpp

       auto streambuf_range = make_range(stream.rdbuf());

.. function:: void swap (range<Iterator>& lhs, range<Iterator>& rhs)

   :noexcept: :samp:`{lhs}.swap({rhs})`

Answers to Open Questions
-------------------------

There are several questions raised in :wg21:`N3350`. The decisions related to
these are discussed below.

make_range
^^^^^^^^^^

The author of :wg21:`N3350` mentions that they would like to be able to pass a single
argument to a :any:`make_range` that is just the beginning of a range, where
the end of the range is a default constructed iterator. This would make it
helpful with iterators such as :cxx:`istream_iterator`. The author is concerned
that this will make the use of :any:`make_range` potentially confusing.

It was decided that the author's concerns are valid, however the ability to
create an :cxx:`istream_iterator` range (and :cxx:`istreambuf_iterator` range)
is a desirable feature. An overload for :any:`make_range` is provided to take
an :cxx:`istream` and :cxx:`istreambuf` to create a proper range.

Inherit from std::pair<Iterator, Iterator>
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The author of :wg21:`N3350` mentions inheriting from
:cxx:`std::pair<Iterator, Iterator>`. Rather than rely on inheritance to
represent a range, it was decided to allow a :any:`range` to be constructed
implicitly with a :cxx:`std::pair`.

Remove member functions and replace as free algorithms
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The author of :wg21:`N3350` mentions that it might be worth placing some member
functions such as :cxx:`pop_*` and :any:`slice` as non member
functions, at the cost of some Iterator copying.

It was decided that this was unnecessary, and the member functions will stay.

Range Category
^^^^^^^^^^^^^^

The author of :wg21:`N3350` proposes defining range categories.

It was decided that these add nothing of use.

Split takes arbitrary number of indices
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The author of :wg21:`N3350` mentions taking an arbitrary number of indices and
returning an N + 1 element :cxx:`tuple<>`. The author mentions that this would
be tricky with negative indices and bidirectional iterators.

It was decided that this is an unnecessary complication of the range
component's internals.
