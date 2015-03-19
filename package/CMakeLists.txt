list(APPEND GENERATORS "TGZ;TXZ;ZIP;7Z")

if (BUILD_PACKAGE_MSI)
  list(APPEND GENERATORS "WIX")
endif ()

if (BUILD_PACKAGE_RPM)
  list(APPEND GENERATORS "RPM")
endif ()

set(WIX_SOURCE_DIR "${PACK_SOURCE_DIR}/wix")

#------------------------------------------------------------------------------
# Generic Package
#------------------------------------------------------------------------------
set(CPACK_OUTPUT_FILE_PREFIX ${PACK_BINARY_DIR})

set(CPACK_GENERATOR ${GENERATORS})
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "MNMLSTC")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++14 features for C++11")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/Readme.rst")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/License.rst")

set(CPACK_PACKAGE_VERSION_MAJOR ${CORE_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${CORE_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${CORE_VERSION_PATCH})

set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${PROJECT_VERSION}")

# Intended for installers such as NSIS or WIX
set(CPACK_PACKAGE_INSTALL_DIRECTORY "mnmlstc/${PROJECT_NAME}")

#------------------------------------------------------------------------------
# Wix Configuration
#------------------------------------------------------------------------------
set(CPACK_WIX_CMAKE_PACKAGE_REGISTRY ${PROJECT_NAME})
set(CPACK_WIX_UPGRADE_GUID "AE6C6986-200F-464C-83EF-C3A94C6C9995")
set(CPACK_WIX_PRODUCT_GUID "*")
set(CPACK_WIX_PRODUCT_ICON "${WIX_SOURCE_DIR}/mnmlstc.ico")
set(CPACK_WIX_LICENSE_RTF "${WIX_SOURCE_DIR}/License.rtf")
set(CPACK_WIX_UI_BANNER "${WIX_SOURCE_DIR}/banner.bmp")
set(CPACK_WIX_UI_DIALOG "${WIX_SOURCE_DIR}/dialog.bmp")

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

include(CPack)
