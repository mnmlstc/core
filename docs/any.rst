.. |any| replace:: :class:`any <core::any>`

.. _core-any-component:

Any Component
=============

The |any| component is currently available in Boost (and has been for
quite some time). The |any| component follows the Library Technical
Specification to the letter, and even performs the *small object optimization*.
This means that for types whose size are less than (or equal to) a ``void*``,
no allocation will take place.

The any component resides in ``<core/any.hpp>``.

.. namespace:: core

.. class:: bad_any_cast

   :inherits: std::bad_cast

   This is the exception thrown when :func:`any_cast` fails.
   It inherits from ``std::bad_cast``.

.. function:: char const* bad_any_cast::what () const noexcept

   Returns the string "bad any cast". At some point in the future a more
   descriptive error may be given.

.. class:: any

.. function:: any::any (any const&)
              any::any (any&&) noexcept
              any::any () noexcept

   .. versionadded:: 1.2

      The behavior regarding the move constructor has changed to follow
      the specification. It will now construct a value of the same type
      as contained in the incoming |any| and use the move constructor of
      the type. If the incoming |any| is empty, the newly constructed
      |any| will be as well. Before 1.2, the move constructor would simply
      result in a state change.

   The default set of constructors work as one might imagine. The copy
   constructor is not marked as noexcept as the underlying type *may*
   throw an exception, and due to the type erasure performed, the |any| has
   no way of enforcing this at compile time.

.. function:: any::any (ValueType&& value)

   When constructing an |any| with a given :cxx:`ValueType`, it will perform a
   series of compile time checks to see whether it should perform the small
   object optimization. If the object is deemed small enough, it will not
   allocate memory. Otherwise, a new :cxx:`ValueType` will be allocated via
   :cxx:`operator new`, and constructed with the given *value*.

   :raises: Any exceptions thrown by the copy or move constructor
            of the given ValueType.

.. function:: any& any::operator = (any const&)
              any& any::operator = (any&&) noexcept

   Assigns the contents of the incoming |any| to ``*this``.

.. function:: any& any::operator = (ValueType&& value)

   Assigns *value* to ``*this``. If ``*this`` already manages a contained
   object, it will be destroyed after *value* is assigned.

   .. versionadded:: 1.1

      This function was unfortunately omitted from the 1.0 release.

.. function:: void any::swap (any&) noexcept

   Swaps the object contained within the given |any| with the one contained
   within ``*this``.

.. function:: std::type_info const& any::type () const noexcept

   Returns the :cxx:`std::type_info` for the type contained within. If the
   |any| is empty, it will return :cxx:`typeid(void)`.

.. function:: bool any::empty () const noexcept

   If the |any| does not contain any data (i.e.
   :func:`type() <core::any::type>` returns :cxx:`typeid(void)`), it will
   return true.

.. function:: void any::clear () noexcept

   :postcondition: :func:`empty() <core::any::empty>` == true

   Destroys the object contained within the |any|.


.. function:: ValueType any_cast (any const& operand)
              ValueType any_cast (any&& operand)
              ValueType any_cast (any& operand)

   Given a type *ValueType*, it will attempt to extract the value stored within
   the given |any|. *ValueType* may be either concrete or a reference type.
   If ``typeid(remove_reference_t<ValueType>)`` is not equal to the value
   returned by :func:`type() <core::any::type>`, :class:`bad_any_cast` is
   thrown.

   :returns: ``*any_cast<add_const_t<remove_reference_t<ValueType>>(&operand)``
             for the first :func:`any_cast` signature. For the other overloads,
             the return type is
             ``*any_cast<remove_reference_t<ValueType>>(&operand)``.

   :raises: :class:`bad_any_cast`

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


