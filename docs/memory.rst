Memory Component
================

.. default-domain:: cpp

This section discusses the memory components that are augmented by
MNMLSTC Core.

.. namespace:: core

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
