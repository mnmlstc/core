Variant Component
=================

.. namespace:: core

.. index:: variant

The :any:`variant` component is a generic type-safe implementation of a
discriminate union. It is equivalent to the Boost.Variant library, with several
small exceptions.

Specifically, the :any:`variant` does not allow for recursive variants,
and makes no attempt to support them in any capacity. It is the author's
opinion that a :any:`variant` should be stack allocated only, and not
have to handle or deal with allocating memory.

Additionally, the :any:`variant` has the ability to perform type-based
pattern matching via lambdas. This allows a user to define the different code
paths that are desired without requiring a user to implement a visitor functor
separately. *This means that a variant is capable of performing a type-based
switch statement*

The variant component resides in :file:`<core/{variant}.hpp>`.

.. index:: variant; exceptions

.. class:: bad_variant_get

   :inherits: :cxx:`std::logic_error`

   Used when the call to :any:`get` the value out of a :any:`variant` uses the
   incorrect index of the type list.

   This type is unavailable if :c:macro:`CORE_NO_EXCEPTIONS` is defined.

.. class:: template <class... Ts> variant

   The :any:`variant` type represents a type-safe discriminate union. Much like
   the Boost.Variant, a :any:`variant` is never in an uninitialized state. When
   default constructing a :any:`variant`, the first type in the
   :any:`variant`'s typelist is initialized.

   .. index:: variant; constructors

   .. function:: template <class T> variant (T&& value)

      :requires: :samp:`{T}` be of a type from which any type in :samp:`{Ts}`
                 is constructible.

      Constructs the first type in :samp`{Ts...} which can be constructed from
      :samp:`{T}`. The way in which this construction takes place is a first
      come first serve construction. The reason for this is simply due to how
      the constructor delegation occurs internally. 

      .. versionadded:: 1.2

         If a :any:`variant` contains types that are constructible from each
         other, and a :samp:`{T}` is passed that is :samp:`{exactly}` the same
         type of one of the :any:`variant`'s possible states, it will
         immediately construct that one, instead of trying to find the first
         type that can be constructed from the given :samp:`{T}`.

      .. note:: Before 1.2, it was recommended that a :any:`variant` contain
         types that cannot be constructed from each other. This is no longer
         required.


   .. function:: variant (variant const& that)

      Constructs a :any:`variant` with the same type that is initialized in
      :samp:`{that}`. The object is initialized with the type's copy
      constructor.

   .. function:: variant (variant&& that)

      Constructs a :any:`variant` with the same type that is initialized in 
      :samp:`{that}`. The object is initialized with the type's move
      constructor.

   .. function:: variant ()

      Constructs a :any:`variant` by initializing the first type in its type
      list.

   .. function:: variant& operator = (variant const&)
                 variant& operator = (variant&&)

      Assigns the contents of the other :any:`variant` to :cxx:`*this`. The
      object contained within :cxx:`*this` is destructed first.

   .. function:: void swap (variant&)

      Swaps the contents of both variants.

   .. index:: variant; operations

   .. function:: auto visit (Visitor&&, Args&&... args) const
                 auto visit (Visitor&&, Args&&... args)

     Visiting a :any:`variant` follows the following semantics. These semantics
     require that, when given a callable type :samp:`{Visitor}`, and variadic
     arguments :samp:`{args}...``, that the return type of the visit will be
     a result of
     :samp:`common_type_t<invoke_of_t<{Visitor}, {Ts}, {Args}...>...>`.

     If a common type cannot be found, then the visitation function will
     fail to compile properly. This means that a visitor *must* be capable of
     being invoked with all types in the :any:`variant`'s typelist and the
     arguments given. The visitor is executed with the *INVOKE* psuedo
     expression.

     These same semantics are required for :any:`match`.

     :returns: :samp:`common_type_t<invoke_of_t<{Visitor}, {Ts}, {Args}...>...>`

   .. function:: auto match (Visitors&&... visitors) const
                 auto match (Visitors&&... visitors)

      Takes a variadic number of :samp:`{visitors}` that are all callable
      objects. These objects are combined into a single visitor and then
      executed on the :any:`variant`.

      :example:

        .. code-block:: cpp

           variant<int, std::string> item { };
           item.match(
             [] (int v) { std::cout << v << std::endl; },
             [] (std::string const& s) { std::cout << s << std::endl; });

   .. index:: variant; observers

   .. function:: std::type_info const& type () const noexcept

      :returns: The :cxx:`::std::type_info` of the value currently managed by
                the :any:`variant`.

   .. function:: std::uint32_t which () const noexcept

      :returns: index into type list of which type is currently managed by the
                variant.

   .. function:: bool empty () const noexcept

      Provided for compatibility with Boost.Variant, and to keep in line with
      the other 'object containers' contained within this library.

      :returns: false

   .. index:: variant; operators

   .. function:: bool operator == (variant const& that) const noexcept

      If both :any:`variant`'s :any:`which` is the same value, the values
      contained within are compared via :cxx:`operator ==`. Otherwise, the
      result of comparing any:`which` is returned.

   .. function:: bool operator < (variant const& that) const noexcept

      If both :any:`variant`'s :any:`which` are equal, the values contained are
      compared. Otherwise, the result of comparing :any:`which` is returned.


.. index:: variant; functions

.. function:: template <size_t N> auto const& get (variant const& v)
              template <size_t N> auto&& get (variant&& v)
              template <size_t N> auto& get (variant& v)

   Given an index :samp:`{N}`, which is within the range
   :samp:`[0, sizeof...({Ts}))`, returns :samp:`{T}` if :any:`which` is
   equal to :samp:`{N}`. Throws an exception otherwise.

   :noexcept: :cxx:`false`
   :throws: :any:`bad_variant_get`

.. function:: template <size_t N> T const* get (variant const* v)
              template <size_t N> T* get (variant* v)

   A pointer form of :any:`get\<N>`. Given an index :samp:`{N}`, which is
   within the range :samp:`[0, sizeof...({Ts}))`, returns :samp:`{T}` if
   :any:`which` is equal to :samp:`{N}`. Otherwise a :cxx:`nullptr` is
   returned.

.. function:: template <class T> T const& get (variant const& v)
              template <class T> T&& get (variant&& v)
              template <class T> T& get (variant& v)

   Given a type :samp:`{T}`, where :samp:`{T}` is one of the types that 
   :samp:`{v}` can contain, return it if the :any:`variant` contains it.
   Throws an exception otherwise.

   :noexcept: :cxx:`false`
   :throws: :any:`bad_variant_get`

.. function:: template <class T> T const* get (variant const* v)
              template <class T> T* get (variant* v)

   A pointer form of :any:`get\<T>`. Given a type :samp:`{T}`, where
   :samp:`{T}` is within the parameter pack of :samp:`{v}`, return a pointer to
   the element of type :samp:`{T}` if :samp:`{v}` currently contains one.
   Otherwise a :cxx:`nullptr` is returned.

.. function:: void swap (variant& lhs, variant& rhs)

   Calls :any:`variant\<Ts...>::swap`. Equivalent to :samp:`{lhs}.swap({rhs})`

Specializations
---------------

These are specializations placed in the :cxx:`std` namespace.

.. namespace:: std

.. class:: template <class... Ts> hash<variant<Ts...>>

   A specialization of :cxx:`std::hash<T>` for variants. Requires that all
   :samp:`{Ts}...` in a :any:`variant` be specialized for :cxx:`std::hash`.
