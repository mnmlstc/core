Any Component
=============

.. default-domain:: cpp

The ``any`` component is currently available in Boost (and has been for quite
some time). There is, at the time of this writing, a `draft proposal
<http://beman.github.io/dot16/any-proposal.html>`_ for a standard any type,
which has several API distinctions from the Boost.Any implementation, from which
it is inspired. Instance, the proposal recommends that an any implementation
make a small object optimization to avoid allocations for small and integral
types. The MNMLSTC Core any implements this optimization, and attempts to
follow the proposal as closely as possible.

.. note:: While the :class:`any`it does not provide the
  *allocator.uses.construction* constructors. This is due to several library
  implementations not implementing them for various containers, and to
  alleviate possible confusing errors, these have been omitted from the
  MNMLSTC implementation.

.. namespace:: core

.. class:: bad_any_cast

   :inherits: std::bad_cast

   This is the exception thrown when :cpp:func:`core::any_cast` fails.
   It inherits from ``std::bad_cast``.

   .. function:: char const* what () const noexcept

                 Returns the string "bad any cast". At some point in the future
                 a more descriptive error may be given.

.. class:: any

   .. function:: any (any const&)
                 any (any&&) noexcept
                 any () noexcept

      The default set of constructors work as one might imagine. The copy
      constructor is not marked as noexcept as the underlying type *may*
      throw an exception, and due to the type erasure performed, the any has
      no way of enforcing this at compile time.

   .. function:: any (ValueType&& value)

                 When constructing an any with any given ``ValueType``, it will
                 perform a series of compile time checks to see whether it
                 should perform the small object optimization.

                 :type value: ValueType&&
                 :raises: Any exceptions thrown by the copy or move constructor
                          of the given ValueType.

   .. function:: any& operator = (any const&)
                 any& operator = (any&&)

   .. function:: void any::swap (any&)

   .. function:: std::type_info const& type () const noexcept

                 Returns the ``std::type_info`` for the type contained within.
                 If the any is empty, it will return ``typeid(void)``.

   .. function:: bool empty () const noexcept

                 If the any does not contain any data (i.e.,
                 :cpp:func:`any::type` returns ``typeid(void)``), it will
                 return true.

.. function:: ValueType any_cast (any const&)
              ValueType any_cast (any&)
              ValueType const* any_cast (any const*)
              ValueType* any_cast (any*)

              :raises: :cpp:class:`bad_any_cast`
