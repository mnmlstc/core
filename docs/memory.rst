Memory Component
================

.. default-domain:: cpp

This section discusses the memory component of MNMLSTC Core. Contained in this
component are two new smart pointers (both with deep copy semantics) and a
C++11 equivalent to C++14's :func:`make_unique\<T>`.

.. |poly_ptr| replace:: :class:`poly_ptr <poly_ptr\<T, Deleter>>`
.. |deep_ptr| replace:: :class:`deep_ptr <deep_ptr\<T, Deleter, Copier>>`

.. namespace:: core

.. class:: poly_ptr<T, Deleter>

   The |poly_ptr| is a smart pointer for polymorphic types that
   retains sole ownership of the *polymorphic* type ``T`` and performs *deep
   copies* on assignment or copy construction.  This use is extremely close to
   the :class:`any`, however it affords the user the ability to ascertain what
   possible types the |poly_ptr| has while also allowing custom allocation,
   copying, and deallocation.  In this way, a |poly_ptr| can be seen as a
   ``std::unique_ptr`` with deep-copy semantics, and type erasure. |poly_ptr|
   has an interface equivalent to ``std::unique_ptr``. Due to the type erasure
   that is used, the size of a |poly_ptr| is
   ``sizeof(std::unique_ptr<T, Deleter> + sizeof(function-ptr)``.

   .. note:: Due to the lack of polymorphic allocators in C++, custom memory
      allocation is currently relegated to performing allocation within a
      user-supplied copier function. Because the copier function is a function
      pointer this means that |poly_ptr| is restricted to stateless lambdas
      or function pointers.

   The |poly_ptr| requires that RTTI and exceptions be enabled. Because of
   the type-erasure used, RTTI is absolutely required to for the polymorphic
   deep-copy to take place.

   .. warning:: *Only* a polymorphic type (that is, any type where the
      ``std::is_polymorphic`` type trait is true) may be used with |poly_ptr|.
      If deep-copy semantics with a ``std::unique_ptr`` are desired for a
      non-polymorphic type, use |deep_ptr| instead.

   The |poly_ptr| is *not* polymorphic itself and is marked ``final`` to
   prevent user inheritance.

   .. type:: unique_type

      Represents the ``std::unique_ptr`` used internally to hold the managed
      object.

   .. type:: element_type

      A type equivalent to the expression
      ``typename unique_type::element_type``

   .. type:: deleter_type

      A type equivalent to the expression
      ``typename unique_type::deleter_type``

   .. type:: copier_type

      A type used to represent the copy function used to perform deep copies.
      It has the type signature of: ``unique_type (*)(unique_type const&)``.

   .. type:: pointer

      A type equivalent to the expression ``typename unique_type::pointer``.

   .. function:: explicit poly_ptr(U* ptr)

      Takes a derived pointer to a given type *U*. *U* must be a non-abstract
      type and have :type:`element_type` as a base class within its inheritance
      tree.

   .. function:: poly_ptr (U*, E&&, copier_type=default_poly_copy)

      Takes some derived type *U*, a universal reference *E*, and an optional
      copier function pointer. The universal reference *E* is forwarded to the
      internal std::unique_ptr, where it will handle the proper rules required
      to initialize the deleter_type.

   .. function:: explicit poly_ptr (std::unique_ptr<U, E>&&, copier_type)

      A unique_ptr of type *U* and deleter *E*, with an optional copier_type
      parameter that is by default :func:`default_poly_copy\<T, D, U>`. As
      normal, *U* must have :type:`element_type` in its hierarchy.

   .. function:: poly_ptr (poly_ptr const& that)

      Performs a deep copy with the object managed by ``that``, if any such
      object exists.

   .. function:: poly_ptr (polymorphic&& that)

      Moves ``that``'s pointer and copier into ``*this``, and then sets
      ``that``'s copier to :func:`null_poly_copy\<T, D>`.

   .. function:: poly_ptr () noexcept

      The default constructor for a |poly_ptr| will place it into such a state
      that bool(|poly_ptr|) will return false;

   .. function:: operator = (std::unique_ptr<U, D>&& ptr)

      Calls ``poly_ptr<T, Deleter> { std::move(ptr) }.swap(*this)``

      :returns: ``*this``

   .. function:: operator = (poly_ptr<T, Deleter>&& that) noexcept

      Calls ``poly_ptr<T, Deleter> { std::move(that) }.swap(*this)``

      :returns: ``*this``

   .. function:: operator = (poly_ptr<T, Deleter> const& that)

      Performs a deep copy with the object managed by ``that``, if such
      an object exists.

      :returns: ``*this``

   .. function:: operator bool () const noexcept

      .. note:: This cast operator is marked as explicit.

      :returns: Whether ``*this`` owns an object

   .. function:: element_type& operator * () const

      :returns: an lvalue reference to the object owned by ``*this``.

   .. function:: pointer operator -> () const noexcept

      :returns: a pointer to the object owned by ``*this``

   .. function:: pointer get () const noexcept

      :returns: A pointer to the managed object, or ``nullptr`` if no such
                object exists.

   .. function:: deleter_type const& get_deleter () const noexcept
                 deleter_type& get_deleter () noexcept

      :returns: The deleter object used for destruction of the managed object.

   .. function:: copier_type const& get_copier () const noexcept
                 copier_type& get_copier () noexcept

      :returns: The function pointer used for copying the managed object.

   .. function:: pointer release () noexcept

      Releases the ownership of the managed object, if any such object exists.
      Any calls to :func:`poly_ptr\<T, Deleter>::get` will return ``nullptr``
      after this call.

      :returns: pointer to the managed object or ``nullptr`` if the |poly_ptr|
                did not manage an object.

   .. function:: void reset (pointer ptr = pointer { })

      Replaces the managed object. Performs the following actions (these
      differ from the order of operations followed by ``std::unique_ptr``).

      * If the incoming pointer is ``nullptr``, the order of operations
        follows those performed by ``std::unique_ptr``, along with the value
        returned by :func:`poly_ptr\<T, Deleter>::get_copier` being set to a
        null copier.
      * If the incoming pointer is *not* ``nullptr``, and there is no
        managed object, a :class:`bad_polymorphic_reset` exception is thrown.
      * If the incoming pointer is *not* ``nullptr``, a ``typeid`` comparison
        between the managed object and the incoming pointer is performed.
        If the ``std::type_info`` returned from both is not identical,
        a :class:`bad_polymorphic_reset` is thrown.
        If the ``std::type_info`` is identical, the order of operations
        follows those performed by ``std::unique_ptr``.

   .. function:: void swap(poly_ptr&) noexcept

      Swaps the managed object and copier function

.. class:: deep_ptr<T, Deleter, Copier>

   .. todo:: Fill out this section

.. class:: bad_polymorphic_reset

   :inherits: std::logic_error

   Thrown when a :func:`poly_ptr<T, Deleter>::reset` is passed a
   non-null pointer and the |poly_ptr| does no manage an object, or if the
   passed in pointer differs in type from the currently managed object.

.. class:: default_copy<T>

   .. todo:: fIll out this section

.. function:: std::unique_ptr<T, D> default_poly_copy<T, D, U> (\
              std::unique_ptr<T, D> const&)

   This function is used as the default copier when assigning a raw pointer or
   unique_ptr to a |poly_ptr|. It will perform a deep copy with a call to
   :func:`make_unique<T>`, with type *U* and dynamic_cast the stored pointer
   of *T* into *U* as it performs the assignment. The :type:`deleter_type` of
   the given unique_ptr will *also* be copied.

   :returns: ``std::unique_ptr<T, D>`` with a managed object.

.. function:: std::unique_ptr<T, D> null_poly_copy<T, D> (\
              std::unique_ptr<T, D> const&)

   This function is used within a |poly_ptr| for when it does not manage an
   object. Given any unique_ptr, it will return an empty unique_ptr.

   :returns: An empty ``std::unique_ptr<T, D>``

.. function:: bool operator == (poly_ptr<T, D> const&, nullptr_t) noexcept
              bool operator != (poly_ptr<T, D> const&, nullptr_t) noexcept
              bool operator >= (poly_ptr<T, D> const&, nullptr_t) noexcept
              bool operator <= (poly_ptr<T, D> const&, nullptr_t) noexcept
              bool operator >(poly_ptr<T, D> const&, nullptr_t) noexcept
              bool operator <(poly_ptr<T, D> const&, nullptr_t) noexcept
              bool operator == (nullptr_t, poly_ptr<T, D> const&) noexcept
              bool operator != (nullptr_t, poly_ptr<T, D> const&) noexcept
              bool operator >= (nullptr_t, poly_ptr<T, D> const&) noexcept
              bool operator <= (nullptr_t, poly_ptr<T, D> const&) noexcept
              bool operator >(nullptr_t, poly_ptr<T, D> const&) noexcept
              bool operator <(nullptr_t, poly_ptr<T, D> const&) noexcept

   :returns: the result of comparing :func:`poly_ptr\<T, Deleter>::get` and
             ``nullptr`` with the given operator.

.. function:: poly_ptr<T, Deleter> make_poly<T>(U&& args)

   Provided to supplement the ``std::make_shared<T>`` and
   :func:`make_unique\<T>` functions. Constructs a |poly_ptr| with an
   ``element_type`` of *T*, taking derived universal reference *U*. This
   function internally calls :func:`make_unique\<T>` to create the
   |poly_ptr|.

.. function:: std::unique_ptr<T> make_unique<T>(args)
              std::unique_ptr<T> make_unique<T>(size)

   ``make_unique`` is provided to help supplement the ``std::make_shared<T>``
   function for the ``std::unique_ptr<T>`` type. The first overload will be
   used if the given type T is not an array. If the given type T is an array of
   an unknown bound (that is, ``std::extent<T>::value == 0``) the second
   overload is used. A third overload is provided to insure that the compiler

   will error. This third overload is available when the given type
   T is an array of a known bound (that is,
   ``std::extent<T>::value != 0``).

   :param args: Variadic template arguments with which to construct
                a T
   :type args: Args&&...
   :returns: ``std::unique_ptr<T>``

   :param size: Extent of ``std::unique_ptr<T[]>`` desired.
   :type size: std::size_t
   :returns: ``std::unique_ptr<T[]>``

Specializations
---------------

There are specializations for |poly_ptr| and |deep_ptr| for integration with
the C++ standard library.

.. namespace:: std

.. class:: hash<poly_ptr<T, Deleter>>

   This specialization of :class:`hash` allows |poly_ptr| to be used as a
   key type in associative containers.

   For a given |poly_ptr| *ptr*, this specialization insures that
   ``std::hash<poly_ptr<T, Deleter>> { }(ptr)`` is equivalent to the expression
   ``std::hash<typename poly_ptr<T, Deleter>::pointer> { }(ptr.get())``

.. class:: std::hash<deep_ptr<T, Deleter, Copier>>

   This specialization of :class:`hash` allows |deep_ptr| to be used as a
   key type in associative containers.

   For a given |deep_ptr| *ptr*, this specialization insure that
   ``std::hash<deep_ptr<T, Deleter, Copier>> { }(ptr)`` is equivalent to the
   expression
   ``std::hash<typename deep_ptr<T, Deleter, Copier>::pointer> { }(ptr.get())``

.. function:: void swap<T, D>(poly_ptr<T, D>& lhs, poly_ptr<T, D>& rhs) \
              noexcept

   A specialization of ``std::swap`` that calls
   :func:`poly_ptr<T, Deleter>::swap`.

