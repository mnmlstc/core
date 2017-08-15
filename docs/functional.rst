Functional Component
====================

.. namespace:: core

The functional component contains several utilities to assist with functions.
Among these are the implementation of the *INVOKE* pseudo-expression in code
form, as well as an extension of this pseudo-expression with *unpack* semantics
at both compile time and runtime.

Additionally, the functional component provides a :any:`function_traits`
object to discern at compile time the return type and arity of a function. The
ability to discover the types of a given function's argument is also possible.

The functional component can be found in the :file:`<core/{functional}.hpp>`
header.


.. class:: unpack_t

   .. deprecated:: 2.0 Use :any:`apply` instead of invoke and unpack.

   A sentinel class to represent that one wishes to call :any:`invoke` with
   unpack semantics. An instance is available under the name :samp:`{unpack}`.

.. class:: runpack_t

   A sentinel class to represent that one wishes to call :func:`invoke` with
   runtime unpack semantics. An instance is available under the name
   :samp:`{runpack}`.

.. class:: template <class T> is_reference_wrapper

   A type trait that will be :cxx:`std::true_type` if :samp:`{T}` is some form
   of :cxx:`std::reference_wrapper`.

.. class:: template <class T> function_traits

   Function traits to discern various amounts of details for a given callable
   type :samp:`{T}`. The traits allow one to get the :member:`arity` of a
   callable, as well as explore the types of various argument indices.

   .. type:: return_type

      The return type of a given function :samp:`{T}`.

   .. type:: pointer

      A type alias that represents the callable as a function pointer.

   .. member:: static constexpr size_t arity

      Represents the number of arguments the callable :samp:`{T}` takes.

   .. type:: template <size_t N> argument

      Given a :cxx:`std::size_t` :samp:`{N}`, argument will be a type alias for
      the type located at the index in its parameter list.

.. function:: template <class F, class... Args> \
              auto invoke (Functor&& f, Args&&...)

   An implemenetation of the *INVOKE* pseudo-expression as defined in the C++11
   standard.

   .. versionchanged:: 2.0 There are now only two overloads for this function
      that now encompass the previous versions, while also augmenting their
      use (:cxx:`std::reference_wrapper` and smart pointers are now guaranteed
      to be usable as the 'object' parameter when invoking a member function
      pointer or pointer to member).

   :example:

    .. code-block:: cpp

       std::string str { "Hello, World!" };
       auto shared_ptr = make_shared(str);
       auto ref = std::ref(str);
       auto ptr = std::addressof(str);
       auto mem_fn = &::std::string::size;

       assert(core::invoke(mem_fn, shared_ptr) == 13);
       assert(core::invoke(mem_fn, ptr) == 13);
       assert(core::invoke(mem_fn, ref) == 13);

.. function:: template <class F, class T> \
              auto apply (Functor&& f, Tuple&& t)

   .. versionadded:: 2.0

   Unpacks the elements of :samp:`{t}` into :samp:`{f}` and then invokes
   :samp:`{f}`.

.. function:: template <class F>\
              apply_functor<F> make_apply(F&& f)

   .. versionadded:: 2.0

   Creates an :any:`apply_functor` via template deduction.


Function Objects
----------------

Several function objects are provided for use with various C++ standard library
algorithms and functions. The operator function objects (e.g.,
:any:`plus`, :any:`minus`, etc.) are provided here due to the C++14
requirement that their :cxx:`operator ()` be :cxx:`constexpr`. Additionally,
the :cxx:`void` specialization of these function objects is implemented. This
specialization will use template deduction for its :cxx:`operator ()`
arguments.

Below is an example of using the 'default' version of the operator function
objects:

.. code-block:: cpp

   static constexpr auto less = core::less<> { };
   static_assert(less(3, 4), "");

.. class:: template <class F> apply_functor

   .. versionadded:: 2.0

   A functor type that allows :any:`apply` to be used in conjunction with
   the standard library algorithm functions.

   :example:

    .. code-block:: cpp

       std::vector<std::tuple<std::string, int, double>> x;
       auto f = make_apply([] (std::string, int, double) { });
       for_each(core, f);

.. class:: template <class T> converter

   .. versionadded:: 2.0

   A functor type that allows construction of a :samp:`{T}` from any other
   type. This is most useful for calls to :any:`transform`, when a
   :cxx:`std::mem_fn` can't be used such as a constructor. Additionally, it
   also permits easier conversion between types with explicit constructors.
   It's :cxx:`operator ()` takes a parameter pack, to allow better interop with
   :cxx:`std::bind`

   .. function:: constexpr T operator () (Args&&... args) const

      :returns: :samp:`{T}` constructed via :samp:`{args}`

      :example:

         .. code-block:: cpp

            using duration = std::chrono::seconds;
            std::vector<int> ints { 1, 2, 3, 4, 5 };
            std::vector<duration> secs { };
            auto inserter = std::back_inserter(secs);
            core::transform(ints, secs, converter<duration> { });
      

Arithmetic Function Objects
^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. class:: template <class T> plus

   .. versionadded:: 2.0

   Used to call :cxx:`operator +` on two instances of type :samp:`{T}`.

   .. function:: constexpr T operator () (T const& lhs, T const& rhs) const

      Calls :cxx:`operator +` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result.

.. class:: template <> plus<void>

   .. versionadded:: 2.0

   .. type:: is_transparent

   .. function:: constexpr auto operator () (T&& lhs, U&& rhs) const

      Calls :cxx:`operator +` via template deduction on :samp:`{lhs}` and
      :samp:`{rhs}` and returns the result.

.. class:: template <class T> minus

   .. versionadded:: 2.0

   Used to call :cxx:`operator -` on two instances of type :samp:`{T}`.

   .. function:: constexpr T operator () (T const& lhs, T const& rhs) const

      Calls :cxx:`operator -` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result.

.. class:: template <> minus<void>

   .. versionadded:: 2.0

   .. type:: is_transparent

   .. function:: constexpr auto operator () (T&& lhs, U&& rhs) const

      Calls :cxx:`operator -` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result.

.. class:: template <class T> multiplies

   .. versionadded:: 2.0

   Used to call :cxx:`operator*` on two instances of type :samp:`{T}`.

   .. type:: result_type
             first_argument_type
             second_argument_type

      Represents :samp:`{T}`

   .. function:: constexpr T operator () (T const& lhs, T const& rhs) const

      Calls :cxx:`operator*` on :samp:`{lhs}` and :samp:`{rhs}` and returns the
      result.

.. class:: template <> multiplies<void>

   .. versionadded:: 2.0

   .. type:: is_transparent

   .. function:: constexpr auto operator () (T&& lhs, U&& rhs) const

      Calls :cxx:`operator*` on :samp:`{lhs}` and :samp:`{rhs}` and returns the
      result.

.. class:: template <class T> divides

   .. versionadded:: 2.0

   Used to call :cxx:`operator /` on two instances of type :samp:`{T}`.

   .. type:: result_type
             first_argument_type
             second_argument_type

      Represents :samp:`{T}`

   .. function:: constexpr T operator () (T const& lhs, T const& rhs) const

      Calls :cxx:`operator /` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result.

.. class:: template <> divides<void>

   .. versionadded:: 2.0

   .. type:: is_transparent

   .. function:: constexpr auto operator () (T&& lhs, U&& rhs) const

      Calls :cxx:`operator /` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result.

.. class:: template <class T> modulus

   .. versionadded:: 2.0

   Used to call ``operator %`` on two instances of :samp:`{T}`.

   .. type:: result_type
             first_argument_type
             second_argument_type

      Represents :samp:`{T}`

   .. function:: constexpr T operator () (T const& lhs, T const& rhs) const

      Calls ``operator %`` on :samp:`{lhs}` and :samp:`{rhs}` and returns the
      result.

.. class:: template <> modulus<void>

   .. versionadded:: 2.0

   .. type:: is_transparent

   .. function:: constexpr auto operator () (T&& lhs, U&& rhs) const

      Calls ``operator %`` on :samp:`{lhs}` and :samp:`{rhs}` and returns the
      result.

Comparison Function Objects
^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. class:: template <class T> equal_to

   .. versionadded:: 2.0

   Used to call :cxx:`operator ==` on two instances of :samp:`{T}`.

   .. type:: first_argument_type
             second_argument_type

      Represents :samp:`{T}`

   .. type:: result_type

      Represents :cxx:`bool`.

   .. function:: constexpr bool operator () (T const& lhs, T const& rhs) const

      Calls :cxx:`operator ==` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result

.. class:: template <> equal_to<void>

   .. versionadded:: 2.0

   .. type:: is_transparent

   .. function:: constexpr auto operator () (T&& lhs, U&& rhs) const

      Calls :cxx:`operator ==` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result.

.. class:: template <class T> not_equal_to

   .. versionadded:: 2.0

   Used to call :cxx:`operator !=` on two instances of :samp:`{T}`.

   .. type:: first_argument_type
             second_argument_type

      Represents :samp:`{T}`

   .. type:: result_type

      Represents :cxx:`bool`.

   .. function:: constexpr bool operator () (T const& lhs, T const& rhs) const

      Calls :cxx:`operator !=` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result

.. class:: template <> not_equal_to<void>

   .. versionadded:: 2.0

   .. type:: is_transparent

   .. function:: constexpr auto operator () (T&& lhs, U&& rhs) const

      Calls :cxx:`operator !=` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result

.. class:: template <class T> greater_equal

   .. versionadded:: 2.0

   Used to call :cxx:`operator >=` on two instances of :samp:`{T}`.

   .. type:: first_argument_type
             second_argument_type

      Represents :samp:`{T}`

   .. type:: result_type

      Represents :cxx:`bool`.

   .. function:: constexpr bool operator () (T const& lhs, T const& rhs) const

      Calls :cxx:`operator >=` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result

.. class:: template <> greater_equal<void>

   .. versionadded:: 2.0

   .. type:: is_transparent

   .. function:: constexpr auto operator () (T&& lhs, U&& rhs) const

      Calls :cxx:`operator >=` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result.

.. class:: template <class T> less_equal

   .. versionadded:: 2.0

   Used to call :cxx:`operator <=` on two instances of :samp:`{T}`.

   .. type:: first_argument_type
             second_argument_type

      Represents :samp:`{T}`

   .. type:: result_type

      Represents :cxx:`bool`.

   .. function:: constexpr bool operator () (T const& lhs, T const& rhs) const

      Calls :cxx:`operator <=` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result

.. class:: template <> less_equal<void>

   .. versionadded:: 2.0

   .. type:: is_transparent

   .. function:: constexpr auto operator () (T&& lhs, U&& rhs) const

      Calls :cxx:`operator <=` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result

.. class:: template <class T> greater

   .. versionadded:: 2.0

   Used to call :cxx:`operator >` on two instances of :samp:`{T}`.

   .. type:: first_argument_type
             second_argument_type

      Represents :samp:`{T}`

   .. type:: result_type

      Represents :cxx:`bool`.

   .. function:: constexpr bool operator () (T const& lhs, T const& rhs) const

      Calls :cxx:`operator >` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result

.. class:: template <> greater<void>

   .. versionadded:: 2.0

   .. type:: is_transparent

   .. function:: constexpr auto operator () (T&& lhs, U&& rhs) const

      Calls :cxx:`operator >` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result

.. class:: template <class T> less

   .. versionadded:: 2.0

   Used to call :cxx:`operator <` on two instances of :samp:`{T}`.

   .. type:: first_argument_type
             second_argument_type

      Represents :samp:`{T}`

   .. type:: result_type

      Represents :cxx:`bool`.

   .. function:: constexpr bool operator () (T const& lhs, T const& rhs) const

      Calls :cxx:`operator <` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result.

.. class:: template <> less<void>

   .. versionadded:: 2.0

   .. type:: is_transparent

   .. function:: constexpr auto operator () (T&& lhs, U&& rhs) const

      Calls :cxx:`operator <` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result.

Logical Function Objects
^^^^^^^^^^^^^^^^^^^^^^^^

.. class:: template <class T> logical_and

   .. versionadded:: 2.0

   Used to call :cxx:`operator and` on two instances of :samp:`{T}`.

   .. type:: first_argument_type
             second_argument_type

      Represents :samp:`{T}`

   .. type:: result_type

      Represents :cxx:`bool`.

   .. function:: constexpr bool operator () (T const& lhs, T const& rhs) const

      Calls :cxx:`operator and` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result.

.. class:: template <> logical_and<void>

   .. versionadded:: 2.0

   .. type:: is_transparent

   .. function:: constexpr auto operator () (T&& lhs, U&& rhs) const

      Calls :cxx:`operator and` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result

.. class:: template <class T> logical_or

   .. versionadded:: 2.0

   Used to call :cxx:`operator or` on two instances of :samp:`{T}`.

   .. type:: first_argument_type
             second_argument_type

      Represents :samp:`{T}`

   .. type:: result_type

      Represents :cxx:`bool`

   .. function:: constexpr bool operator () (T const& lhs, T const& rhs) const

      Calls :cxx:`operator or` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result.

.. class:: template <> logical_or

   .. versionadded:: 2.0

   .. type:: is_transparent

   .. function:: constexpr auto operator () (T&& lhs, U&& rhs) const

      Calls :cxx:`operator or` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result.

.. class:: template <class T> logical_not

   .. versionadded:: 2.0

   Used to call :cxx:`operator not` on an instance of :samp:`{T}`.

   .. type:: argument_type

      Represents :samp:`{T}`

   .. type:: result_type

      Represents :cxx:`bool`

   .. function:: constexpr bool operator () (T const& arg) const

      Calls :cxx:`operator not` on :samp:`{arg}` and returns the result.

.. class:: template <> logic_not<void>

   .. versionadded:: 2.0

   .. type:: is_transparent

   .. function:: constexpr auto operator () (T&& arg) const

      Calls :cxx:`operator not` on :samp:`{arg}` and returns the result.

Bitwise Function Objects
^^^^^^^^^^^^^^^^^^^^^^^^

.. class:: template <class T> bit_and

   .. versionadded:: 2.0

   Used to call :cxx:`operator &` on two instances of :samp:`{T}`.

   .. type:: first_argument_type
             second_argument_type
             result_type

      Represents :samp:`{T}`

   .. function constexpr T operator () (T const& lhs, T const& rhs) const

      Calls :cxx:`operator &` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result.

.. class:: template <> bit_and<void>

   .. version::added:: 2.0

   .. type:: is_transparent

   .. function:: constexpr auto operator () (T&& lhs, U&& rhs) const

      Calls :cxx:`operator &` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result.

.. class:: template <class T> bit_xor

   .. versionadded:: 2.0

   Used to call :cxx:`operator ^` on two instances of :samp:`{T}`.

   .. type:: first_argument_type
             second_argument_type
             result_type

      Represents :samp:`{T}`

   .. function:: constexpr T operator () (T const& lhs, T const& rhs) const

      Calls :cxx:`operator ^` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result.

.. class:: template <> bit_xor<void>

   .. versionadded:: 2.0

   .. type:: is_transparent

   .. function:: constexpr auto operator () (T&& lhs, U&& rhs) const

      Calls :cxx:`operator ^` on :samp:`{lhs}` and :samp:`{rhs}` and returns
      the result.

.. class:: template <class T> bit_or

   .. versionadded:: 2.0

   Used to call :cxx:`operator|` on two instances of :samp:`{T}`.

   .. type:: first_argument_type
             second_argument_type
             result_type

      Represents :samp:`{T}`

   .. function:: constexpr T operator () (T const& lhs, T const& rhs) const

      Calls :cxx:`operator|` on :samp:`{lhs}` and :samp:`{rhs}` and returns the
      result.

.. class:: template <> bit_or<void>

   .. versionadded:: 2.0

   .. type:: is_transparent

   .. function:: constexpr auto operator () (T&& lhs, U&& rhs) const

      Calls :cxx:`operator|` on :samp:`{lhs}` and :samp:`{rhs}` and returns the
      result.

.. class:: template <class T> bit_not

   .. versionadded:: 2.0

   Used to call :cxx:`operator ~` on one instance of :samp:`{T}`.

   .. type:: argument_type
             result_type

      Represents :samp:`{T}`

   .. function:: constexpr T operator () (T const& arg) const

      Calls :cxx:`operator ~` on :samp:`{arg}` and returns the result.

.. class:: template <> bit_not<void>

   .. versionadded:: 2.0

   .. type:: is_transparent

   .. function:: constexpr auto operator () (T&& arg) const

      Calls :cxx:`operator ~` on :samp:`{arg}` and returns the result.

Proposal Extensions
-------------------

This section contains some extensions to the original prosposals, however,
they are either deprecated, or not useful, and will be removed in a future
version of Core.

.. function:: auto invoke (unpack_t, Functor&& f, Unpackable&& u)
              auto invoke (unpack_t, Unpackable&& u)

   .. deprecated:: 2.0 Use :func:`apply` instead.

.. function:: auto invoke (runpack_t, Functor&& f, Runpackable&& r)

   .. deprecated:: 2.0 No replacement has been written. This function was a
      mistake

   :requires: :samp:`{r}` must have a member function named :samp:`at`, which
              takes a :cxx:`std::size_t` as its parameters.
   :throws: ``std::out_of_range``
   :noexcept: false

   This version of :any:`invoke` uses runtime unpacking semantics. It will take
   the arity of :samp:`{Functor}, and then unpack :samp:`{r}` via its
   :samp:`at` member function. As an example a :samp:`{Functor}` with
   4 arguments, and a :samp:`{Runpackable}` of type :cxx:`std::vector<int>`
   would expand to:

   .. code-block:: cpp

      core::invoke(
        std::forward<Functor>(functor),
        std::forward<Runpackable>(runpackable).at(0),
        std::forward<Runpackable>(runpackable).at(1),
        std::forward<Runpackable>(runpackable).at(2),
        std::forward<Runpackable>(runpackable).at(3),
      );

   As the standard containers all throw :cxx:`std::out_of_range`, this function
   should be expected to as well.

   .. note:: *ALL* standard containers with an ``at`` member function can be
      used with this, including the associative containers, such as
      ``std::map``, and ``std::unordered_map``. *However*, the requirement that
      the ``at`` member function take a ``std::size_t`` remains.
