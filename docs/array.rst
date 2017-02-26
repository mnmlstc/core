Array Component
===============

.. namespace:: core

.. index:: array

The array component currently provides two helper functions for working with
:cxx:`std::array` in the C++ standard library. Specifically, this component
provides an implementation of :wg21:`N4315`. This component can be found in the
:file:`<core/{array}.hpp>` header.

.. index:: array; functions

.. function:: constexpr std::array<T, N> make_array (Args&&... args)

   Given a parameter pack :samp:`{args}`, return a :cxx:`std::array` with the
   :samp:`common_type_t<{Args}...>`. Additionally, a type can be given
   explicitly. This function is *does not* participate in overload resolution
   if any of the elements in :samp:`{args}` are a
   :cxx:`std::reference_wrapper`.

   :example:

    .. code-block:: cpp

       auto a1 = make_array(1, 2, 3, 4);
       using type1 = decltype(a1)::value_type;
       static_assert(std::is_same<type1, int>::value, "");

       auto a2 = make_array<long>(1, 2L, 3L, 4);
       using type2 = decltype(a2)::value_type;
       static_assert(std::is_same<type2, long>::value, "");

.. function:: constexpr std::array<T, N> to_array (T (&array)[N])

   Given a C array of type :samp:`{T}` with a defined length of :samp:`{N}`,
   creates a :cxx:`std::array` with the same length and size.

   This function will copy initialize each :samp:`{T}`
