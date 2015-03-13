Overview
========

MNMLSTC Core is a small and easy to use C++11 library that adds a functionality
set that will be available in C++14 and later, as well as some useful
additions, or some proposals that have not been completely approved yet.

Information on installing and using MNMLSTC Core can be found in its
`documentation <http://mnmlstc.github.io/core/>`_.

MNMLSTC Core is released under the Apache 2.0 License.

.. image:: https://travis-ci.org/mnmlstc/core.svg
    :target: https://travis-ci.org/mnmlstc/core

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

 * A C++11 compliant compiler (GCC 4.8.1 or Clang 3.4 meet the minimum feature
   set required to build and use MNMLSTC Core)
 * `CMake 3.1.0 <http://cmake.org>`_

Additionally, to actively develop MNMLSTC Core, the following are required:
 * `Sphinx Documentation Generator <http://sphinx-doc.org>`_
 * `Alabaster Sphinx Theme <https://github.com/bitprophet/alabaster>`_
 * `WiX Toolset <http://wixtoolset.org>`_

Sphinx and the Alabaster Sphinx Theme are only necessary if generating
documentation manually.

WiX Toolset is only required if building packages for Windows.
