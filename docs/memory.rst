Memory Component
================

.. namespace:: core

.. index:: memory

This section discusses the memory component of MNMLSTC Core. Contained in this
component are three new smart pointers (two with deep copy semantics) and a
C++11 equivalent to C++14's :any:`make_unique`.

Everything discussed in this section resides in the :file:`<core/{memory}.hpp>`
header.

.. |observer_ptr| replace:: :class:`observer_ptr <core::observer_ptr<T>>`
.. |poly_ptr| replace:: :class:`poly_ptr <core::poly_ptr<T, Deleter>>`
.. |deep_ptr| replace:: :class:`deep_ptr <core::deep_ptr<T, Deleter, Copier>>`

Polymorphic Smart Pointer
-------------------------

.. index::
   single: memory; poly_ptr
   single: smart pointers; poly_ptr
   single: poly_ptr

.. class:: template <class T, class Deleter> poly_ptr

   .. deprecated:: 2.0

   The :any:`poly_ptr` is a smart pointer for polymorphic types that
   retains sole ownership of the *polymorphic* type :samp:`{T}` and performs
   *deep copies* on assignment or copy construction.  This use is extremely
   close to the :any:`any`, however it affords the user the ability to
   ascertain what possible types the :any:`poly_ptr` has while also allowing
   custom allocation, copying, and deallocation.  In this way,
   a :any:`poly_ptr` can be seen as a :cxx:`std::unique_ptr` with deep-copy
   semantics, and type erasure. :any:`poly_ptr` has an interface equivalent to
   :cxx:`std::unique_ptr`. Due to the type erasure that is used, the size of a
   :cxx:`poly_ptr` is
   :samp:`sizeof(std::unique_ptr<{T}, {Deleter}> + sizeof(function-ptr)`.

   .. note:: Due to the lack of polymorphic allocators in C++, custom memory
      allocation is currently relegated to performing allocation within a
      user-supplied copier function. Because the copier function is a function
      pointer this means that :any:`poly_ptr` is restricted to stateless
      lambdas or function pointers.

   The :any:`poly_ptr` requires that RTTI and exceptions be enabled. Because of
   the type-erasure used, RTTI is absolutely required to for the polymorphic
   deep-copy to take place.

   *Only* a polymorphic type (that is, any type where the
   :cxx:`std::is_polymorphic` type trait is true) may be used with 
   :any:`poly_ptr`. If deep-copy semantics with a :cxx:`std::unique_ptr` are
   desired for a non-polymorphic type, use :any:`deep_ptr` instead.

   The :any:`poly_ptr` is *not* polymorphic itself and is marked ``final`` to
   prevent user inheritance.

   This type is not available if :c:macro:`CORE_NO_RTTI` is defined.

   .. index:: poly_ptr; type aliases

   .. type:: unique_type
   
      Represents the :cxx:`std::unique_ptr` used internally to hold the managed
      object.
   
   .. type:: element_type
   
      A type equivalent to the expression
      :samp:`typename {unique_type}::element_type`
   
   .. type:: deleter_type
   
      A type equivalent to the expression
      :samp:`typename {unique_type}::deleter_type`
   
   .. type:: copier_type
   
      Represents a copy function used to perform deep copies. Has the
      type signature :samp:`{unique_type} (*)({unique_type} const&)`

   .. type:: pointer
   
      A type equivalent to the expression
      :samp:`typename {unique_type}::pointer`.
   
   .. function:: explicit poly_ptr(U* ptr)

      Takes a derived pointer to a given type :samp:`{U}`. :samp:`{U}` must be
      a non-abstract type and have :any:`element_type` as a base class within
      its inheritance tree.

   .. function:: poly_ptr (U* ptr, E&& deleter, copier_type=default_poly_copy)

      Takes some derived type :samp:`{U}`, a forwarding reference :samp:`{E}`,
      and an optional copier function pointer. The universal reference
      :samp:`{E}` is forwarded to the internal :cxx:`std::unique_ptr`, where it
      will handle the proper rules required to initialize the deleter_type.

   .. function:: explicit poly_ptr (            \
                   std::unique_ptr<U, E>&& ptr, \
                   copier_type c=default_poly_copy)

      A unique_ptr of type :samp:`{U}` and deleter :samp:`{E}, with an optional
      :any:`copier_type` parameter that is by default :any:`default_poly_copy`.
      As usual, :samp:`{U}` must have :any:`element_type` in its parent
      hierarchy.

   .. function:: poly_ptr (poly_ptr const& that)

      Performs a deep copy with the object managed by :samp:`{that}`, if any
      such object exists.

   .. function:: poly_ptr (polymorphic&& that)

      Moves :samp:`{that}`'s pointer and copier into the :any:`poly_ptr`, and
      then sets :samp:`{that}`'s copier to :any:`null_poly_copy`.

   .. function:: poly_ptr () noexcept

      The default constructor for a :any:`poly_ptr` will place it into such a
      state that :any:`operator bool` will return false;

   .. function:: poly_ptr& operator = (std::unique_ptr<U, D>&& ptr)

      Assigns the contents of :samp:`{ptr}` to the :any:`poly_ptr`.

      :returns: The same :any:`poly_ptr` that was assigned to.

   .. function:: poly_ptr& operator = (poly_ptr&& that) noexcept

      Moves the pointer, deleter, and copier function of that :samp:`{that}`
      into the :any:`poly_ptr`.

      :returns: The same :any:`poly_ptr` that was assigned to.

   .. function:: poly_ptr& operator = (poly_ptr const& that)

      Performs a deep copy with the object managed by :samp:`{that}`, if such
      an object exists. Also copies the deleter and copier function of
      :samp:`{that}`

      :returns: The same :any:`poly_ptr` that was assigned to.

   .. function:: explicit operator bool () const noexcept

      :returns: Whether the :any:`poly_ptr` manages an object.

   .. function:: element_type& operator * () const

      :returns: an lvalue reference to the object managed by :any:`poly_ptr`.

   .. function:: pointer operator -> () const noexcept

      :returns: a pointer to the object managed by the :any:`poly_ptr`

   .. function:: pointer get () const noexcept

      :returns: A pointer to the managed object, or :cxx:`nullptr` if no such
                object exists.

   .. function:: deleter_type const& get_deleter () const noexcept
                 deleter_type& get_deleter () noexcept

      :returns: The deleter object used for destruction of the managed object.

   .. function:: copier_type const& get_copier () const noexcept
                 copier_type& get_copier () noexcept
   
      :returns: The function pointer used for copying the managed object.
   
   .. function:: pointer release () noexcept
   
      Releases the ownership of the managed object, if any such object exists.
      Any calls to :any:`get` will return :cxx:`nullptr` after this call.
   
      :returns: pointer to the managed object or `nullptr` if the
                :any:`poly_ptr` did not manage an object.

   .. function:: void reset (pointer ptr = pointer { })
   
      Replaces the managed object. Performs the following actions (these
      differ from the order of operations followed by :cxx:`std::unique_ptr`).
   
      * If the incoming pointer is :cxx:`nullptr`, the order of operations
        follows those performed by :cxx:`std::unique_ptr`, along with the value
        returned by :any:`get_copier` being set to a null copier.
      * If the incoming pointer is *not* :cxx:`nullptr`, and there is no
        managed object, a :any:`bad_polymorphic_reset` exception is thrown.
      * If the incoming pointer is *not* :cxx:`nullptr`, a :cxx:`typeid`
        comparison between the managed object and the incoming pointer is
        performed. If the :cxx:`std::type_info` returned from both is not
        identical, a :any:`bad_polymorphic_reset` is thrown.
        If the :cxx:`std::type_info` is identical, the order of operations
        follows those performed by :cxx:`std::unique_ptr`.
   
   .. function:: void swap (poly_ptr& that) noexcept
   
      Swaps the managed object and copier function of the :any:`poly_ptr` with
      the managed object and copier function of :samp:`{that}`.

Deep Copying Smart Pointer
--------------------------

.. class:: template <class T, class Deleter, class Copier> deep_ptr

   :deprecated: 2.0

   :any:`deep_ptr` is a smart pointer for a type that retains sole ownership of
   the pointer it manages and performs a *deep copy* on assignment or copy
   construction. :any:`deep_ptr` is much like :cxx:`std::unique_ptr` with
   deep-copy semantics. Unlike :any:`poly_ptr`, :any:`deep_ptr` is for concrete
   types where polymorphism is not desired. :any:`poly_ptr` has *some* storage
   overhead for copying a polymorphic type, however :any:`deep_ptr` performs
   the same optimization as :cxx:`std::unique_ptr` in that it is only
   :samp:`sizeof({T}*)`, unless the given :samp:`{Deleter}` and
   :samp:`{Copier}` types hold state.

   With the exception of the copy assignment and copy constructor,
   :any:`deep_ptr` has an interface identical to that of
   :cxx:`std::unique_ptr`, and exhibits the same behavior as
   :cxx:`std::unique_ptr`

   If the result of the :any:`copier_type` differs from :any:`pointer`, the
   program will be malformed, and a static assertion will cause a compiler
   error.

   .. type:: element_type
   
      The type of object managed by the :any:`deep_ptr`.

   .. type:: deleter_type

      The deleter object used to destroy and deallocate the object managed by
      the :any:`deep_ptr`.

   .. type:: copier_type

      The copier object used to perform an allocation and deep copy the object
      managed by :any:`deep_ptr`.

   .. type:: pointer
   
      :samp:`remove_reference_t<{deleter_type}>::pointer` if the type exists,
      otherwise, :samp:`{element_type}*`.

   .. function:: deep_ptr (pointer ptr, E&& deleter, C&& copier) noexcept

      Actually two separate constructors, these follow the behavior of the
      :cxx:`std::unique_ptr` constructors that take a pointer, and deleter
      object. The behavior extends to the type desired for the copier object as
      well.

   .. function:: deep_ptr (std::unique_ptr<U, E>&& ptr) noexcept

      Constructs a :any:`deep_ptr` with the contents of the unique_ptr. The
      given type :samp:`{U}` must be a pointer convertible to :any:`pointer`,
      and :samp:`{E}` must be a type that can construct a :any:`deleter_type`.

   .. function:: explicit deep_ptr (pointer ptr) noexcept

      Constructs a :any:`deep_ptr` with the default deleter, default copier,
      and the given pointer. The :any:`deep_ptr` assumes ownership of
      :samp:`{ptr}`.

   .. function:: deep_ptr (std::nullptr_t) noexcept

      Delegates construction of the :any:`deep_ptr` to the 
      :ref:`default constructor <deep-ptr-default-constructor>`.

   .. function:: deep_ptr (deep_ptr const& that)

      Constructs a new object to be managed via :samp:`{that}`'s object.

   .. function:: deep_ptr (deep_ptr&& that) noexcept

      Constructs a :any:`deep_ptr` with the managed object, deleter, and copier
      of :samp:`{that}` via move construction.

      :postcondition: :samp:`{that}` is empty

   .. _deep-ptr-default-constructor:
   .. function:: constexpr deep_ptr () noexcept

      Default constructs a :any:`deep_ptr` into an empty state.

   .. function:: deep_ptr& operator = (std::unique_ptr<U, D>&& ptr) noexcept
   
         Assigns the contents of :samp:`{ptr}` to the :any:`deep_ptr`.
 
   .. function:: deep_ptr& operator = (deep_ptr const& that) noexcept
                 deep_ptr& operator = (deep_ptr&& that) noexcept

      Assigns the contents of :samp:`{that}` to :any:`deep_ptr`.

   .. function:: deep_ptr& operator = (std::nullptr_t) noexcept

      Resets the :any:`deep_ptr` and the object it manages.

   .. function:: explicit operator bool () const noexcept
   
      :returns: Whether the :any:`deep_ptr` manages an object
   
   .. function:: element_type& operator * () const
   
      Attempting to dereference a :any:`deep_ptr` that does not manage an
      object will result in undefined behavior
   
      :returns: an lvalue reference to the managed object
   
   .. function:: pointer operator -> () const noexcept
   
      :returns: a pointer to the managed object or :cxx:`nullptr` if no such
                object exists.
   
   .. function:: pointer get () const noexcept

      :returns: A pointer to the managed object, or :cxx:`nullptr` if no such
                object exists.

   .. function:: deleter_type const& get_deleter () const noexcept
                 deleter_type& get_deleter () noexcept

      :returns: The deleter object used for destruction of the managed object.

   .. function:: copier_type const& get_copier () const noexcept
                 copier_type& get_copier () noexcept

      :returns: The copier object used for copying the managed object.

   .. function:: pointer release () noexcept

      :postcondition: :any:`get` returns :cxx:`nullptr`

      Releases the ownership of the managed object, if any such object exists.

   .. function:: void reset (pointer ptr = pointer { })

      Replaces the currently managed object with :samp:`{ptr}`.

   .. function:: void swap(deep_ptr& that) noexcept

      Swaps the managed object, copier object, and deleter object of
      :samp:`{that}` with the :any:`deep_ptr`


Dumbest Smart Pointer
---------------------

.. class:: template <class T> observer_ptr

   :any:`observer_ptr` is "the dumbest smart pointer", in that it is only ever
   used in the place of a raw pointer. The idea is to inform the user that the
   :any:`observer_ptr` does not *own* the pointer it *watches*. It can be
   treated like a raw pointer, except that there is no need to read the
   documentation to see if the user needs to manage a raw pointer or not.
   Because the :any:`observer_ptr` is a non-owning smart pointer, the need for
   a move constructor and assignment operator is superfluous as copying a
   pointer is just as cheap as moving one.

   .. type:: element_type

      The type of the object managed by :any:`observer_ptr`.

   .. type:: const_pointer
             pointer
   
      :samp:`add_pointer_t<add_const_t<{element_type}>` and
      :samp:`add_pointer_t<{element_type}>` respectively.

   .. type:: const_reference
             reference

      :samp:`add_lvalue_reference<add_const_t<{element_type}>` and
      :samp:`add_lvalue_reference<{element_type}>` respectively.

   .. function:: observer_ptr (std::nullptr_t ptr) noexcept
                 observer_ptr (pointer ptr) noexcept
                 observer_ptr (add_pointer_t<T> ptr) noexcept

      Constructs the :any:`observer_ptr` with the given pointer. If
      :samp:`{ptr}` is convertible to :any:`pointer`, it will construct it
      that way (via a :cxx:`dynamic_cast`).

   .. function:: void swap (observer_ptr<T>& that) noexcept

      Swaps the pointer observed by :samp:`{that}` with the pointer observed
      by :any:`observer_ptr`.

   .. function:: explicit operator const_pointer () const noexcept
                 explicit operator pointer () noexcept

      Allows an :any:`observer_ptr` to be explicitly converted to
      :any:`const_pointer` or :any:`pointer` respectively.

   .. function:: explicit operator bool () const noexcept

      Allows the :any:`observer_ptr` to be explicitly converted to a boolean.

   .. function:: reference operator * () const noexcept

      :returns: reference to the object watched by the :any:`observer_ptr`.

   .. function:: pointer operator -> () const noexcept

      :returns: the object watched by the :any:`observer_ptr`

   .. function:: pointer get () const noexcept

      :returns: The object watched by the :any:`observer_ptr`

   .. function:: pointer release () noexcept

      :returns: the object watched by the :any:`observer_ptr`. The
                :any:`observer_ptr` is then set to :cxx:`nullptr`.

   .. function:: void reset (pointer ptr=pointer { }) noexcept

      Resets the object watched by the :any:`observer_ptr` with :samp:`{ptr}`.

Custom Allocators
-----------------

.. class:: template <class T, size_t N> arena_allocator

   The :any:`arena_allocator` type fulfills an Allocator capable interface
   that allows stack allocation to reduce the cost of accessing the free
   store for short lived objects.

Utilities
---------

.. function:: void swap(poly_ptr<T, D>& lhs, poly_ptr<T, D>& rhs) noexcept

   Provided for ADL calls. Equivalent to calling :samp:`{lhs}.swap({rhs})`.

.. function:: void swap(deep_ptr<T, D, C>& l, deep_ptr<T, D, C>& r) noexcept

   Provided for ADL calls. Equivalent to calling :samp:`{l}.swap({r})`.

.. function:: void swap (observer_ptr<T>& lhs, observer_ptr<T>& rhs) noexcept

   Provided for ADL calls. Equivalent to calling :samp:`{lhs}.swap({rhs})`.

.. class:: bad_polymorphic_reset

   :inherits: std::logic_error

   Thrown when a :func:`poly_ptr<T, Deleter>::reset` is passed a
   non-null pointer and the :any:`poly_ptr` does not manage an object, or if
   the passed in pointer differs in type from the currently managed object.

.. class:: template <class T> default_copy

   The default copy policy used by :any:`deep_ptr` during a copy operation.
   There are no partial specializations available. The default operation to
   perform is to allocate a new :samp:`{T}` pointer with :cxx:`operator new`,
   and to initialize this :samp:`{T}` with a :samp:`{T} const&`.

   .. type:: pointer

      Represents :samp:`{T}*`

   .. function:: constexpr default_copy ()

      Constructs the :any:`default_copy` object.

   .. function:: default_copy (default_copy<U> const& that) noexcept

      Constructs a :any:`default_copy` from :samp:`{that}`.

   .. function:: pointer operator () (pointer const ptr)

      Allocates a new :any:`pointer` and initializes it with the dereferenced
      :samp:`{ptr}`, to invoke the copy constructor.

.. function:: template <class T, class D, class U> \
              unique_ptr<T, D> default_poly_copy (unique_ptr<T, D> const&)

   This function is used as the default copier when assigning a raw pointer or
   unique_ptr to a :any:`poly_ptr`. It will perform a deep copy with a call to
   :any:`make_unique`, with type :samp:`{U}` and dynamic_cast the stored
   pointer of :samp:`{T}` into :samp:`{U}` as it performs the assignment. The
   :any:`deleter_type` of the given :cxx:`unique_ptr` will *also* be copied.

   :returns: :cxx:`std::unique_ptr<T, D>` with a managed object.

.. function:: template <class T, class D> \
              unique_ptr<T, D> null_poly_copy (unique_ptr<T, D> const&)

   This function is used within a :any:`poly_ptr` for when it does not manage
   an object. Given any :cxx:`std::unique_ptr`, it will return an empty
   :cxx:`std::unique_ptr`.

   :returns: An empty :cxx:`std::unique_ptr<T, D>`

Comparison Operators
--------------------

.. todo:: Fill out each function entry

.. function:: bool operator == (poly_ptr const&, poly_ptr const&) noexcept
              bool operator != (poly_ptr const&, poly_ptr const&) noexcept
              bool operator >= (poly_ptr const&, poly_ptr const&) noexcept
              bool operator <= (poly_ptr const&, poly_ptr const&) noexcept
              bool operator > (poly_ptr const&, poly_ptr const&) noexcept
              bool operator < (poly_ptr const&, poly_ptr const&) noexcept

   Compares two :any:`poly_ptr`'s via :any:`~core::poly_ptr\<T, Deleter>::get`
   with the given operator.

.. function:: bool operator == (deep_ptr const&, deep_ptr const&) noexcept
              bool operator != (deep_ptr const&, deep_ptr const&) noexcept
              bool operator >= (deep_ptr const&, deep_ptr const&) noexcept
              bool operator <= (deep_ptr const&, deep_ptr const&) noexcept
              bool operator > (deep_ptr const&, deep_ptr const&) noexcept
              bool operator < (deep_ptr const&, deep_ptr const&) noexcept

   Compares two :any:`deep_ptr`'s via
   :any:`~core::deep_ptr\<T, Deleter, Copier>::get` with
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

.. function:: bool operator == (deep_ptr const&, nullptr_t) noexcept
              bool operator != (deep_ptr const&, nullptr_t) noexcept
              bool operator >= (deep_ptr const&, nullptr_t) noexcept
              bool operator <= (deep_ptr const&, nullptr_t) noexcept
              bool operator > (deep_ptr const&, nullptr_t) noexcept
              bool operator < (deep_ptr const&, nullptr_t) noexcept
              bool operator == (nullptr_t, deep_ptr const&) noexcept
              bool operator != (nullptr_t, deep_ptr const&) noexcept
              bool operator >= (nullptr_t, deep_ptr const&) noexcept
              bool operator <= (nullptr_t, deep_ptr const&) noexcept
              bool operator > (nullptr_t, deep_ptr const&) noexcept
              bool operator < (nullptr_t, deep_ptr const&) noexcept

   :returns: The result of comparing the result of
             :any:`~deep_ptr\<T, Deleter, Copier>::get` and :cxx:`nullptr` with
             the given operator.

.. function:: bool operator == (observer_ptr const&, observer_ptr const&)
              bool operator != (observer_ptr const&, observer_ptr const&)
              bool operator >= (observer_ptr const&, observer_ptr const&)
              bool operator <= (observer_ptr const&, observer_ptr const&)
              bool operator  > (observer_ptr const&, observer_ptr const&)
              bool operator  < (observer_ptr const&, observer_ptr const&)

   :returns: The result of comparing the objects watched by :any:`observer_ptr`
             via the given operator.

.. function:: bool operator == (observer_ptr const&, std::nullptr_t)
              bool operator != (observer_ptr const&, std::nullptr_t)
              bool operator == (std::nullptr_t, observer_const&)
              bool operator != (std::nullptr_t, observer_const&)

   :returns: The result of comparing the objects watched by :any:`observer_ptr`
             with :cxx:`nullptr` via the given operator

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

.. function:: template <class T, class D> poly_ptr<T, D> make_poly(U&& args)

   Provided to supplement the :cxx:`std::make_shared<T>`` and
   :any:`make_unique` functions. Constructs a :any:`poly_ptr` with an
   :any:`~poly_ptr\<T, Deleter>::element_type` of :samp:`{T}`, taking a derived
   forwarding reference :samp:`{U}`. This function internally calls
   :any:`make_unique` to create the :any:`poly_ptr`.

.. function:: template <class T> deep_ptr<T> make_deep(Args&&... args)

   Used to supplement the :any:`make_unique`, :any:`make_poly`,
   and :cxx:`std::make_shared` functions. Takes a parameter pack :samp:`{args}`
   to construct a :samp:`{T}` with. This :samp:`{T}` is allocated via operator
   new (the default allocation scheme) and passed to a :any:`deep_ptr` for
   construction. This :any:`deep_ptr` is then returned by the function.

.. function:: template <class T> \
              std::unique_ptr<T[]> make_unique(std::size_t size)
              template <class... Args> \
              std::unique_ptr<T> make_unique(Args&&... args)
              template <class T, size_t N> \
              void make_unique<T[N]> (Args&&...) = delete

   :any:`make_unique` is provided to help supplement the
   :cxx:`std::make_shared<T>` function for the :cxx:`std::unique_ptr<T>` type.
   The first overload will be used if the given type :samp:`{T}` is not an
   array. If the given type :samp:`{T}` is an array of an unknown bound (that
   is, :samp:`std::extent<{T}>::value == 0`) the second overload is used.
   A third overload is provided to insure that the compiler will error. This
   third overload is available when the given type :samp:`{T}` is an array of a
   known bound (that is, :samp:`std::extent<{T}>::value != 0`).


Specializations
---------------

There are specializations for :any:`poly_ptr` and :any:`deep_ptr` for
integration with the C++ standard library.

.. namespace:: std

.. class:: template <> hash<poly_ptr<T, Deleter>>

   This specialization of :any:`hash` allows |poly_ptr| to be used as a
   key type in associative containers.

   For a given :any:`poly_ptr` *ptr*, this specialization insures that
   ``std::hash<poly_ptr<T, Deleter>> { }(ptr)`` is equivalent to the expression
   ``std::hash<typename poly_ptr<T, Deleter>::pointer> { }(ptr.get())``

.. class:: template <> hash<deep_ptr<T, Deleter, Copier>>

   This specialization of :class:`hash` allows |deep_ptr| to be used as a
   key type in associative containers.

   For a given |deep_ptr| *ptr*, this specialization insure that
   ``std::hash<deep_ptr<T, Deleter, Copier>> { }(ptr)`` is equivalent to the
   expression
   ``std::hash<typename deep_ptr<T, Deleter, Copier>::pointer> { }(ptr.get())``


