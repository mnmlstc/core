Metatemplate Component
======================

.. namespace:: core::meta

The metatemplate component contains several utilities that will aid anyone
who has to do metatemplate programming. A majority of MNMLSTC Core's internals
use this component, and others may receive some use from it as a result.

This module works primarily on parameter packs. Several metafunctions are
mimic names and behavior as found in :doc:`algorithm`. A parameter pack
wrapper type to use these metafunctions on is provided. Additionally, each
metafunction has an alias available, in the same way that each
:doc:`type-traits` has an ``_t`` alias available. e.g.,
:any:`transform` has an alias :any:`transform_t`.

The metatemplate component can be found in the :file:`<core/{meta}.hpp>` header.
Additionally, all types declared in this component are located inside the
:cxx:`core::meta` namespace.

.. type:: template <size_t N> size

   .. versionadded:: 1.2

   An alias for :samp:`std::integral_constant<std::size_t, {N}>`

.. type:: template <bool B> boolean

   .. versionadded:: 1.2

   An alias for :samp:`std::integral_constant<bool, {B}>`

.. type:: template <int I> integer

   .. versionadded:: 1.2

   An alias for :samp:`std::integral_constant<int, {I}>`

.. class:: template <class... Args> all

   .. versionadded:: 1.2

   :complexity: O(n)

   Takes a parameter pack :samp:`{Args}...`, where each type in
   :samp:`{Args}...` is a type trait property that contains a boolean member
   :samp:`{value}. If all :samp:`{Args}...` :samp:`{value} members are true,
   the result is equivalent to :cxx:`std::true_type`. Otherwise, it will be
   equivalent to :cxx:`std::false_type`. The arguments are evaluated lazily,
   which allows for short circuit evaluation of each type given.

   :example:

    .. code-block:: cpp

       using result = meta::all<
         is_void<T>,
         is_same<T, U>,
         is_base_of<W, V>
       >;

       static_assert(result::value, "");

.. class:: template <class... Args> any

   .. versionadded:: 1.2

   :complexity: O(n)

   Takes a parameter pack :samp:`{Args}...`, where each type in
   :samp:`{Args}...` is a type trait property that contains a boolean member 
   :samp:`{value}`. If any :samp:`{Args}...` :samp:`{value} members are true,
   the result is equivalent to :cxx:`std::true_type`. Otherwise, the result
   will be equivalent to :cxx:`std::false_type`. The arguments are evaluated
   lazily, which allows for short circuit evaluation of each type given.

   :example:

    .. code-block:: cpp

       using result = meta::any_t<
         is_same<U, T>,
         is_void<void>
         is_void<nullptr_t>
       >;

.. class:: template <class... Args> none

   .. versionadded:: 1.2

   :complexity: O(n)

   Takes a parameter pack :samp:`{Args}...`, where each type in
   :samp:`{Args}...` is a type trait property that contains a boolean member
   named :samp:`{value}`. This metafunction is the inverse of :any:`all`. The
   arguments are evaluated lazily, which allows for short circuit evaluation of
   each type given. It can also be used as a negate metafunction.

   :example:

    .. code-block:: cpp

       using result = meta::none_t<is_void<T>>;

.. class:: template <class... Args> pack

   .. versionadded:: 1.2

   This type is used as a wrapper for parameter packs, and offers a few static
   member functions for ease of use.

   .. function:: static constexpr size_t size () noexcept

      :returns: :samp:`sizeof...({Args})`

   .. function:: static constexpr bool empty () noexcept

      :returns: :samp:`sizeof...(Args) == 0`

   .. type:: front

      If :any:`empty` is false, this is an alias for the first type in
      :samp:`{Args}...`. Otherwise, this alias does not exist.

   .. type:: back

      If :any:`empty` is false, this is an alias for the last type in
      :samp:`{Args}...`. Otherwise, this alias does not exist.

.. class:: template <class T, template <class...> class F> transform

   .. versionadded:: 1.2

   :complexity: O(1)

   Takes a :any:`pack` :samp:`{T}`, and a template class :samp:`{F}`. When the
   underlying dependent alias :any:`type` is instantiated, :samp:`{F}` will
   be applied to each element in :samp:`{T}`, to create a new :any:`pack`.

   :samp:`{F}` is a simple metafunction. It does not require a type alias
   member.

   :example:

    .. code-block:: cpp
       
       template <class T> struct void_to_int : identity<T> { };
       template <> struct void_to_int<void> : identity<int> { };
       template <class T>
       using void_to_int_t = typename void_to_int<T>::type;
       using out = meta::pack<int, int, double, int>;
       using in = meta::pack<void, int, double, void>;
       using result = meta::transform_t<in, void_to_int_t>;
       static_assert(is_same<out, result>::value, "");

.. class:: template <class T, template <class> class F, class U> count_if

   .. versionadded:: 1.2

   :complexity: O(1)

   Takes a :any:`pack` :samp:`{T}`, and a predicate template class :samp:`{F}`.
   The result of this operation will be of type :any:`size`, with its
   :samp:`{value}` member equal to the number of elements in :samp:`{T}` for
   which the predicate :samp:`{F}` returned true. There is no ``_t`` alias
   provided for this metafunction.

   :example:

     .. code-block:: cpp
        
        using in = meta::pack<int, void, double, void, std::string, void>;
        using result = meta::count_if<in, is_void>;
        static_assert(result::value == 3, "");

.. class:: template <class T, template <class> class F, class U> find_if

   .. versionadded:: 1.2

   :complexity: O(n)

   Takes a :any:`pack` :samp:`{T}`, and a template class predicate :samp:`{F}`.
   :samp:`{F}` must contain a static boolean member named :samp:`{value}` when
   given a type :samp:`{U}`. When the underlying dependent alias :any:`type` is
   instantiated, :samp:`{F}` will be applied to each member in :samp:`{T}`
   sequentially. If any element will result in :samp:`{F}`'s :samp:`{value}`
   returning true, a :any:`pack` starting from that element  until the end of
   the original pack is returned. If no element in the parameter pack satisfies
   the predicate :samp:`{F}` then an empty :any:`pack` is returned.

   :example:

    .. code-block:: cpp

       using in = meta::pack<int, std::string, int, double, void, double>;
       using out = meta::pack<void, double>;
       using result = meta::find_if_t<in, std::is_void>;
       static_assert(is_same<out, result>::value, "");

.. class:: template <class T, template <class> class F, class U> filter

   .. versionadded:: 1.2

   :complexity: O(1)

   Takes a :any:`pack` :samp:`{T}`, and a predicate template class :samp:`{F}`.
   Any elements within :samp:`{T}` that do not satisfy the predicate
   :samp:`{F}`, will be removed from the resulting :any:`pack`.

   While this metafunction may *appear* to require sequentially require
   evaluation for each element in :samp:`{T}`, it occurs all at once, giving us
   a decent compile time operation complexity.

   :example:

     .. code-block:: cpp

        template <class T> struct is_not_void : std::true_type { };
        template <> struct is_not_void<void> : std::false_type { };
        using in = meta::pack<int, void, std::string, void, double, void>
        using out = meta::pack<int, std::string, double>;
        using result = meta::filter<in, is_not_void>

.. class:: template <size_t N, class T> element

   .. versionadded:: 1.2

   :complexity: O(?)

   Takes a :cxx:`size_t` :samp:`{N}` and a :any:`pack` :samp:`{T}`. The result
   of this metafunction will be equal to the element that resides at index
   :samp:`{N}` in :samp:`{T}`.

   :example:

     .. code-block:: cpp

        using in = meta::pack<void, std::string, int, double>;
        using result = meta::element_t<0, in>;
        static_assert(is_void<result>::value, "");

.. class:: template <class T, class... Args> push_front

   .. versionadded:: 1.2

   :complexity: O(1)

   Takes a :any:`pack` :samp:`{T}` and a parameter pack :samp:`{Args}...`. This
   metafunction will return a :any:`pack` with the elements :samp:`{Args}...`
   followed by the elements of :samp:`{T}`.

   :example:
  
     .. code-block:: cpp

        using in = meta::pack<void, double>;
        using out = meta::pack<int, double, void, double>;
        using result = meta::push_front_t<in, int, double>;
        static_assert(is_same<out, result>::value, "");

.. class:: template <class T, class... Args> push_back

   .. versionadded:: 1.2

   :complexity: O(1)

   Takes a :any:`pack` :samp:`{T}` and a parameter pack :samp:`{Args}...`. This
   metafunction will return a :any:`pack` with the elements of :samp:`{T}`
   followed by :samp:`{Args}...`.

   :example:

   .. code-block:: cpp

      using in = meta::pack<void, double>;
      using out = meta::pack<void, double, void int>;
      using result = meta::push_back_t<in, void, int>;
      static_assert(is_same<out, result>::value, "");

.. class:: template <class U, class T> index

   .. versionadded:: 1.2

   :complexity: O(n)

   Takes a :any:`pack` :samp:`{T}`, and a type :samp:`{U}`. This metafunction
   will be equal to the index of the first instance of :samp:`{U}` found in
   :samp:`{T}` as a :any:`size`. If :samp:`{T}` does not contain any elements
   of :samp:`{U}`, the maximum value for a :any:`size` is returned instead
   (i.e., :cxx:`std::numeric_limits<std::size_t>::max()`). There is no ``_t``
   alias provided for this metafunction.

   :example:

   .. code-block:: cpp

      using in = meta::pack<void, double, std::string, std::vector<int>>;
      using result = meta::index<std::string, in>;
      static_assert(result::value == 2, "");

.. class:: template <class U, class T> count

   .. versionadded:: 1.2

   :complexity: O(n)

   Takes a :any:`pack` :samp:`{T}`, and a type :samp:`{U}`. This metafunction
   will be equal to the number of elements in :samp:`{T}` that are the same as
   :samp:`{U}`. The result is a :any:`size`.

   :example:

   .. code-block:: cpp

      using in = meta::pack<double, double, int, std::string>;
      using result = meta::count<double, in>;
      static_assert(result::value == 2, "");

.. class:: template <class U, class T> find

   .. versionadded:: 1.2

   :complexity: O(n)

   Takes a :any:`pack` :samp:`{T}` and some type :samp:`{U}`. Upon finding the
   first instance of :samp:`{U}` in :samp:`{T}`, a :any:`pack` starting at
   :samp:`{U}` until the end of :samp:`{T}` is returned. Otherwise an empty
   :any:`pack` is returned.

   This metafunction uses :any:`find_if` under the hood, with a bound version
   of :cxx:`std::is_same` (so that :cxx:`std::is_same` can take one parameter)

   :example:

   .. code-block:: cpp

      using in = meta::pack<int, std::string, int, double>;
      using out = meta::pack<std::string, int, double>;
      using result = meta::find_t<std::string, in>;
      static_assert(is_same<out, result>::value, "");

.. class:: template <class... Args> merge

   .. versionadded:: 1.2

   :complexity: O(1)

   Takes a variable number of :any:`pack` in :samp:`{Args}...`. This
   metafunction will return a new :any:`pack` with each element in each
   :any:`pack` in :samp:`{Args}...`.

   :example:

   .. code-block:: cpp

      using in1 = meta::pack<std::string, double, int, void>;
      using in2 = meta::pack<void, double, int>;
      using out = meta::pack<
        std::string, double, int,
        void, void, double, int
      >;
      using result = meta::merge<in1, in2>;
      static_assert(is_same<result, out>::value, "");


.. class:: template <class T> index_sequence_from

   .. versionadded:: 1.2

   :complexity: O(1)

   Takes a :any:`pack` :samp:`{T}`. This will generate an :any:`index_sequence`
   such that each element in :samp:`{T}` will have a corresponding :samp:`{I}`
   in the returned sequence.

   :example:

   .. code-block:: cpp

      using in = meta::pack<void, double, void>;
      using out = make_index_sequence<3>;
      using result = index_sequence_from<in>;
      static_assert(is_same<out, result>::value, "");

.. class:: template <class T> pop_front

   .. versionadded:: 1.2

   :complexity: O(1)

   Takes a :any:`pack` :samp:`{T}`. This metafunction will return a new
   :any:`pack` with the first element in :samp:`{T}` removed. This metafunction
   will result in an error if :samp:`{T}` has no elements.

   :example:

   .. code-block:: cpp

      using in = meta::pack<int, void, double>;
      using out = meta::pack<void, double>;
      using result = meta::pop_front_t<in>;
      static_assert(is_same<out, result>::value, "");

.. class:: template <class T> pop_back

   .. versionadded:: 1.2

   :complexity: O(1)

   Takes a :any:`pack` :samp:`{T}`. This metafunction will return a new
   :any:`pack` with the last element in :samp:`{T}` removed. This metafunction
   will result in an error if :samp:`{T}` has no elements.

   :example:

   .. code-block:: cpp

      using in = meta::pack<int, void, double>;
      using out = meta::pack<int, void>;
      using result = meta::pop_back_t<in>;
      static_assert(is_same<out, result>::value, "");

.. class:: template <class T> reverse

   .. versionadded:: 1.2

   :complexity: O(1)

   Takes a :any:`pack` :samp:`{T}`. This metafunction will return a new
   :any:`pack` with each element in :samp:`{T}` in its reversed position, such
   that :samp:`{T}`'s first element will be its last, and vice versa.

   :example:

   .. code-block:: cpp

      using in = meta::pack<int, void, double>;
      using out = meta::pack<double, void, int>;
      using result = meta::reverse_t<in>;
      static_assert(is_same<out, result>::value, "");

.. class:: template <class T> to_pack

   .. versionadded:: 1.2

   :complexity: O(1)

   Takes any variadic template class :samp:`{T}`. This metafunction will then
   return a :any:`pack` whose elements correspond to those in :samp:`{T}`.

   :example:

   .. code-block:: cpp

      using in = std::tuple<double, std::string, int>;
      using out = meta::pack<double, std::string, int>;
      using result = meta::to_pack_t<in>;
      static_assert(is_same<out, result>::value, "");

.. class:: template <class T, class U> from_pack

   .. versionadded:: 1.2

   :complexity: O(1)

   Takes a :any:`pack` :samp:`{T}`, and any variadic tempate class :samp:`{U}`.
   This metafunction will return an instantiation of :samp:`{U}` whose elements
   correspond to those in :samp:`{T}`. This metafunction acts as the inverse of
   :any:`to_pack`.

   :example:

   .. code-block:: cpp

      using in = meta::pack<std::string, double, int>;
      using out = std::tuple<std::string, double, int>;
      using result = meta::from_pack_t<in, std::tuple>;
      static_assert(is_same<out, result>::value, "");
