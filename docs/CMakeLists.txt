find_program(SPHINX_EXECUTABLE
  NAMES sphinx-build
  DOC "Sphinx Documentation Generator"
)

if (NOT SPHINX_EXECUTABLE)
  message(FATAL_ERROR "Cannot build documentation without Sphinx!")
endif ()
mark_as_advanced(SPHINX_EXECUTABLE)

add_custom_target(docs
  COMMAND ${SPHINX_EXECUTABLE} ${DOCS_SOURCE_DIR} ${DOCS_BINARY_DIR}
  COMMENT "Generating Sphinx Documentation" VERBATIM
)

set_property(
  DIRECTORY APPEND PROPERTY
  ADDITIONAL_MAKE_CLEAN_FILES ${DOCS_BINARY_DIR}
)
