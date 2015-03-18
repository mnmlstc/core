.. role:: cmake(code)
   :language: cmake

.. _using-mnmlstc-core:

Using MNMLSTC Core
==================

This section of the documentation goes into detail on how to use the MNMLSTC
Core library. Specifically, it discusses how the library is laid out, and how
features will be added and or deprecated.

Library Layout
--------------

MNMLSTC Core follows the same header layout as the C++ standard library. Each
library component is located in its own header. For instance, the
:ref:`memory component <core-memory-component>` is located in
``<core/memory.hpp>``, and the
:ref:`optional component <core-optional-component>` is located in
``<core/optional.hpp>``. Several components may have a sub-namespace within
the :code:`core::` namespace. Each component's documentation will inform
the user if this is the case.

Feature Addition and Deprecation
--------------------------------

MNMLSTC Core follows `Semantic Versioning 2.0
<http://semver.org/spec/v2.0.0.html>`_. Additionally, starting with version
1.2, a 4th version number will be used within the CMakeLists.txt file to
represent the *build revision* of the library. In many linux distributions,
this is usually appended as a hyphen followed by a number, to distinguish the
changes made between build versions and release versions. This 4th number
(known as :cmake:`PROJECT_VERSION_TWEAK` within CMake) will be appended to *all
packages* moving forward. There is no mention of this approach in SemVer, and
as such this can be considered an extension of it. While SemVer does take some
criticism, the issue at hand for C and C++ projects is that there are
effectively two projects within them. The first being the actual library, tool,
or framework, and the other being the build system used to build, test, and
package it.

Biicode
-------

`Biicode <http://biicode.com>`_ is a C and C++ package manager. MNMLSTC Core
now supports Biicode as of version 1.2. Using MNMLSTC Core with Biicode is
quite simple. Within your :code:`biicode.conf` file, place the following:

.. code-block:: ini

   [requirements]
     mnmlstc/core @1.2.0

   [includes]
     core/*.hpp : mnmlstc/core/include

And then run the :code:`bii find` command. This will pull the latest tagged
version of MNMLSTC Core 1.2.0, as well as the License file. To use the headers,
simply include :code:`<core/<header>.hpp>`.

CMake
-----

MNMLSTC Core is designed to be used with the `CMake <http://cmake.org>`_ build
system. Until version 1.2, MNMLSTC Core followed a tradition of how
:cmake:`FindXXX.cmake` scripts would perform. Namely, it would define a
variable :cmake:`CORE_INCLUDE_DIR` and :cmake:`CORE_INCLUDE_DIRS`. Later on it
provided a :cmake:`core_INCLUDE_DIRS` variable. While these are still
provided, MNMLSTC Core now follows a new paradigm. Starting with version 1.2,
MNMLSTC Core will now supply an *imported* cmake target. This target allows the
use of cmake generator expressions for the target specific build files. Here is
an example of using MNMLSTC Core with CMake:

.. code-block:: cmake

  find_package(core 1.2.0 REQUIRED)
  add_library(my_library ${MY_SOURCE_FILES})
  target_include_directories(my_library
    PUBLIC
      $<TARGET_PROPERTY:mnmlstc::core,INTERFACE_INCLUDE_DIRECTORIES>)

While this is more verbose than a simple :cmake:`include_directories` call in
CMake, it allows libraries that may use MNMLSTC Core to propagate their include
directories without requiring that additional packages find both the library
that depends on MNMLSTC Core *and* MNMLSTC Core itself.

Additionally, this approach allows a user to now use MNMLSTC Core without
having to install it directly on a machine, and instead use it as a build tree
export. Before 1.2, this approach was not available, and an installation of
MNMLSTC Core was required.

The following variables are available for use after finding MNMLSTC Core:

.. code-block:: cmake

   CORE_VERSION
   CORE_VERSION_MAJOR
   CORE_VERSION_MINOR
   CORE_VERSION_PATCH
   CORE_VERSION_TWEAK

The following variables are available but deprecated for use:

.. code-block:: cmake

   CORE_INCLUDE_DIR
   CORE_INCLUDE_DIRS
   core_INCLUDE_DIRS

The following target is exported:

.. code-block:: cmake

   mnmlstc::core
