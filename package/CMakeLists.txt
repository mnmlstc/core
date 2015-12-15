list(APPEND SOURCE_GENERATORS TBZ2)
list(APPEND SOURCE_GENERATORS TGZ)
list(APPEND SOURCE_GENERATORS TXZ)
list(APPEND SOURCE_GENERATORS ZIP)
list(APPEND SOURCE_GENERATORS 7Z)

if (NOT WIN32)
  list(APPEND BINARY_GENERATORS STGZ)
endif ()

if (BUILD_PACKAGE_MSI)
  list(APPEND BINARY_GENERATORS "WIX")
endif ()

if (BUILD_PACKAGE_RPM)
  list(APPEND BINARY_GENERATORS "RPM")
endif ()

#------------------------------------------------------------------------------
# Generic Package Settings
#------------------------------------------------------------------------------
set(CPACK_OUTPUT_FILE_PREFIX ${PACK_BINARY_DIR})

set(CPACK_GENERATOR ${BINARY_GENERATORS})
set(CPACK_SOURCE_GENERATOR ${SOURCE_GENERATORS})

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR MNMLSTC)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++14 library features for C++11")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/Readme.rst")

set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/License.rst")

set(CPACK_PACKAGE_VERSION_MAJOR ${CORE_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${CORE_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${CORE_VERSION_PATCH})

set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${PROJECT_VERSION}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME ${CPACK_PACKAGE_FILE_NAME})

# Intended for installers such as NSIS or WIX
set(CPACK_PACKAGE_INSTALL_DIRECTORY "mnmlstc/${PROJECT_NAME}")

list(APPEND CPACK_SOURCE_IGNORE_FILES "/build-clang/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/build-msvc/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/build-gcc/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/build/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/.git/")

list(APPEND CPACK_SOURCE_IGNORE_FILES ".gitattributes")
list(APPEND CPACK_SOURCE_IGNORE_FILES ".travis.yml")
list(APPEND CPACK_SOURCE_IGNORE_FILES ".gitignore")
list(APPEND CPACK_SOURCE_IGNORE_FILES ".DS_Store")

list(APPEND CPACK_SOURCE_IGNORE_FILES "appveyor.yml")
#------------------------------------------------------------------------------
# Wix Configuration
#------------------------------------------------------------------------------
set(CPACK_WIX_CMAKE_PACKAGE_REGISTRY ${PROJECT_NAME})
set(CPACK_WIX_UPGRADE_GUID "AE6C6986-200F-464C-83EF-C3A94C6C9995")
set(CPACK_WIX_PRODUCT_GUID "*")
set(CPACK_WIX_PRODUCT_ICON "${PACK_SOURCE_DIR}/mnmlstc.ico")
set(CPACK_WIX_LICENSE_RTF "${PACK_SOURCE_DIR}/License.rtf")
set(CPACK_WIX_UI_BANNER "${PACK_SOURCE_DIR}/banner.bmp")
set(CPACK_WIX_UI_DIALOG "${PACK_SOURCE_DIR}/dialog.bmp")

#------------------------------------------------------------------------------
# RPM Configuration
#------------------------------------------------------------------------------
set(CPACK_RPM_PACKAGE_ARCHITECTURE noarch)
set(CPACK_RPM_PACKAGE_RELEASE 1)
set(CPACK_RPM_PACKAGE_LICENSE "ASL 2.0")
set(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")

# Prevents CPack from generating file conflicts
set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "/usr/share/doc/mnmlstc")
list(APPEND CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "/usr/share/cmake")

#------------------------------------------------------------------------------
# Finalize
#------------------------------------------------------------------------------
include(CPack)

# Create some custom commands to piggy back off of CPack
# If not on OS X, we create an "empty" command for 'dist'
if (APPLE AND BUILD_PACKAGE_PKG)
  find_package(PythonInterp REQUIRED)
  add_custom_target(pkgbuild
  COMMAND ${PYTHON_EXECUTABLE}
    ${PROJECT_SOURCE_DIR}/scripts/pkgbuild.py
    -b ${CMAKE_BINARY_DIR}
    -r ${PACK_SOURCE_DIR}
  COMMENT "Run pkgbuild.py packaging script"
  VERBATIM)
else ()
  add_custom_target(pkgbuild COMMAND ${CMAKE_COMMAND} -E sleep 0 VERBATIM)
endif ()

# A 'nice' 
add_custom_target(source
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target package_source
  VERBATIM)
add_custom_target(dist
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target package
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target source
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target pkgbuild
  VERBATIM)
