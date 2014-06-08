list(APPEND GENERATORS "TGZ" "ZIP")

if (BUILD_PACKAGE_MSI)
  list(APPEND GENERATORS "WIX")
endif ()

# disabled until CPack can generate PKGs on modern OS X
#if (BUILD_PACKAGE_PKG)
#  list(APPEND GENERATORS "PackageMaker")
#endif ()

if (BUILD_PACKAGE_DEB)
  list(APPEND GENERATORS "DEB")
endif ()

if (BUILD_PACKAGE_RPM)
  list(APPEND GENERATORS "RPM")
endif ()

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

# Intended for installers such as NSIS or WIX
set(CPACK_PACKAGE_INSTALL_DIRECTORY "mnmlstc/${PROJECT_NAME}")

#------------------------------------------------------------------------------
# Wix Configuration
#------------------------------------------------------------------------------
set(CPACK_WIX_UPGRADE_GUID "AE6C6986-200F-464C-83EF-C3A94C6C9995")
set(CPACK_WIX_PRODUCT_GUID "*")
set(CPACK_WIX_PRODUCT_ICON "${PROJECT_SOURCE_DIR}/package/mnmlstc.ico")
set(CPACK_WIX_LICENSE_RTF "${PROJECT_SOURCE_DIR}/package/License.rtf")
set(CPACK_WIX_UI_BANNER "${PROJECT_SOURCE_DIR}/package/banner.bmp")
set(CPACK_WIX_UI_DIALOG "${PROJECT_SOURCE_DIR}/package/dialog.bmp")

#------------------------------------------------------------------------------
# DEB Configuration
#------------------------------------------------------------------------------
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE all)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "MNMLSTC <development@mnmlstc.com>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "cmake (>= 2.8.12)")

#------------------------------------------------------------------------------
# RPM Configuration
#------------------------------------------------------------------------------
set(CPACK_RPM_PACKAGE_ARCHITECTURE noarch)
set(CPACK_RPM_PACKAGE_REQUIRES "cmake >= 2.8.12")
set(CPACK_RPM_PACKAGE_RELEASE 1)
set(CPACK_RPM_PACKAGE_LICENSE "ASL 2.0")
set(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")

include(CPack)
