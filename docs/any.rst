Any Component
=============

.. default-domain:: cpp

.. |any| replace:: :class:`any`

The |any| component is currently available in Boost (and has been for
quite some time). There is, at the time of this writing, a `draft proposal
<http://beman.github.io/dot16/any-proposal.html>`_ for a standard any type,
which has several API distinctions from the Boost.Any implementation, from which
it is inspired. Instance, the proposal recommends that an any implementation
make a small object optimization to avoid allocations for small and integral
types. The MNMLSTC Core any implements this optimization, and attempts to
follow the proposal as closely as possible.

.. note:: The |any| component does *not* implement the
   *allocator.uses.construction* constructors defined in the any proposal. The
   reason for this is due to:

    * Several libraries do not include the constructors necessary for types
      that define them
    * In most cases the compiler cannot properly disambiguate which constructor
      to use, even with the use of type traits. This was mentioned on the LLVM
      blog, and is mentioned as part of
      `LWG Defect 2255 <http://cplusplus.github.io/LWG/lwg-active.html#2255>`_.

   Because this component was written for C++11, it was decided to omit these
   additional allocator aware constructors.

.. namespace:: core

.. class:: bad_any_cast

   :inherits: std::bad_cast

   This is the exception thrown when :func:`any_cast` fails.
   It inherits from ``std::bad_cast``.

   .. function:: char const* what () const noexcept

      Returns the string "bad any cast". At some point in the future a more
      descriptive error may be given.

.. class:: any

   .. function:: any (any const&)
                 any (any&&) noexcept
                 any () noexcept

      The default set of constructors work as one might imagine. The copy
      constructor is not marked as noexcept as the underlying type *may*
      throw an exception, and due to the type erasure performed, the |any| has
      no way of enforcing this at compile time.

   .. function:: any (ValueType&& value)

      When constructing an |any| with a given ``ValueType``, it will perform a
      series of compile time checks to see whether it should perform the small
      object optimization. If the object is deemed small enough, it will not
      allocate memory. Otherwise, a new ValueType will be allocated via
      operator new, and constructed with the given *value*.

      :type value: ValueType&&
      :raises: Any exceptions thrown by the copy or move constructor
               of the given ValueType.

   .. function:: any& operator = (any const&)
                 any& operator = (any&&) noexcept

      Assigns the contents of the incoming any to ``*this``.

   .. function:: void any::swap (any&) noexcept

      Swaps the object contained within the given |any| with the one contained
      within ``*this``.

   .. function:: std::type_info const& type () const noexcept

      Returns the ``std::type_info`` for the type contained within. If the
      |any| is empty, it will return ``typeid(void)``.

   .. function:: bool empty () const noexcept

      If the |any| does not contain any data (i.e. :func:`any::type` returns
      ``typeid(void)``), it will return true.

   .. function:: void clear () noexcept

      :postcondition: :func:`any::empty` == true

      Destroys the object contained within the |any|.



.. function:: ValueType any_cast (any const&)
              ValueType any_cast (any&&)
              ValueType any_cast (any&)

   .. todo:: Discuss behavior and return value.

   :raises: :class:`bad_any_cast`

.. function:: ValueType const* any_cast (any const*)
              ValueType* any_cast (any*)

   .. todo:: Discuss behavior and return value
