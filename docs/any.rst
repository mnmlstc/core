Any Component
=============

.. namespace:: core

The :any:`any` component is currently available in Boost (and has been for
quite some time). The :any:`any` component follows the Library Technical
Specification to the letter, and even performs the *small object optimization*.
This means that for types whose size are less than (or equal to) a
:cxx:`void*`, no allocation will take place.

The any component resides in :file:`<core/{any}.hpp>`.

This component is unavailable if :c:macro:`CORE_NO_RTTI` is defined.

.. index::
   single: any; bad_any_cast
   single: exceptions; bad_any_cast

.. class:: bad_any_cast

   :inherits: std::bad_cast

   This is the exception thrown when :any:`any_cast` fails. It inherits from
   :cxx:`std::bad_cast`.

   This type is unavailable if :c:macro:`CORE_NO_EXCEPTIONS` is defined.

   .. function:: char const* what () const noexcept
   
      Returns the string "bad any cast". If :cxx:`typeid(T).name()` was
      standardized, this would be added to the error message

.. index:: any

.. class:: any

   .. index:: any; constructors

   .. function:: any (any const&)
                 any (any&&) noexcept
                 any () noexcept

      The default set of constructors work as one might imagine. The copy
      constructor is not marked as noexcept as the underlying type *may*
      throw an exception, and due to the type erasure performed, the :any:`any`
      has no way of enforcing this at compile time.

   .. function:: any (ValueType&& value)
   
      When constructing an :any:`any` with a given *ValueType*, it will perform
      a series of compile time checks to see whether it should perform the
      small object optimization. If the object is deemed small enough, it will
      not allocate memory. Otherwise, a new *ValueType* will be allocated via
      :cxx:`operator new`, and constructed with the given *value*.
   
      :raises: Any exceptions thrown by the copy or move constructor
               of the given ValueType.

   .. function:: any& operator = (any const&)
                 any& operator = (any&&) noexcept
   
      Assigns the contents of the incoming :any:`any` to :cxx:`*this`.

   .. function:: any& operator = (ValueType&& value)
   
      Assigns *value* to :cxx:`*this`. If :cxx:`*this` already manages a
      contained object, it will be destroyed after *value* is assigned.
   
   .. function:: void swap (any&) noexcept
   
      Performs a simple state change with the incoming :any:`any`.

      .. note:: Previous versions of the documentation for this function gave
         the impression that an actual swap operation took place. However,
         a *state* change results in calling swap on the internal storage
         type used by :any:`any`.

   .. index:: any; observers

   .. function:: std::type_info const& type () const noexcept
   
      Returns the :cxx:`std::type_info` for the type contained within. If the
      :any:`any` is empty, it will return :cxx:`typeid(void)`.

   .. function:: bool empty () const noexcept
   
      If the :any:`any` does not contain any data (i.e. :any:`type` returns
      :cxx:`typeid(void)`), it will return :cxx:`true`.

   .. index:: any; operations

   .. function:: void clear () noexcept
   
      :postcondition: :any:`empty` == true
   
      Destroys the object contained within the :any:`any`.

.. index:: any; functions

.. function:: ValueType any_cast (any const& operand)
              ValueType any_cast (any&& operand)
              ValueType any_cast (any& operand)

   Given a type *ValueType*, it will attempt to extract the value stored within
   the given :any:`any`. *ValueType* may be either concrete or a reference
   type. If :cxx:`typeid(remove_reference_t<ValueType>)` is not equal to the
   value returned by :any:`type`, :any:`bad_any_cast` is thrown.

   :returns: :cxx:`*any_cast<add_const_t<remove_reference_t<T>>(&operand)`
             for the first :any:`any_cast` signature. For the other overloads,
             the return type is
             :cxx:`*any_cast<remove_reference_t<T>>(&operand)`.

   :raises: :any:`bad_any_cast`

   :example:
     .. code-block:: cpp

        any x(5) // x holds an int
        auto y = any_cast<int>(x); // cast to a value
        any_cast<int&>(x) = 10; // cast to a reference for mutation.

        // x now holds a string.
        x = std::string { "Hey Buzz... your girlfriend..." };
        auto woof = std::move(any_cast<std::string&>(x)); // move value in x
        assert(any_cast<std::string const&>(x) == "");

.. function:: ValueType const* any_cast (any const* operand)
              ValueType* any_cast (any* operand)

   This function works a lot like :cxx:`dynamic_cast` and allows one to
   use the :cxx:`dynamic_cast` assignment idiom:

   :returns: *ValueType* if operand is not equal to :cxx:`nullptr` and
             :cxx:`typeid(ValueType)` is the same as the value returned by 
             :func:`type() <core::any::type>`, a pointer to the object managed
             by *operand* is returned. Otherwise, :cxx:`nullptr`.

   :example:
     .. code-block:: cpp

        any x(5) // x holds an int
        if (auto ptr = any_cast<double>(&x)) {
          // do something with a double here
        } else if (auto ptr = any_cast<int>(&x)) {
          // handle the int case
        }


