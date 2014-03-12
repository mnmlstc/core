.. _core-memory-component:

Memory Component
================

.. default-domain:: cpp

This section discusses the memory component of MNMLSTC Core. Contained in this
component are three new smart pointers (two with deep copy semantics) and a
C++11 equivalent to C++14's :func:`make_unique\<T>`.

.. |observer_ptr| replace:: :class:`observer_ptr <observer_ptr\<T>>`
.. |poly_ptr| replace:: :class:`poly_ptr <poly_ptr\<T, Deleter>>`
.. |deep_ptr| replace:: :class:`deep_ptr <deep_ptr\<T, Deleter, Copier>>`

.. namespace:: core

Polymorphic Smart Pointer
-------------------------

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

Deep Copying Smart Pointer
--------------------------

.. class:: deep_ptr<T, Deleter, Copier>

   |deep_ptr| is a smart pointer for a type that retains sole ownership of the
   pointer it manages and performs a *deep copy* on assignment or copy
   construction. |deep_ptr| is much like ``std::unique_ptr`` with deep-copy
   semantics. Unlike |poly_ptr|, |deep_ptr| is for concrete types where
   polymorphism is not desired. |poly_ptr| has *some* storage overhead for
   copying a polymorphic type, however |deep_ptr| performs the same
   optimization as ``std::unique_ptr`` in that it is only ``sizeof(T*)``,
   unless the given Deleter and Copier types hold state.

   With the exception of the copy assignment and copy constructor, |deep_ptr|
   has an interface identical to that of ``std::unique_ptr``, and exhibits the
   same behavior as ``std::unique_ptr``

   If the result of the :type:`copier_type` differs from :type:`pointer`, the
   program will be malformed, and a static assertion will cause a compiler
   error.

   .. type:: element_type

      The type of object managed by the |deep_ptr|.

   .. type:: deleter_type

      The deleter object used to destroy and deallocate the object managed by
      the |deep_ptr|.

   .. type:: copier_type

      The copier object used to perform an allocation and deep copy the object
      managed by |deep_ptr|.

   .. type:: pointer

      ``remove_reference_t<deleter_type>::pointer`` if the type exists,
      otherwise, ``element_type*``.

   .. function:: deep_ptr (pointer ptr, E&& deleter, C&& copier) noexcept

      Actually two separate constructors, these follow the behavior of the
      ``std::unique_ptr`` constructors that take a pointer, and deleter object.
      The behavior extends to the type desired for the copier object as well.

   .. function:: deep_ptr (std::unique_ptr<U, E>&&) noexcept

      Constructs a |deep_ptr| with the contents of the unique_ptr. The given
      type *U* must be a pointer convertible to :type:`pointer`, and *E* must
      be a type that can construct a :type:`deleter_type`.

   .. function:: explicit deep_ptr (pointer ptr) noexcept

      Constructs a |deep_ptr| with the default deleter, default copier, and the
      given pointer. The |deep_ptr| assumes ownership of *ptr*.

   .. function:: deep_ptr (std::nullptr_t) noexcept

      Delegates construction of the |deep_ptr| to the 
      :ref:`default constructor <deep-ptr-default-constructor>`.

   .. function:: deep_ptr (deep_ptr const& that)

      Constructs a new object to be managed via *that*'s object.

   .. function:: deep_ptr (deep_ptr&& that) noexcept

      Constructs a |deep_ptr| with the managed object, deleter, and copier of
      *that* via move construction.

      :postcondition: *that* is empty

   .. _deep-ptr-default-constructor:

   .. function:: constexpr deep_ptr () noexcept

      Default constructs a |deep_ptr| into an empty state.

   .. function:: deep_ptr& operator = (std::unique_ptr<U, D>&& ptr) noexcept

      Assigns the contents of *ptr* to ``*this``
 
   .. function:: deep_ptr& operator = (deep_ptr const&) noexcept
                 deep_ptr& operator = (deep_ptr&&) noexcept

      Assigns the contents of the incoming |deep_ptr| to ``*this``

   .. function:: deep_ptr& operator = (std::nullptr_t) noexcept

      Resets the |deep_ptr| and the object it manages.

   .. function:: operator bool () const noexcept

      .. note:: This cast operator is marked as explicit

      :returns: Whether the |deep_ptr| manages an object

   .. function:: element_type& operator * () const

      Attempting to dereference a |deep_ptr| that does not manage an object
      will result in undefined behavior

      :returns: an lvalue reference to the managed object

   .. function:: pointer operator -> () const noexcept

      :returns: a pointer to the managed object or ``nullptr`` if no such
                object exists.

   .. function:: pointer get () const noexcept

      :returns: A pointer to the managed object, or ``nullptr`` if no such
                object exists.

   .. function:: deleter_type const& get_deleter () const noexcept
                 deleter_type& get_deleter () noexcept

      :returns: The deleter object used for destruction of the managed object.

   .. function:: copier_type const& get_copier () const noexcept
                 copier_type& get_copier () noexcept

      :returns: The copier object used for copying the managed object.

   .. function:: pointer release () noexcept

      :postcondition: :func:`deep_ptr\<T, Deleter, Copier>::get` returns \
                      ``nullptr``

      Releases the ownership of the managed object, if any such object exists.

   .. function:: void reset (pointer ptr = pointer { })

      Replaces the currently managed object with *ptr*.

   .. function:: void swap(deep_ptr&) noexcept

      Swaps the managed object, copier object, and deleter object.


Dumbest Smart Pointer
---------------------

.. class:: observer_ptr<T>

   |observer_ptr| is "the dumbest smart pointer", in that it is only ever used
   in the place of a raw pointer. The idea is to inform the user that the
   |observer_ptr| does not *own* the pointer it *watches*. It can be treated
   like a raw pointer, except that there is no need to read the documentation
   to see if the user needs to manage a raw pointer or not. Because the
   |observer_ptr| is a non-owning smart pointer, the need for a move
   constructor and assignment operator is superfluous as copying a pointer
   is just as cheap as moving one.

   .. type:: element_type

      The type of the object managed by |observer_ptr|.

   .. type:: const_pointer
             pointer

      ``add_pointer_t<add_const_t<element_type>`` and
      ``add_pointer_t<element_type>`` respectively.

   .. type:: const_reference
             reference

      ``add_lvalue_reference<add_const_t<element_type>`` and
      ``add_lvalue_reference<element_type>`` respectively.

   .. function:: observer_ptr (std::nullptr_t ptr)
                 observer_ptr (pointer ptr)
                 observer_ptr (add_pointer_t<T> ptr)

      Constructs the |observer_ptr| with the given pointer. If *ptr* is
      convertible to :type:`observer_ptr\<T>::pointer`, it will construct it
      that way (via a dynamic_cast).

   .. function:: void swap (observer_ptr<T>&)

      Swaps the contents of the |observer_ptr| with the other.

   .. function:: operator const_pointer () const
                 operator pointer ()

      :noexcept: true
      :explicit: Yes

      Allows an |observer_ptr| to be explicitly converted to
      :type:`observer_ptr\<T>::const_pointer` or
      :type:`observer_ptr\<T>::pointer` respectively.

   .. function:: operator bool () const

      :noexcept: true
      :explicit: Yes

      Allows the |observer_ptr| to be explicitly converted to a boolean.

   .. function:: reference operator * () const

      :noexcept: true
      :returns: reference to the object watched by the |observer_ptr|.

   .. function:: pointer operator -> () const

      :noexcept: true
      :returns: the object watched by the |observer_ptr|

   .. function:: pointer get () const

      :noexcept: true
      :returns: The object watched by the |observer_ptr|

   .. function:: pointer release () noexcept

      :noexcept: true
      :returns: the object watched by the |observer_ptr|. The |observer_ptr| is
                then set to ``nullptr``.

   .. function:: void reset (pointer ptr=nullptr)

      :noexcept: true

      Resets the object watched by the |observer_ptr| with *ptr*.

Utilities
---------

.. class:: bad_polymorphic_reset

   :inherits: std::logic_error

   Thrown when a :func:`poly_ptr<T, Deleter>::reset` is passed a
   non-null pointer and the |poly_ptr| does not manage an object, or if the
   passed in pointer differs in type from the currently managed object.

.. class:: default_copy<T>

   The default copy policy used by |deep_ptr| during a copy operation. There
   are no partial specializations available. The default operation to perform
   is to allocate a new *T* pointer with ``operator new``, and to initialize
   this *T* with a ``T const&``.

   .. type:: pointer

      Represents ``T*``

   .. function:: constexpr default_copy ()

      Constructs the :class:`default_copy\<T>` object.

   .. function:: default_copy (default_copy<U> const&) noexcept

      Constructs a :class:`default_copy\<T>` from another
      :class:`default_copy\<T>`.

   .. function:: pointer operator () (pointer const ptr)

      Allocates a new :type:`pointer` and initializes it with the dereferenced
      *ptr*, to invoke the copy constructor.

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

Comparison Operators
--------------------

.. function:: bool operator == (poly_ptr const&, poly_ptr const&) noexcept
              bool operator != (poly_ptr const&, poly_ptr const&) noexcept
              bool operator >= (poly_ptr const&, poly_ptr const&) noexcept
              bool operator <= (poly_ptr const&, poly_ptr const&) noexcept
              bool operator > (poly_ptr const&, poly_ptr const&) noexcept
              bool operator < (poly_ptr const&, poly_ptr const&) noexcept

   Compares two |poly_ptr|'s via :func:`poly_ptr\<T, Deleter>::get` with
   the given operator.

.. function:: bool operator == (deep_ptr const&, deep_ptr const&) noexcept
              bool operator != (deep_ptr const&, deep_ptr const&) noexcept
              bool operator >= (deep_ptr const&, deep_ptr const&) noexcept
              bool operator <= (deep_ptr const&, deep_ptr const&) noexcept
              bool operator > (deep_ptr const&, deep_ptr const&) noexcept
              bool operator < (deep_ptr const&, deep_ptr const&) noexcept

   Compares two |deep_ptr|'s via :func:`deep_ptr\<T, Deleter>::get` with
   the given operator.

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

.. function:: bool operator == (deep_ptr<T, D, C> const&, nullptr_t) noexcept
              bool operator != (deep_ptr<T, D, C> const&, nullptr_t) noexcept
              bool operator >= (deep_ptr<T, D, C> const&, nullptr_t) noexcept
              bool operator <= (deep_ptr<T, D, C> const&, nullptr_t) noexcept
              bool operator > (deep_ptr<T, D, C> const&, nullptr_t) noexcept
              bool operator < (deep_ptr<T, D, C> const&, nullptr_t) noexcept
              bool operator == (nullptr_t, deep_ptr<T, D, C> const&) noexcept
              bool operator != (nullptr_t, deep_ptr<T, D, C> const&) noexcept
              bool operator >= (nullptr_t, deep_ptr<T, D, C> const&) noexcept
              bool operator <= (nullptr_t, deep_ptr<T, D, C> const&) noexcept
              bool operator > (nullptr_t, deep_ptr<T, D, C> const&) noexcept
              bool operator < (nullptr_t, deep_ptr<T, D, C> const&) noexcept

   :returns: The result of comparing :func:`deep_ptr\<T, Deleter, Copier>::get`
             and ``nullptr`` with the given operator.

.. function:: bool operator == (observer_ptr const&, observer_ptr const&)
              bool operator != (observer_ptr const&, observer_ptr const&)
              bool operator >= (observer_ptr const&, observer_ptr const&)
              bool operator <= (observer_ptr const&, observer_ptr const&)
              bool operator  > (observer_ptr const&, observer_ptr const&)
              bool operator  < (observer_ptr const&, observer_ptr const&)

   :returns: The result of comparing the objects watched by |observer_ptr| via
             the given operator.

.. function:: bool operator == (observer_ptr const&, std::nullptr_t)
              bool operator != (observer_ptr const&, std::nullptr_t)
              bool operator == (std::nullptr_t, observer_const&)
              bool operator != (std::nullptr_t, observer_const&)

   :returns: The result of comparing the objects watched by |observer_ptr| with
             ``nullptr`` via the given operator

Make Functions
--------------

.. function:: observer_ptr<T> make_observer(W* ptr)
              observer_ptr<T> make_observer(std::unique_ptr<W, D> const& ptr)
              observer_ptr<T> make_observer(std::shared_ptr<W> const& ptr)
              observer_ptr<T> make_observer(std::weak_ptr<W> const& ptr)
              observer_ptr<T> make_observer(deep_ptr<W, C, D> const& ptr)
              observer_ptr<T> make_observer(poly_ptr<W, D> const& ptr)

   Provided to supplement the other ``make_*`` functions for smart pointers,
   the make_observer function will create an observer from any C++11 standard
   smart pointer, a raw pointer, or the smart pointers provided by MNMLSTC Core

.. function:: poly_ptr<T, Deleter> make_poly<T>(U&& args)

   Provided to supplement the ``std::make_shared<T>`` and
   :func:`make_unique\<T>` functions. Constructs a |poly_ptr| with an
   ``element_type`` of *T*, taking derived universal reference *U*. This
   function internally calls :func:`make_unique\<T>` to create the
   |poly_ptr|.

.. function:: deep_ptr<T> make_deep<T>(args)

   Used to supplement the :func:`make_unique\<T>`, :func:`make_poly\<T>`,
   and `make_shared<T>` functions. Takes a variadic number of arguments to
   construct a *T* with. This *T* is allocated via operator new (the default
   allocation scheme) and passed to a |deep_ptr| for construction. This
   |deep_ptr| is then returned by the function.

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

.. function:: void swap(poly_ptr<T, D>& lhs, poly_ptr<T, D>& rhs) \
              noexcept

   A specialization of ``std::swap`` that calls
   :func:`poly_ptr<T, Deleter>::swap`.

.. function:: void swap(deep_ptr<T, D, C>& lhs, deep_ptr<T, D, C>& rhs) \
              noexcept

   A specialization of ``std::swap`` that calls
   :func:`deep_ptr<T, Deleter, Copier>::swap`.

