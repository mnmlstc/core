Type Traits Component
=====================

.. namespace:: core

The type traits component contains many utilities that are available in C++14,
as well as a few custom types. Such as, :any:`is_nothrow_swappable` and
:any:`is_swappable`. *Most* of the types provided within this component are
in fact simple type aliases defined for C++14. e.g.,
``typename std::decay<T>::type`` is aliased to :cxx:`core::decay_t<T>`. All
types that are used as type *modifiers* (rather than type *properties*) have
the equivalent alias available.

Only type traits not provided by the C++11 standard are documented. If a
type trait provided by MNMLSTC Core differs from the standard in any way, it is
also documented here.

The type traits component can be found in the :file:`<core/{type_traits}.hpp>`
header.

.. type:: template <class T> is_null_pointer

   An alias for ``std::true_type`` if :samp:`{T}` is any form of the type
   ``std::nullptr_t``. This includes its *const* and *volatile* qualified
   counterparts.

.. type:: template <class T> class_of_t

   Given a member function pointer type :samp:`{T}`, it extracts the underlying
   class type.

   :example:

    .. code-block:: cpp

       struct A { void empty () { } };

       using type = class_of_t<decltype(&A::empty)>;
       static_assert(std::is_same<type, A>::value, "");

.. type:: template <class...> invokable

   .. deprecated:: 2.0

   Given a typelist :samp:`{Args}...`, checks if the first type in
   :samp:`{Args}...` may be invoked with the rest of the arguments in
   :samp:`{Args}`. The rule for deciding if :samp:`{Args}...` is invokable
   follows the *INVOKE* pseudo expression.

.. class:: template <class...> invoke_of

   .. deprecated:: 2.0

   Given a typelist :samp:`{Args}...`, the member typedef :cxx:`type` will
   represent the return type if :samp:`{Args}` is invoked according to the
   *INVOKE* pseudo-expression.

.. class:: template <class T> result_of

   This is an SFINAE capable version of :cxx:`std::result_of`. It relies on
   :any:`invoke_of` to work correctly.

.. class:: template <class...> common_type

   A more compiler agnostic version of ``std::common_type<{Ts}...>``. This was
   implemented to workaround an issue with Clang's ``std::common_type``
   attempting to discover the common type of two ``void&&``. Additionally,
   this type trait follows the C++14 rules of decaying the common type.

.. type:: template <class T> is_nothrow_swappable

   A type trait that is ``std::true_type`` if a given swap call on a type is
   actually marked as *noexcept*, and ``std::false_type`` otherwise. This trait
   is comparable to libc++'s internal ``__is_nothrow_swappable``.

   This is also, oddly enough, an implementation of the
   :cxx:`is_nothrow_swappable` trait proposed in :wg21:`N4426`, and was added
   to MNMLSTC Core before the proposal was submitted.

.. class:: template <size_t Len, class... Ts> aligned_union

   This is an implementation of :cxx:`std::aligned_union`, and is provided
   for a certain compiler whose library implementations may not have added it
   until 2015-APR-22 and whose colloqiual pronunciation rhymes with
   "Pre Free Tea". That's "Free as in Tea", not "Free as in Speech" ;)

.. type:: template <bool B> bool_constant

   An alias of :cxx:`std::integral_constant<bool, B>`. This is a implementation
   of :wg21:`N4389`.

.. type:: template <size_t I, class T> tuple_element_t
          template <class T> tuple_size_t

   These two type aliases are provided as they are missing from C++11. They
   are simply type aliases for :cxx:`std::tuple_element` and
   :cxx:`std::tuple_size`.

The Detection Idiom
-------------------

The detection idiom is a set of powerful meta templates that obviate the need
for the 'old' approach to detecting if a member function, member, or function
call would work for SFINAE purposes. It was originally started with
Walter E. Brown's :any:`void_t`, but has since grown to include several
additional types such as :any:`is_detected`, :any:`detected_or`, etc.

To give users a better idea, each entry has an example of how each part of
the detection idiom would be used.

.. type:: template <class... Args> void_t

   The infamous :any:`void_t` is a powerful SFINAE metaprogramming tool
   discovered by Walter E. Brown. It is used as the basis for
   :any:`is_detected`, as well as other detection pieces.

   :example:

     .. code-block:: cpp

        // detect if class has a T::size() member function, but ignore the
        // return type.
        template <class T, class=void>
        struct has_size_mem_fn : std::false_type { };

        template <class T>
        struct has_size_mem_fn<
          T,
          void_t<decltype(std::declval<T>().size())>
        > : std::true_type { };

        static_assert(has_size_mem_fn<std::string>::value, "");
        static_assert(not has_size_mem_fn<int>::value, "");

.. type:: template <template <class...> class Op, class... Args> \
          is_detected

   Some text goes here

.. type:: template <template <class...> class Op, class... Args> \
          detected_t

   Some More Text

.. type:: template <class T, template <class...> class Op, class... Args> \
          detected_or

   EVEN. MORE. TEXT.
