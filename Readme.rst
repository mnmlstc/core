Overview
========

MNMLSTC Core is a small and easy to use C++11 library that adds a functionality
set that will be available in C++14 and later, as well as some extremely useful
additions.

Information on installing and using MNMLSTC Core (as well as modifying its
internals) can be found in its documentation

MNMLSTC Core is released under the Apache 2.0 License.

Components
----------

Some components provided by MNMLSTC Core are:

 * ``polymorphic<T>``
 * ``variant<Ts...>``
 * ``optional<T>``
 * ``expected<T>``
 * ``deep_ptr<T>``
 * ``range<T>``
 * ``any``

And others. Details on each component can be found in MNMLSTC Core's
documentation.

Some important differences between these components and their Boost
counterparts are:

 * The ``optional<T>`` implements an interface equivalent to the C++14
   ``std::optional<T>``, but does so with C++11 semantics, which can be
   limiting, due to the now relaxed rules regarding constexpr.
 * The ``expected<T>`` is closely related to the ``Expected<T>`` put forth by
   Andrei Alexandrescu in his 2012 talk *Systematic Error Handling in C++*.
 * The ``variant<Ts...>`` can be empty, and in an uninitialized state. This
   means that, unlike the ``boost::variant<Ts...>``, a default constructed
   ``variant<Ts...>`` is in the same state as an ``optional<T>``.
 * The ``range<T>`` is an actual type that follows the proposed
   ``std::range<T>`` as explained in `N3350
   <http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3350.html>`_.

All of the MNMLSTC Core components reside in the ``core`` namespace.

Requirements
------------

There are several requirements to fully use MNMLSTC Core:

 * A C++11 compliant compiler (See the documentation for a minimum required
   feature set)
 * `CMake 2.8.11 <http://cmake.org>`_
 * `MNMLSTC Unittest <https://github.com/mnmlstc/unittest>`_
 * `Sphinx Documentation Generator <http://sphinx-doc.org>`_
 * `Cloud Sphinx Theme <https://pypi.python.org/pypi/cloud_sptheme>`_

MNMLSTC Unittest is only necessary if building and running the unit tests.

Sphinx and the Cloud Sphinx Theme are only necessary if generating
documentation manually.
