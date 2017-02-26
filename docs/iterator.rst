Iterator Component
==================

.. namespace:: core

.. index:: iterator

The iterator component provides the additional functions provided in C++14 with
respect to iterators. Specifically the free form functions that work like
:cxx:`std::begin` and :cxx:`std::end`.

The iterator component can be found in :file:`<core/{iterator}.hpp>`.

.. index:: iterator; functions

.. function:: template <class Container> \
              constexpr auto size (Container const& container) noexcept

   :returns: :samp:`{container}.size()` or :cxx:`N` if :samp:`{container}` is
             an array.

.. function:: template <class Container> \
              constexpr auto empty (Container const& container) noexcept

   :returns: :samp:`{container}.empty()` or :cxx:`false` if
             :samp:`{container}` is an array.

.. function:: template <class Container> \
              constexpr auto front (Container const& container)
              template <class Container> \
              constexpr auto front (Container& container)

   :returns: :samp:`{container}.front()` or :samp:`{container}[0]` if
             :samp:`{container}` is an array.

.. function:: template <class Container> \
              constexpr auto back (Container const& container)
              template <class Container> \
              constexpr auto back (Container&)

   :returns: :samp:`{container}.back()` or :samp:`{container}[N - 1]` if
             :samp:`{container}` is an array.

.. function:: template <class Container> \
              constexpr auto data (Container const& container) noexcept
              template <class Container> \
              constexpr auto data (Container& container) noexcept

   :returns: :samp:`{container}.data()` or :samp:`{container}` if
             :samp:`{container}` is an array.

.. function:: template <class Container> \
              cbegin (Container const& container)

   :returns: :samp:`begin({container})`

.. function:: template <class Container> \
              cend (Container const& container)

   :returns: :samp:`end({container})`

.. function:: template <class Container> \
              rbegin (Container const& container)
              template <class Container> \
              rbegin (Container& container)

   Requires that the given :samp:`{container}` have a member function named
   :cxx:`rbegin`. If no such member function exists, the function will fail to
   compile.

   :returns: :samp:`{container}.rbegin()`

.. function:: template <class Container> \
              crbegin (Container const& container)

   :returns: :samp:`rbegin({container})`

.. function:: template <class Container> \
              rend (Container const& container)
              template <class Container> \
              rend (Container& container)

   Requires that the given :samp:`{container}` have a member function named
   :cxx:`rend`. If no such member function exists, the function will fail to
   compile.

   :returns: :samp:`{container}.rend()`

.. function:: template <class Container> \
              crend (Container const& container)

   :returns: :samp:`rend({container})`

.. index:: iterator; types

.. class:: template <class T> infix_ostream_iterator

   The :any:`infix_ostream_iterator` is used as a replacement for
   :cxx:`std::ostream_iterator`, where the delimiter is not desired for the
   last element. This iterator comes in use when one might wish to print a
   list.

   :example:

   .. code-block:: cpp

      // prints [1,2,3,4,5]
      std::vector<int> numbers { 1, 2, 3, 4, 5 };
      infix_ostream_iterator iter(std::cout, ",");
      std::cout << "[";
      copy(numbers, iter);
      std::cout << "]" << std::endl;

.. class:: template <class T> number_iterator

   This is an iterator that allows one to iterate over numbers when working
   with functions found in :doc:`algorithms <algorithm>`. It is a bidirectional
   iterator. The reason for this is to avoid some functions which may assume
   a :any:`number_iterator` can represent a contiguous block of memory.

   :requires: :samp:`{T}` must satisfy :cxx:`std::is_integral`.

   .. type:: iterator_category

      :cxx:`std::bidirectional_iterator_tag`

   .. type:: difference_type

      An alias of :samp:`{T}`

   .. type:: value_type

      An alias of :samp:`{T}`

   .. type:: reference

      An alias of :samp:`{value_type}&`

   .. type:: pointer

      An alias of :samp:`{value_type}*`

   .. function:: number_iterator (value_type value, value_type step=1) noexcept

      Constructs a :any:`number_iterator` starting at :samp:`{value}`.
      :samp:`{step}` is how much the :any:`number_iterator` will be incremented
      when :any:`operator ++` is called.

   .. function:: number_iterator (number_iterator const&) noexcept = default
                 number_iterator () noexcept = default
                 ~number_iterator () noexcept = default
                 number_iterator& operator = (\
                   number_iterator const&) noexcept = default

   .. function:: void swap (number_iterator& that) noexcept

      Swaps the value and step used by :any:`number_iterator` with the value
      and step used by :samp:`{that}`.

   .. function:: reference operator * () noexcept

      :returns: :any:`reference` to the current value the
                :any:`number_iterator` holds.

   .. function:: number_iterator& operator ++ () noexcept
                 number_iterator operator ++ (int) const noexcept

      Increments a :any:`number_iterator` by its step.

   .. function:: number_iterator& operator -- () noexcept
                 number_iterator operator -- () const noexcept

      Decrements the :any:`number_iterator` by its step

   .. function:: bool operator == (number_iterator const& that) const noexcept
                 bool operator != (number_iterator const& that) const noexcept

      Compares :samp:`{that}` with :any:`number_iterator`. A
      :any:`number_iterator` is equal *only* if both its value and its step
      are equal.

.. function:: void swap (number_iterator<T>& l, nunber_iterator<T>& r) noexcept

              Swaps the value and step amount of :samp:`{l}` and :samp:`{r}` by
              calling :samp:`{l}.swap({r})`.

.. function:: template <class T> \
              auto make_number_iterator (T value, T step) noexcept
              template <class T> \
              auto make_number_iterator (T value) noexcept
              
              Creates a :any:`number_iterator` via type deduction.
