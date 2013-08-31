.. _using-mnmlstc-core:

Using MNMLSTC Core
==================

This section of the documentation goes ito detail on how to use the MNMLSTC
Core library. Specifically, it discusses how the library is laid out, and how
features will be added and or deprecated.

Library Layout
--------------

MNMLSTC Core follows the same header layout as the C++ standard library.
For example, any augmentations made to components located within the
``<memory>`` header will be found in ``<core/memory.hpp>``. For components that
are not currently in the C++ standard, they will follow a layout of
the *primary* focus of the component being the location of the name. For
example, the ``<core/polymorphic.hpp>`` contains :cpp:class:`polymorphic\<T>`
as it is the primary focus of that specific component.

Feature Addition and Deprecation
--------------------------------

MNMLSTC Core follows `Semantic Versioning 2.0
<http://semver.org/spec/2.0.0.html>`_. 
