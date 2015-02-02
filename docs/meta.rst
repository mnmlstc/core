.. _core-meta-component:

Metatemplate Component
======================

.. default-domain:: cpp

.. |pack| replace::
   :class:`pack\<Args...>`

The metatemplate component contains several utilities that will aid anyone
who has to do metatemplate programming. A majority of MNMLSTC Core's internals
use this component, and others may receive some use from it as a result.

This module works primarily on parameter packs. Several metafunctions are
implementations of :ref:`algorithms <core-algorithm-component>`.
A parameter pack wrapper type to use these metafunctions on is provided.
Additionally, each metafunction has an alias available, in the same way
that each :ref:`type trait <core-type-traits-component>` has an ``_t`` alias
available. e.g., :class:`transform\<T, F\<U>>` has an alias
:type:`transform_t\<T, F\<U>>`.

The metatemplate component can be found in the ``<core/meta.hpp>`` header.
Additionally, all types declared in this component are located inside the
``core::meta`` namespace.

.. type:: size<N>

   .. versionadded:: 1.2

   An alias for ``std::integral_constant<std::size_t, N>``

.. type:: boolean<B>

   .. versionadded:: 1.2

   An alias for ``std::integral_constant<bool, B>``

.. type:: integer<I>

   .. versionadded:: 1.2

   An alias for ``std::integral_constant<int, I>``

.. type:: all<Args...>

   .. versionadded:: 1.2

   :complexity: O(n)

   Takes a parameter pack *Args...*, where each type in *Args...* is a type
   trait property that contains a boolean member *value*. If all *Args...*
   *value* members are true, the result is equivalent to ``std::true_type``.
   Otherwise, it will be equivalent to ``std::false_type``. The arguments
   are evaluated lazily, which allows for short circuit evaluation of
   each type given.

   :example:

   .. code-block:: cpp

      using result = meta::all<
        is_void<T>,
        is_same<T, U>,
        is_base_of<W, V>
      >;

      static_assert(result::value, "");

.. type:: any<Args...>

   .. versionadded:: 1.2

   :complexity: O(n)

   Takes a parameter pack *Args...*, where each type in *Args...* is a type
   trait property that contains a boolean member *value*. If all *Args...*
   *value* members are true, the result is equivalent to ``std::true_type``.
   Otherwise, the result will be equivalent to ``std::false_type``. The
   arguments are evaluated lazily, which allows for short circuit evaluation of
   each type given.

   :example:

   .. code-block:: cpp

      using result = meta::any<
        is_same<U, T>,
        is_void<void>
        is_void<nullptr_t>
      >;

.. type:: none<Args...>

   .. versionadded:: 1.2

   :complexity: O(n)

   Takes a parameter pack *Args...*, where each type in *Args...* is a type
   trait property that contains a boolean member named *value*. This
   metafunction is the inverse of :type:`all\<Args>`. The arguments are
   evaluated lazily, which allows for short circuit evaluation of each type
   given. It can also be used as a negate metafunction.

   :example:

   .. code-block:: cpp

      using result = meta::none<is_void<T>>;
        

.. class:: pack<Args...>

   .. versionadded:: 1.2

   This type is used as a wrapper for parameter packs, and offers a few static
   member functions for ease of use.

   .. function:: static constexpr size_t size () noexcept

      :returns: ``sizeof...(Args)``

   .. function:: static constexpr bool empty () noexcept

      :returns: ``sizeof...(Args) == 0``

   .. type:: front

      If :func:`pack\<Args...>::empty` is false, this is an alias for the first
      type in *Args...*. Otherwise, this alias does not exist.

   .. type:: back

      If :func:`pack\<Args...>::empty` is false, this is an alias for the last
      type in *Args...*. Otherwise, this alias does not exist.


.. class:: transform<T, F<U>>

   .. versionadded:: 1.2

   :complexity: O(1)

   Takes a |pack| *T*, and a template class *F*. When the underlying dependent
   alias :type:`transform\<T, F\<U>>::type` is instantiated, *F* will be
   applied to each element in *T*, to create a new |pack|.

   *F* is a simple metafunction. It does not require a type alias member.

   :example:

   .. code-block:: cpp
      
      template <class T> struct void_to_int : identity<T> { };
      template <> struct void_to_int<void> : identity<int> { };
      template <class T> using void_to_int_t = typename void_to_int<T>::type;
      using out = meta::pack<int, int, double, int>;
      using in = meta::pack<void, int, double, void>;
      using result = meta::transform_t<in, void_to_int_t>;
      static_assert(is_same<out, result>::value, "");

.. class:: count_if<T, F<U>>

   .. versionadded:: 1.2

   :complexity: O(1)

   Takes a |pack| *T*, and a predicate template class *F*. The result of this
   operation will be of type :type:`size\<N>`, with its *value* member
   equal to the number of elements in *T* for which the predicate *F* returned
   true. There is no ``_t`` alias provided for this metafunction.

   :example:

   .. code-block:: cpp
      
      using in = meta::pack<int, void, double, void, std::string, void>;
      using result = meta::count_if<in, is_void>;
      static_assert(result::value == 3, "");

.. class:: find_if<T, F<U>>

   .. versionadded:: 1.2

   :complexity: O(n)

   Takes a |pack| *T*, and a template class predicate *F*. *F* must contain a
   static boolean member named *value* when given a type *U*. When the
   underlying dependent alias :type:`find_if\<T, F\<U>>::type` is instantiated,
   *F* will be applied to each member in *T* sequentially. If any element will
   result in *F*'s *value* returning true, a |pack| starting from that *U*
   until the end of the original pack. If no element in the parameter pack
   satisfies the predicate *F* then an empty |pack| is returned.

   :example:

   .. code-block:: cpp

      using in = meta::pack<int, std::string, int, double, void, double>;
      using out = meta::pack<void, double>;
      using result = meta::find_if_t<in, std::is_void>;
      static_assert(is_same<out, result>::value, "");

.. class:: filter<T, F<U>>

   .. versionadded:: 1.2

   :complexity: O(1)

   Takes a |pack| *T*, and a predicate template class *F*. Any elements within
   *T* that do not satisfy the predicate *F*, will be removed from the
   resulting |pack|.

   While this metafunction may *appear* to require sequentially require
   evaluation for each element in *T*, it occurs all at once, giving us a
   decent compile time operation complexity.

   :example:

   .. code-block:: cpp

      template <class T> struct is_not_void : std::true_type { };
      template <> struct is_not_void<void> : std::false_type { };
      using in = meta::pack<int, void, std::string, void, double, void>
      using out = meta::pack<int, std::string, double>;
      using result = meta::filter<in, is_not_void>

.. class:: element<N, T>

   .. versionadded:: 1.2

   :complexity: O(?)

   Takes a ``size_t`` *N* and a |pack| *T*. The result of this metafunction
   will be equal to the element that resides at index *N* in *T*.

   :example:

   .. code-block:: cpp

      using in = meta::pack<void, std::string, int, double>;
      using result = meta::element_t<0, in>;
      static_assert(is_void<result>::value, "");

.. class:: push_front<T, Args...>

   .. versionadded:: 1.2

   :complexity: O(1)

   Takes a |pack| *T* and a parameter pack *Args...*. This metafunction will
   return a |pack| with the elements *Args...* followed by the elements of *T*.

   :example:
  
   .. code-block:: cpp

      using in = meta::pack<void, double>;
      using out = meta::pack<int, double, void, double>;
      using result = meta::push_front_t<in, int, double>;
      static_assert(is_same<out, result>::value, "");

.. class:: push_back<T, Args...>

   .. versionadded:: 1.2

   :complexity: O(1)

   Takes a |pack| *T* and a parameter pack *Args...*. This metafunction will
   return a |pack| with the elements of *T* followed by *Args...*.

   :example:

   .. code-block:: cpp

      using in = meta::pack<void, double>;
      using out = meta::pack<void, double, void int>;
      using result = meta::push_back_t<in, void, int>;
      static_assert(is_same<out, result>::value, "");

.. class:: index<U, T>

   .. versionadded:: 1.2

   :complexity: O(n)

   Takes a |pack| *T*, and a type *U*. This metafunction will be equal to
   the index of the first instance of *U* found in *T* as a :class:`size\<N>`.
   If *T* does not contain any elements of *U*, the maximum value for a
   :class:`size\<N>` is returned instead (i.e.,
   ``std::numeric_limits<std::size_t>::max()``). There is not ``_t`` alias
   provided for this metafunction.

   :example:

   .. code-block:: cpp

      using in = meta::pack<void, double, std::string, std::vector<int>>;
      using result = meta::index<std::string, in>;
      static_assert(result::value == 2, "");

.. class:: count<U, T>

   .. versionadded:: 1.2

   :complexity: O(n)

   Takes a |pack| *T*, and a type *U*. This metafunction will be equal to the
   number of elements in *T* that are the same as *U*. The result is a
   :class:`size\<N>`.

   :example:

   .. code-block:: cpp

      using in = meta::pack<double, double, int, std::string>;
      using result = meta::count<double, in>;
      static_assert(result::value == 2, "");

.. class:: find<U, T>

   .. versionadded:: 1.2

   :complexity: O(n)

   Takes a |pack| *T* and some type *U*. Upon finding the first instance of *U*
   in *T*, a |pack| starting at *U* until the end of *T* is returned. Otherwise
   an empty |pack| is returned.

   This metafunction uses :class:`find_if\<T, F\<U>>` under the hood, with a
   bound version of ``std::is_same`` (so that ``std::is_same`` can take one
   parameter)

   :example:

   .. code-block:: cpp

      using in = meta::pack<int, std::string, int, double>;
      using out = meta::pack<std::string, int, double>;
      using result = meta::find_t<std::string, in>;
      static_assert(is_same<out, result>::value, "");

.. class:: merge<Args...>

   .. versionadded:: 1.2

   :complexity: O(1)

   Takes a variable number of |pack| in *Args...*. This metafunction will
   return a new |pack| with each element in each |pack| in *Args...*.

   :example:

   .. code-block:: cpp

      using in1 = meta::pack<std::string, double, int, void>;
      using in2 = meta::pack<void, double, int>;
      using out = meta::pack<std::string, double, int, void, void, double, int>;
      using result = meta::merge<in1, in2>;
      static_assert(is_same<result, out>::value, "");


.. class:: index_sequence_from<T>

   .. versionadded:: 1.2

   :complexity: O(1)

   Takes a |pack| *T*. This will generate a :class:`index_sequence\<Is...>`
   such that each element in *T* will have a corresponding *I* in the
   returned sequence.

   :example:

   .. code-block:: cpp

      using in = meta::pack<void, double, void>;
      using out = make_index_sequence<3>;
      using result = index_sequence_from<in>;
      static_assert(is_same<out, result>::value, "");

.. class:: pop_front<T>

   .. versionadded:: 1.2

   :complexity: O(1)

   Takes a |pack| *T*. This metafunction will return a new |pack| with the
   first element in *T* removed. This metafunction will result in an error
   if *T* has no elements.

   :example:

   .. code-block:: cpp

      using in = meta::pack<int, void, double>;
      using out = meta::pack<void, double>;
      using result = meta::pop_front_t<in>;
      static_assert(is_same<out, result>::value, "");

.. class:: pop_back<T>

   .. versionadded:: 1.2

   :complexity: O(1)

   Takes a |pack| *T*. This metafunction will return a new |pack| with the
   last element in *T* removed. This metafunction will result in an error
   if *T* has no elements.

   :example:

   .. code-block:: cpp

      using in = meta::pack<int, void, double>;
      using out = meta::pack<int, void>;
      using result = meta::pop_back_t<in>;
      static_assert(is_same<out, result>::value, "");

.. class:: reverse<T>

   .. versionadded:: 1.2

   :complexity: O(1)

   Takes a |pack| *T*. This metafunction will return a new |pack| with each
   element in *T* in its reversed position, such that *T*'s first element will
   be its last, and vice versa.

   :example:

   .. code-block:: cpp

      using in = meta::pack<int, void, double>;
      using out = meta::pack<double, void, int>;
      using result = meta::reverse_t<in>;
      static_assert(is_same<out, result>::value, "");

.. class:: to_pack<T>

   .. versionadded:: 1.2

   :complexity: O(1)

   Takes any variadic template class *T*. This metafunction will then return
   a |pack| whose elements correspond to those in *T*.

   :example:

   .. code-block:: cpp

      using in = std::tuple<double, std::string, int>;
      using out = meta::pack<double, std::string, int>;
      using result = meta::to_pack_t<in>;
      static_assert(is_same<out, result>::value, "");

.. class:: from_pack<T, U>

   .. versionadded:: 1.2

   :complexity: O(1)

   Takes a |pack| *T*, and any variadic tempate class *U*. This metafunction
   will return an instantiation of *U* whose elements correspond to those
   in *T*. This metafunction acts as the inverse of :class:`to_pack\<T>`.

   :example:

   .. code-block:: cpp

      using in = meta::pack<std::string, double, int>;
      using out = std::tuple<std::string, double, int>;
      using result = meta::from_pack_t<in, std::tuple>;
      static_assert(is_same<out, result>::value, "");
