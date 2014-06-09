.. _core-variant-component:

Variant Component
=================

.. default-domain:: cpp

.. |variant| replace:: :class:`variant <variant\<Ts>>`

The |variant| component is a generic type-safe implementation of a
discriminate union. It is equivalent to the Boost.Variant library, with several
small exceptions.

Specifically, the |variant| does not allow for recursive variants,
and makes no attempt to support them in any capacity. It is the author's
opinion that a |variant| should be stack allocated only, and not
have to handle or deal with allocating memory.

Additionally, the |variant| has the ability to perform type-based
pattern matching via lambdas. This allows a user to define the different code
paths that are desired without requiring a user to implement a visitor functor
separately. *This means that a variant is capable of performing a type-based
switch statement*

The variant component resides in ``<core/variant.hpp>``.

.. namespace:: core

.. class:: bad_variant_get

   :inherits: ``std::logic_error``

   Used when the call to get the value out of a |variant| uses the incorrect
   index of the type list.

.. class:: variant<Ts>

   The |variant| type represents a type-safe discriminate union. Much like
   the Boost.Variant, a |variant| is never in an uninitialized state. When
   default constructing a |variant|, the first type in the |variant|'s typelist
   is initialized.

   .. function:: variant (T&& value)

      :requires: *T* be of a type from which any type in *Ts* is constructible.

      Constructs the first type in *Ts* which can be constructed from *T*. The
      way in which this construction takes place is a first come first serve
      construction. The reason for this is simply due to how the constructor
      delegation occurs internally. As such, it is recommended that a |variant|
      contain types that cannot be constructed from each other.

   .. function:: variant (variant const& that)

      Constructs a |variant| with the same type that is initialized in *that*.
      The object is initialized with the type's copy constructor.

   .. function:: variant (variant&& that)

      Constructs a |variant| with the same type that is initialized in *that*.
      The object is initialized with the type's move constructor.

   .. function:: variant ()

      Constructs a |variant| by initializing the first type in its type list.

   .. function:: variant& operator = (variant const&)
                 variant& operator = (variant&&)

      Assigns the contents of the other |variant| to ``*this``. The object
      contained within ``*this`` is destructed first.

   .. function:: bool operator == (variant const& that) const noexcept

      If both |variant|'s :func:`which` is the same value, the values contained
      within are compared.

   .. function:: bool operator < (variant const& that) const noexcept

      If both |variant|'s :func:`which` are equal, the values contained are
      compared. Otherwise, the result of comparing :func:`which` is returned.

   .. function:: void swap (variant&)

      Swaps the contents of both variants.

   .. function:: visit (Visitor&&, args) const
                 visit (Visitor&&, args)

     Visiting a |variant| follows the following semantics. These semantics
     require that, when given a callable type ``Visitor``, and variadic
     arguments ``Args...``, that the return type of the visit will be
     a result of ``common_type_t<invoke_of_t<Visitor, Ts, Args...>...>``.

     If a common type cannot be found, then the visitation function will
     fail to compile properly. This means that a visitor *must* be capable of
     being invoked with all types in the |variant|'s typelist and the arguments
     given. The visitor is executed with the *INVOKE* psuedo expression.

     These same semantics are required for :func:`match`.

     :returns: ``common_type_t<invoke_of_t<Visitor, Ts, Args...>...>``

   .. function:: match (Visitors&&) const
                 match (Visitors&&)

      Takes a variadic number of arguments that are all callable objects. These
      objects are combined into a single visitor and then executed on the
      |variant|.

   .. function:: auto get<N> () noexcept

      Depending on the value of ``*this`` (reference, const reference, rvalue)
      the type of the value returned will be affected as well.

      :noexcept: false
      :returns: The element located at N in the type list.
      :throws: :class:`bad_variant_get` if N != :func:`which`.

   .. function:: std::type_info const& type () const noexcept

      :returns: The typeid of the value currently managed by the |variant|.

   .. function:: std::uint32_t which () const noexcept

      :returns: index into type list of which type is currently managed by the
                variant.

   .. function:: bool empty () const noexcept

      Provided for compatibility with Boost.Variant, and to keep in line with
      the other 'object containers' contained within this library.

      :returns: false


Specializations
---------------

.. class:: hash<variant<Ts>>

   A specialization of ``std::hash<T>`` for variants. Requires that all
   *Ts* in a |variant| be specialized for ``std::hash``.

.. function:: void swap (variant& lhs, variant& rhs)

   Calls :func:`variant\<Ts>::swap`

.. function:: auto const& get<N>(variant const&)
              auto&& get<N>(variant&&)
              auto& get<N>(variant&)

   Calls :func:`variant\<Ts>::get`, and returns the value. This specialization
   is provided to interact with ``std::tuple`` and to provide *some* semblance
   of boost interoperability. However it does not support using the type
   to get the value from the variant.
