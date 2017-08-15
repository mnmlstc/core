Utility Component
=================

.. namespace:: core

The utility component provides additional functions and meta templates that
cannot be categorized. Most of the utilities provided are for working with
type lists.

The utility component can be found in the :file:`<core/{utility}.hpp>` header.

.. class:: template <class T, T... Is> integer_sequence

   Meta template to generate a series of :samp:`{T}` to allow easier unpacking.

.. class:: template <size_t... Is> index_sequence

   A shortcut for :any:`integer_sequence`.

.. class:: template <size_t N> make_index_sequence

   Used to generate the :any:`index_sequence`, with :samp:`{N}` numbers.

.. class:: template <size_t N, class... Ts> type_at

   .. type:: type

      When accessed, provides the type at the index :samp:`{N}` in the type
      list :samp:`{Ts}`

.. function:: template <size_t N> constexpr auto value_at(Ts&&...) noexcept

   :returns: The value located at the index :samp:`{N}` in the type list
             :samp:`{Ts}`.
   :requires: :samp:`{N}` may not be >= :samp:`sizeof...({Ts})`

.. class:: template <class Callable> scope_guard

   Scope guard acts much like the Boost.ScopeExit library. It is non-copyable,
   but movable. It can be constructed with any callable type
   :samp:`{Callable}`. The type :samp:`{Callable}` must be move assignable and
   move constructible, and marked :cxx:`noexcept`. :samp:`{Callable}` must have
   a function arity of 0 (that is, it takes no arguments). It does not matter
   whether or not :samp:`{Callable}` returns a value, as it will be ignored.
   To easily construct a :any:`scope_guard`, a function named
   :any:`make_scope_guard` is provided for type inference. This can easily be
   used to give a :any:`scope_guard` a lambda. As an example::

      int x = new int { 4 }
      auto scope_guard = core::make_scope_guard([x] { delete x; });

   .. function:: void dismiss () noexcept

      Calling :any:`dismiss` on a :any:`scope_guard` will result in its
      :samp:`{Callable}` never being invoked. This can be seen as a way to
      *rollback* bookkeeping code.

.. class:: template <class T> capture

   This is used to provide a way to emulate C++14 move captures. It holds a
   value, and can only be move constructed or copy constructed from a non-const
   reference. A function named :any:`make_capture` is provided for type
   inference.

   .. type:: value_type

      An alias for :samp:`{T}`.

   .. type:: reference

      An alias for :samp:`{value_type}&`

   .. type:: pointer

      An alias for :samp:`{value_type}*`

   .. function:: capture (capture& that)

      Moves the value managed by :samp:`{that}` when constructing.

   .. function:: operator reference () const noexcept

      Allows the :any:`capture` to implicitly convert to an lvalue reference
      to its underlying data.

   .. function:: reference operator * () const noexcept

      Provided for shorthand of :any:`get`

      :returns: A :any:`reference` to the stored value.

   .. function:: pointer operator -> () const noexcept

      Provided for convenient access to the underlying value owned by a
      :any:`capture`.

   .. function:: reference get () const noexcept

      :returns: A :any:`reference` to the value owned by the :any:`capture`

.. function:: capture<T> make_capture (remove_reference_t<T>&& ref)
              capture<T> make_capture (remove_reference_t<T>& ref)

   Constructs a :any:`capture` from the given :samp:`{ref}`.

   :note: :samp:`{T}` must be move constructible.

.. function:: auto make_scope_guard(Callable&& callable) noexcept

   Constructs a :any:`scope_guard` from the given :samp:`{callable}`.

.. function:: T exchange (T& obj, U&& value)

   Replaces the value stored in :samp:`{obj}` with :samp:`{value}`.

   :returns: The old value stored in :samp:`{obj}`.

   :requires: :samp:`{T}` must be move constructible.
              :samp:`{T}` must be able to accept objects of type :samp:`{U}`
              via move assignment.

   :noexcept: :samp:`is_nothrow_move_constructible<{T}>`
              :samp:`is_nothrow_assignable<add_lvalue_reference_t<{T}>, {U}>`

.. function:: constexpr auto to_integral (E e) noexcept

   Converts an :cxx:`enum` or :cxx:`enum class` to its underlying type.

   :requires: :samp:`{E}` must satisfy :cxx:`std::is_enum`.
   :returns: :samp:`underlying_type_t<{E}>`

.. function:: constexpr T&& forward (remove_reference_t<T>&&) noexcept
              constexpr T&& forward (remove_reference_t<T>&) noexcept

   ``constexpr`` versions of ``std::forward``.

.. function:: constexpr remove_reference_t<T>&& move (T&&) noexcept

   ``constexpr`` version of ``std::move``.
