Overview
========

MNMLSTC Core is a small and easy to use C++11 library that adds a functionality
set that will be available in C++14 and later, as well as some useful
additions, or some proposals that have not been completely approved yet.

Information on installing and using MNMLSTC Core can be found in its
`documentation <http://mnmlstc.github.io/core/>`_.

MNMLSTC Core is released under the Apache 2.0 License.
Build scripts provided by MNMLSTC Core are released under the CC0 1.0 Universal
License.

.. image:: https://travis-ci.org/mnmlstc/core.svg
   :target: https://travis-ci.org/mnmlstc/core

.. image:: https://codecov.io/gh/mnmlstc/core/branch/master/graph/badge.svg
   :target: https://codecov.io/gh/mnmlstc/core

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
 * `CMake 3.0.0 <http://cmake.org>`_

Additionally, to develop or package MNMLSTC Core, the following are required:

 * `Sphinx Documentation Generator <http://sphinx-doc.org>`_
 * `Guzzle Sphinx Theme <https://github.com/guzzle/guzzle_sphinx_theme>`_
 * `WiX Toolset <http://wixtoolset.org>`_

Sphinx and the Guzzle Sphinx Theme are only necessary if generating
documentation manually to be included with the package.

WiX Toolset is only required if building packages for Windows.
