Polymorphic Component
=====================

.. default-domain:: cpp

The :class:`polymorphic\<T, Deleter>` component is currently only provided by
MNMLSTC Core. At the time of this writing, it is not planned for inclusion in a
future version of the C++ standard library, nor has it been proposed for
inclusion.

The polymorphic component requires that both RTTI and exceptions be enabled.
The RTTI is absolutely required for the polymorphic deep-copy to take place.

.. namespace:: core

.. warning:: *Only* polymorphic types may be used with
   :class:`polymorphic\<T, Deleter>`. If deep-copy semantics with a
   :class:`std::unique_ptr\<T>` are desired for a non-polymorphic type, use
   :class:`deep_ptr\<T>` instead.

.. class:: bad_polymorphic_reset

   :inherits: std::logic_error

   Thrown when a :func:`polymorphic\<T, Deleter>::reset` is passed a non-null
   pointer and the :class:`polymorphic\<T, Deleter>` does not manager an
   object, or if the passed in pointer differs in type from the currently
   managed object.

.. class:: polymorphic<T, Deleter>

   The :class:`polymorphic\<T, Deleter>` is a smart pointer for polymorphic
   types that retains sole ownership of the *polymorphic* contained type ``T``
   and performs *deep copies* on assignment or copy construction. In this way, a
   :class:`polymorphic\<T, Deleter>` can be seen as a
   :class:`std::unique_ptr\<T>` with deep copy semantics. The interface is
   equivalent to :class:`std::unique_ptr\<T>`. Due to the type erasure that is
   performed, the size of :class:`polymorphic\<T, Deleter>` is
   ``sizeof(std::unique_ptr<T, Deleter> + sizeof(function-ptr)``.

   Unfortunately, do to the lack of polymorphic allocators, custom allocation
   is currently limited to initializing a polymorphic with a custom copier
   function.

   :class:`polymorphic\<T, Deleter>` is *not* polymorphic itself, and is marked
   ``final`` in its interface.

   .. note:: You cannot create a ``polymorphic<T[], Deleter>``. It is for a
      single instance of a given base type ``T``.

   .. type:: unique_type

      Represents the :class:`std::unique_ptr\<T, D>` used internally to hold
      the object.

   .. type:: element_type

      A type equal to ``typename unique_type::element_type``

   .. type:: deleter_type

      A type equal to ``typename unique_type::deleter_type``

   .. type:: copier_type

      The type used to represent the copy function used to perform deep copies.
      It has a type signature of: ``unique_type (*)(unique_type const&)``.

   .. type:: pointer

      A type equal to ``typename unique_type::pointer``

   .. function:: explicit polymorphic (U* ptr)

      Takes a derived pointer to a given type *U*. *U* must be a non-abstract
      type, and have :type:`element_type` as a base class within its
      inheritance tree.

   .. function:: polymorphic (U*, deleter_type const&, copier_type)
                 polymorphic (U*, deleter_type&&, copier_type)

      Takes some derived type *U*, and a deleter_type. The copier_type is an
      optional parameter. The rules

   .. function:: explicit polymorphic (std::unique_ptr<U, D>&&, copier_type)

      A unique_ptr of type *U* where *U* is some derived type of
      :type:`element_type` and a copier function that can properly handle the
      deep copy is permitted. If no copier is provided, one will be provided
      that uses the :type:`std::unique_ptr<U, D>::element_type` as the type
      to cast to.

   .. function:: polymorphic(polymorphic const& that)

      Performs a deep copy with the object managed by ``that``, if such
      an object exists.

   .. function:: polymorphic (polymorphic&& that)

      Moves ``that``'s pointer and copier into ``*this``, and then sets
      the copier to a null copier.

   .. function:: polymorphic () noexcept

      The default constructor for a :class:`polymorphic\<T, Deleter>` will
      place it into a state such that ``bool(polymorphic<T, Deleter> { })``
      will return true.

   .. function:: operator = (std::unique_ptr<U, D>&& ptr)

      Calls ``polymorphic<T, Deleter> { std::move(ptr) }.swap(*this)``

      :returns: ``*this``

   .. function:: operator = (polymorphic<T, Deleter>&& that) noexcept

      Calls ``polymorphic<T, Deleter> { std::move(that) }.swap(*this)``

      :returns: ``*this``

   .. function:: operator = (polymorphic<T, Deleter> const& that)

      Performs a deep copy with the object managed by ``that``, if such
      an object exists.

      :returns: ``*this``

   .. function:: pointer release () noexcept

      Releases the ownership of managed object, if any such object exists.
      Any calls to :func:`polymorphic\<T, Deleter>::get` will return
      ``nullptr`` after this call.

      :returns: pointer to the managed object or ``nullptr`` if there was no
                internal object.

   .. function:: void reset (pointer ptr = nullptr)

      Replaces the managed object. Performs the following actions (these
      differ from the order of operations followed by ``std::unique_ptr``).

      * If the incoming pointer is ``nullptr``, the order of operations
        follows those performed by ``std::unique_ptr``, along with the value
        returned by :func:`polymorphic\<T, Deleter>::get_copier` being set to
        a null copier.
      * If the incoming pointer is *not* ``nullptr``, and there is no
        managed object, a :class:`bad_polymorphic_reset` exception is thrown.
      * If the incoming pointer is *not* ``nullptr``, a ``typeid`` comparison
        between the managed object and the incoming pointer is performed.
        If the ``std::type_info`` returned from both is not identical,
        a :class:`bad_polymorphic_reset` is thrown.
        If the ``std::type_info`` is identical, the order of operations
        follows those performed by ``std::unique_ptr``.

   .. function:: void swap (polymorphic<T, Deleter>& that) noexcept

      Performs a ``std::swap`` with the managed object and the copier function.

   .. function:: pointer get () const noexcept

      :returns: A pointer to the managed object, or ``nullptr`` if no such
                object exists.

   .. function:: deleter_type const& get_deleter () const noexcept
                 deleter_type& get_deleter () noexcept

      :returns: The deleter object used for destruction of the managed object.

   .. function:: copier_type const& get_copier () const noexcept
                 copier_type& get_copier () noexcept

      :returns: The function pointer used for copying the managed object.

   .. function:: operator bool () const noexcept

      .. note:: Due to a limitation with Sphinx-doc, the signature above does
         not allow for 'explicit operators'. ``operator bool`` is marked
         explicit in the :class:`polymorphic\<T, Deleter>` interface.

      :returns: Whether ``*this`` owns an object

   .. function:: element_type& operator * () const

      :returns: An lvalue reference to the object owned by ``*this``.

   .. function:: pointer operator -> () const noexcept

      :returns: a pointer to the object owned by ``*this``.

.. function:: auto default_poly_copier<T, D, U>(std::unique_ptr<T, D> const&)

   This function is used as the default copier when assigning a value or
   unique_ptr to a :class:`polymorphic\<T, Deleter>`. It will perform a deep
   copy with a call to :func:`make_unique<T>`, with type *U* and dynamic_cast
   the stored pointer of T into U as it performs the assignment. The
   :type:`deleter_type` of the given unique_ptr will *also* be copied.

   :returns: :class:`std::unique_ptr\<T, D>`

.. function:: polymorphic<T> make_polymorphic<T, U>(args)

   :func:`make_polymorphic\<T, U>` is provided to supplement the
   ``std::make_shared<T>`` and :func:`make_unique\<T>` functions.
   
   :param args: Variadic template arguments with which to construct
                a U
   :type args: Args&&...
   :returns: :class:`polymorphic\<T, Deleter>`

.. function:: bool operator == (polymorphic<T, D> const&, nullptr_t) noexcept
              bool operator != (polymorphic<T, D> const&, nullptr_t) noexcept
              bool operator >= (polymorphic<T, D> const&, nullptr_t) noexcept
              bool operator <= (polymorphic<T, D> const&, nullptr_t) noexcept
              bool operator >(polymorphic<T, D> const&, nullptr_t) noexcept
              bool operator <(polymorphic<T, D> const&, nullptr_t) noexcept
              bool operator == (nullptr_t, polymorphic<T, D> const&) noexcept
              bool operator != (nullptr_t, polymorphic<T, D> const&) noexcept
              bool operator >= (nullptr_t, polymorphic<T, D> const&) noexcept
              bool operator <= (nullptr_t, polymorphic<T, D> const&) noexcept
              bool operator >(nullptr_t, polymorphic<T, D> const&) noexcept
              bool operator <(nullptr_t, polymorphic<T, D> const&) noexcept

   :returns: The result of comparing :func:`polymorphic<T, Deleter>::get` and
             ``nullptr`` with the given operator.


Specializations
---------------

There are several specializations for :class:`polymorphic\<T, Deleter>` related
to the C++ standard library.

.. class:: std::hash<polymorphic<T, Deleter>>

   This specialization of :class:`std::hash` allows
   :class:`polymorphic\<T, Deleter>` to be used as a key type in associative
   containers.

   For a given :class:`polymorphic\<T, Deleter>` *ptr*, this specialization
   ensures that ``std::hash<polymorphic<T, Deleter>> { }(ptr)`` is equal to
   ``std::hash<T*> { }(ptr.get())``

.. function:: void std::swap<T>(polymorphic<T>& lhs, polymorphic<T>& rhs)

   A specialization of ``std::swap`` that calls
   :func:`polymorphic<T, Deleter>::swap`.


