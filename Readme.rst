Overview
========

MNMLSTC Core is a small and easy to use C++11 library that adds a functionality
set that will be available in C++14 and later, as well as some useful
additions, or some proposals that have not been completely approved yet.

Information on installing and using MNMLSTC Core can be found in its
`documentation <http://mnmlstc.github.io/core/>`_.

MNMLSTC Core is released under the Apache 2.0 License.

Components
----------

Some components provided by MNMLSTC Core are:

 * ``variant<Ts...>``
 * ``optional<T>``
 * ``expected<T>``
 * ``deep_ptr<T>``
 * ``poly_ptr<T>``
 * ``string_view``
 * ``range<T>``
 * ``any``

Details on each component can be found in MNMLSTC Core's documentation. All of
the MNMLSTC Core components reside in the ``core`` namespace. The library is
organized equivalent to the standard library e.g., components related to memory
are in the memory header, functional components in the functional header, etc.

Requirements
------------

There are several requirements to fully use MNMLSTC Core:

 * A C++11 compliant compiler (GCC 4.8.2 or Clang 3.3 meet the minimum feature
   set required to build and use MNMLSTC Core)
 * `CMake 2.8.11 <http://cmake.org>`_
 * `MNMLSTC Unittest <https://github.com/mnmlstc/unittest>`_
 * `Sphinx Documentation Generator <http://sphinx-doc.org>`_
 * `Sphinx RTD Theme <https://pypi.python.org/pypi/sphinx_rtd_theme>`_

MNMLSTC Unittest is only necessary if building and running the unit tests.

Sphinx and the Sphinx RTD Theme are only necessary if generating
documentation manually.
