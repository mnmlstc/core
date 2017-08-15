Using MNMLSTC Core
==================

This section of the documentation goes into detail on how to use the MNMLSTC
Core library. Specifically, it discusses how the library is laid out, and how
features will be added and or deprecated.

Library Layout
--------------

MNMLSTC Core follows the same header layout as the C++ standard library. Each
library component is located in its own header. For instance, the
:doc:`memory component <memory>` is located in :file:`<core/{memory}.hpp>`, and
the :doc:`optional component <optional>` is located in
:file:`<core/{optional}.hpp>`. Several components may have a sub-namespace
within the :cxx:`core::` namespace. Each component's documentation will inform
the user if this is the case.

Project Layout
--------------

MNMLSTC Core's source tree has the following layout from the root:

 * docs - Contains all the documentation source files (reStructuredText)
 * include/core - Contains the headers necessary to use MNMLSTC Core
 * package - Contains files and resources for packaging
 * scripts - Contains various utility scripts
 * tests - Contains the unit test files
 * tools - Contains various plugins for different tools
 * License.rst - A license and copyright notice concerning MNMLSTC Core

Building and Installing
-----------------------

MNMLSTC Core provides several prebuilt packages for various operating systems.
However, if no package is provided for a given platform, it may be necessary
to 'build' MNMLSTC Core from source. This requires the `CMake`_ build
generation tool. MNMLSTC Core requires a C++11 compliant compiler.

If on a Unix-like system, run the following commands (options to be selected
are placed within ``[]``):

.. code-block:: bash

   mkdir build
   cd build
   cmake ../ \
    -DCMAKE_BUILD_TYPE=[Debug|Release|RelWithDebInfo] \
    -DBUILD_WITH_LIBCXX=[ON|OFF] \
    -DBUILD_DOCS=[ON|OFF] \
    -DDISABLE_EXCEPTIONS=[ON|OFF] \
    -DDISABLE_RTTI=[ON|OFF]
   make && make check && make install

Users on Windows should use the provided MSI package. At the time of this
writing, MNMLSTC Core does not work with Visual Studio, and requires either
GCC or Clang.

Configuration Options
^^^^^^^^^^^^^^^^^^^^^

The options documented below are meant more for those who might be packaging
MNMLSTC Core for repeatable installations on other systems, or installing
from source.

.. option:: BUILD_WITH_LIBCXX

   Used when building the tests provided with MNMLSTC Core.

.. option:: BUILD_DOCS

   Builds the sphinx documentation. Requires `Sphinx`_ 1.4.9 or later.

.. option:: DISABLE_EXCEPTIONS

   Exports the :c:macro:`CORE_NO_EXCEPTIONS` when importing MNMLSTC Core via
   CMake's :cmake:`find_package` command.

The option :cmake:`DISABLE_EXCEPTIONS` allows a user to export the
:c:macro:`CORE_NO_EXCEPTIONS`

The option :cmake:`DISABLE_RTTI` allows a user to export the
:c:macro:`CORE_NO_RTTI`

Compile Time Configuration
^^^^^^^^^^^^^^^^^^^^^^^^^^

Additionally, one can define the following macros when using MNMLSTC Core with
other libraries to configure MNMLSTC Core's available features.

.. c:macro:: CORE_NO_EXCEPTIONS

   Disables all features within MNMLSTC Core that require exceptions. In cases
   where an exception *would* be thrown, :cxx:`std::abort` is called instead.

.. c:macro:: CORE_NO_RTTI

   Disables all features within MNMLSTC Core that rely on RTTI

Using with CMake
----------------

MNMLSTC Core is also designed to be used with the `CMake`_ build system. Until
version 2.0, MNMLSTC Core followed a tradition of how :file:`Find{XXX}.cmake`
scripts would perform. Namely, it would define a variable
:cmake:`CORE_INCLUDE_DIR` and :cmake:`CORE_INCLUDE_DIRS`. Later on it provided
a :cmake:`core_INCLUDE_DIRS` variable. While these are still provided, MNMLSTC
Core now follows a new paradigm. Starting with version 2.0, MNMLSTC Core will
now supply an *imported* cmake target. This target allows the use of cmake
generator expressions for the target specific build files. Here is an example
of using MNMLSTC Core with CMake:

.. code-block:: cmake

   find_package(core 2.0.0 REQUIRED)
   add_library(my_library STATIC ${MY_SOURCE_FILES})
   target_link_libraries(my_library PUBLIC mnmlstc::core)

Because :cmake:`mnmlstc::core` is an :cmake:`INTERFACE` library, it uses
transitive linking by default, which means it will pass on its
*usage requirements* when used with :cmake:`target_link_libraries`. Regardless,
one can also use the less transitive form as well:

.. code-block:: cmake

  find_package(core 2.0 REQUIRED)
  add_library(my_library ${MY_SOURCE_FILES})
  target_include_directories(my_library
    PUBLIC
      $<TARGET_PROPERTY:mnmlstc::core,INTERFACE_INCLUDE_DIRECTORIES>)
  target_compile_definitions(my_library
    PUBLIC
      $<TARGET_PROPERTY:mnmlstc::core,INTERFACE_COMPILE_DEFINITIONS>)

Both of these approaches allow a user to now use MNMLSTC Core without having to
install it directly on a machine, and instead use it as a build tree export.
Before 2.0, this approach was not available, and an installation of MNMLSTC
Core was required.

Because :cmake:`mnmlstc::core` is an :cmake:`INTERFACE` library, it will also
permit *transitive linking* which means it will pass on its
*usage requirements* when used with :cmake:`target_link_libraries`:

.. code-block:: cmake

   find_package(core 2.0.0 REQUIRED)
   add_library(my_library ${MY_SOURCE_FILES})
   target_link_libraries(my_library INTERFACE mnmlstc::core)

This has the same result as the previous example.

The following variables are available for use after finding MNMLSTC Core:

.. code-block:: none

   CORE_VERSION
   CORE_VERSION_MAJOR
   CORE_VERSION_MINOR
   CORE_VERSION_PATCH
   CORE_VERSION_TWEAK

The following variables are available but deprecated for use:

.. code-block:: none

   CORE_INCLUDE_DIR
   CORE_INCLUDE_DIRS
   core_INCLUDE_DIRS

The targets exported by the MNMLSTC Core CMake package and the properties they
provides are:

 * :cmake:`mnmlstc::core`

   * :cmake:`INTERFACE_INCLUDE_DIRECTORIES`
   * :cmake:`INTERFACE_COMPILE_DEFINITIONS`

Debugging
---------

Starting with version 2.0, MNMLSTC Core provides a set of pretty printers for
the type it provides for use with both `GDB`_ and `LLDB`_. Because Visual
Studio is not supported, no pretty printing is available.

GDB
^^^

To enable the GDB pretty printers, add the following to your :file:`.gdbinit`

.. code-block:: none

   python
   import sys
   sys.path.insert(0, '/install-prefix/share/mnmlstc/formatter')
   import core
   core.__gdb_init_module(None)
   end

The :samp:`{install-prefix}` is typically :file:`/usr/local` on POSIX systems.
However, check with your system administrator for the installation location to
make sure it is accurate.

LLDB
^^^^

To enable LLDB pretty printers, colloquially known as data formatters, add
the following to your :file:`.lldbinit`::

  command script import /install-prefix/share/mnmlstc/core/llvm.py

The :samp:`{install-prefix}` is typically :file:`/usr/local` on POSIX systems.
However, check with your system administrator for the installation location to
make sure it is accurate.

Feature Addition and Deprecation
--------------------------------

MNMLSTC Core follows the `Semantic Versioning`_ specification. When a feature
is deprecated, it will be marked with an attribute (e.g.,
:cxx:`[[gnu::deprecated]]`). Because this type of attribute was not added
until C++14, it will unfortunately be compiler specific. Additionally, some
compiler versions don't understand multiple "deprecated" attributes and will
error when more than one of these attributes is applied to something. However,
the documentation for a specific feature or component will be marked as
deprecated, along with a link to the newer functionality.

In accordance with `Semantic Versioning`_, new features will be made available
in minor version releaes. Any API rewrites will be in major releases. It should
also be noted that MNMLSTC Database uses inline namespaces to keep major
versions as well as keep a stable ABI.

.. note:: While MNMLSTC Core has stated before version 2.0 that it follows
   Semantic Versioning, it has not done so in practice. C++ has the additional
   concern of internal ABI changes. for example, the difference between a user
   defined copy constructor and a defaulted copy constructor can, in some
   cases, cause obscure bugs and even segmentation faults. Starting with 2.0,
   MNMLSTC Core will make a more concerted effort to properly follow
   the Semantic Versioning specification.

Distribution
------------

In addition to installing MNMLSTC Core from source, or using `Biicode`_,
MNMLSTC Core is released with several packages for various platforms. The
naming convention of these packages is as follows:

.. code:: 

   core-<major>.<minor>.<patch>+<os>.<platform>.<extension>

For example, a 64-bit Windows MSI for 2.0.0 would be
:code:`core-2.0.0+windows.x64.msi`.

As of right now, the following binary package formats are provided:

 * Windows MSI x64
 * Linux RPM noarch
 * FreeBSD BIN any
 * Linux BIN any
 * Linux DEB any
 * OS X PKG any

 * Windows MSI x86

The :code:`BIN` format is a self-extracting tarball. A separate one is provided
for FreeBSD to keep in line with other MNMLSTC projects that may provide
platform specific packages (e.g., x64 and x86)

.. note:: Only the Linux RPM package is signed.

MNMLSTC Core provides source packages in the following formats:

 * .tar.bz2
 * .tar.gz
 * .tar.xz
 * .zip
 * .7z

.. _Semantic Versioning: http://semver.org/spec/v2.0.0
.. _Biicode: http://biicode.com
.. _Sphinx: http://sphinx-doc.org
.. _CMake: http://cmake.org
.. _LLDB: http://lldb.llvm.org
.. _GDB: http://www.gnu.org/software/gdb/
