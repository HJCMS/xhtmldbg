# CMakeLists.txt
##############################################################
PROJECT (xhtmldbg)

CMAKE_MINIMUM_REQUIRED (VERSION 2.8.0 FATAL_ERROR)

##############################################################
# CMP0002: we have multiple targets with the same name for the unit tests
##############################################################
CMAKE_POLICY (VERSION 2.8)

##############################################################
# where to look first for cmake modules, before ${CMAKE_ROOT}/cmake/Modules/ is checked
##############################################################
SET (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

##############################################################
## Package
##############################################################
SET (QT_MIN_VERSION 4.6.0)
FIND_PACKAGE (PkgConfig REQUIRED)
FIND_PACKAGE (Qt4 REQUIRED)
FIND_PACKAGE (Qt4 COMPONENTS QTWEBKIT QTNETWORK QTDBUS QTXMLPATTERNS)
FIND_PACKAGE (QTidy REQUIRED)
FIND_PACKAGE (Raptor REQUIRED)
FIND_PACKAGE (GeoIP REQUIRED)
FIND_PACKAGE (Java)

INCLUDE (${QT_USE_FILE})

##############################################################
## Set xhtmldbg Specific Information
##############################################################

SET (XHTMLDBG_VERSION_MAJOR 0)
SET (XHTMLDBG_VERSION_MINOR 8)
SET (XHTMLDBG_VERSION_RELEASE 8)
SET (XHTMLDBG_PATCHLEVEL ".rc2")
SET (XHTMLDBG_VERSION "${XHTMLDBG_VERSION_MAJOR}.${XHTMLDBG_VERSION_MINOR}.${XHTMLDBG_VERSION_RELEASE}${XHTMLDBG_PATCHLEVEL}")
SET (XHTMLDBG_DISTRIBUTION_TEXT "compiled sources" CACHE STRING "Indicate the distribution in bug reports")
SET (LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)")
SET (XHTMLDBG_CXXFLAGS "" CACHE STRING "Defined in pkg-config file")
SET (XHTMLDBG_REQUIRES "" CACHE STRING "Defined in pkg-config file")
SET (XHTMLDBG_RESOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/data/qrcimages.qrc")
SET (CMAKE_INSTALL_RPATH_USE_LINK_PATH  TRUE)
## Extra Debugging Options
OPTION (XHTMLDBG_BUILD_VERBOSE "Enable more Debugging Messages (ON/OFF)" OFF)
MARK_AS_ADVANCED (XHTMLDBG_BUILD_VERBOSE)
## Enable Beta Extensions
OPTION (XHTMLDBG_BETA_EXTENSION "Enable Extensions that market Beta (ON/OFF)" OFF)
MARK_AS_ADVANCED (XHTMLDBG_BETA_EXTENSION)

IF (NOT QT_USE_QTNETWORK)
  MESSAGE (FATAL_ERROR  "xhtmldbg: Requires Qt4 with QtNetwork support.")
ENDIF (NOT QT_USE_QTNETWORK)

##############################################################
## Set Header Definitions
##############################################################

INCLUDE (CheckCXXCompilerFlag)
INCLUDE (MacroEnsureVersion)

IF (CMAKE_COMPILER_IS_GNUCXX)

  IF (CMAKE_SYSTEM_NAME MATCHES Linux)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnon-virtual-dtor -Wundef -Wcast-align -Wchar-subscripts -Wpointer-arith -Wformat-security -fno-exceptions -fno-check-new -fno-common")
    ADD_DEFINITIONS (-D_BSD_SOURCE -DQT_NO_EXCEPTIONS)
  ENDIF (CMAKE_SYSTEM_NAME MATCHES Linux)

  # gcc under Windows
  IF (MINGW)
    SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--export-all-symbols -Wl,--disable-auto-import")
    SET (CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--export-all-symbols -Wl,--disable-auto-import")
    ADD_DEFINITIONS(-DQT_NO_DEBUG)
  ENDIF (MINGW)

  # visibility support http://gcc.gnu.org/wiki/Visibility
  CHECK_CXX_COMPILER_FLAG (-fvisibility=hidden __CHECK_GCC_VISIBILITY)
  SET (__CHECK_GCC_VISIBILITY ${__CHECK_GCC_VISIBILITY} CACHE BOOL "GCC support for hidden visibility")

  # stack-protector support
  CHECK_CXX_COMPILER_FLAG(-fstack-protector __CHECK_GCC_STACK_PROTECTOR)
  SET (__CHECK_GCC_STACK_PROTECTOR ${__CHECK_GCC_STACK_PROTECTOR} CACHE BOOL "GCC support for stack-protector")

  # buffer checks
  CHECK_CXX_COMPILER_FLAG(-D_FORTIFY_SOURCE=2 __CHECK_GCC_FORTIFY_SOURCE)
  SET (__CHECK_GCC_FORTIFY_SOURCE ${__CHECK_GCC_FORTIFY_SOURCE} CACHE BOOL "GCC support for -DFORTIFY_SOURCE")

  # emit position-independent code
  CHECK_CXX_COMPILER_FLAG(-fPIC __CHECK_GCC_FPIC)
  SET (__CHECK_GCC_FPIC ${__CHECK_GCC_FPIC} CACHE BOOL "GCC support for -fPIC")

  # gives some advantages over fPIC
  CHECK_CXX_COMPILER_FLAG(-pie __CHECK_GCC_FPIE)
  SET (__CHECK_GCC_FPIE ${__CHECK_GCC_FPIE} CACHE BOOL "GCC support for -pie")

  # Pass the flag -export-dynamic to the ELF linker, on targets that support it.
  CHECK_CXX_COMPILER_FLAG(-rdynamic __CHECK_LINK_RDYNAMIC)
  SET (__CHECK_LINK_RDYNAMIC ${__CHECK_LINK_RDYNAMIC} CACHE BOOL "compiler support for -rdynamic")

  # get the gcc version
  EXEC_PROGRAM (${CMAKE_C_COMPILER} ARGS --version OUTPUT_VARIABLE _gcc_version_info)

  STRING (REGEX MATCH "[345]\\.[0-9]\\.[0-9]" _gcc_version "${_gcc_version_info}")
  # gcc on mac just reports: "gcc (GCC) 3.3 20030304 ..." without the patch level, handle this here:
  IF (NOT _gcc_version)
    STRING (REGEX REPLACE ".*\\(GCC\\).* ([34]\\.[0-9]) .*" "\\1.0" _gcc_version "${_gcc_version_info}")
  ENDIF (NOT _gcc_version)

  MACRO_ENSURE_VERSION ("4.1.0" "${_gcc_version}" GCC_IS_NEWER_THAN_4_1)
  MACRO_ENSURE_VERSION ("4.2.0" "${_gcc_version}" GCC_IS_NEWER_THAN_4_2)
  MACRO_ENSURE_VERSION ("4.3.0" "${_gcc_version}" GCC_IS_NEWER_THAN_4_3)
  MACRO_ENSURE_VERSION ("4.4.0" "${_gcc_version}" GCC_IS_NEWER_THAN_4_4)
  MACRO_ENSURE_VERSION ("4.5.0" "${_gcc_version}" GCC_IS_NEWER_THAN_4_5)

  IF (__CHECK_GCC_VISIBILITY AND GCC_IS_NEWER_THAN_4_1)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")

    IF (GCC_IS_NEWER_THAN_4_2)
        SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility-inlines-hidden")
    ENDIF (GCC_IS_NEWER_THAN_4_2)

  ENDIF (__CHECK_GCC_VISIBILITY AND GCC_IS_NEWER_THAN_4_1)

  IF (GCC_IS_NEWER_THAN_4_2 AND __CHECK_GCC_VISIBILITY)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_VISIBILITY")
    ## Bugfix gcc-4.3 need explicit definition
    ADD_DEFINITIONS (-DHAVE_VISIBILITY)
  ENDIF (GCC_IS_NEWER_THAN_4_2 AND __CHECK_GCC_VISIBILITY)

  IF (__CHECK_GCC_STACK_PROTECTOR)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector -Wstack-protector")
    IF(__CHECK_GCC_FORTIFY_SOURCE)
      SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=2")
    ENDIF(__CHECK_GCC_FORTIFY_SOURCE)
  ENDIF (__CHECK_GCC_STACK_PROTECTOR)

  IF (__CHECK_GCC_FPIC)
    SET (LIB_FPIC_CXXFLAGS "-fPIC")
  ELSE (__CHECK_GCC_FPIC)
    SET (LIB_FPIC_CXXFLAGS "")
  ENDIF (__CHECK_GCC_FPIC)

  IF (__CHECK_GCC_FPIE)
    SET (APP_PIE_CXXFLAGS "-fPIE")
    SET (APP_PIE_LDFLAGS "-pie")
  ELSE (__CHECK_GCC_FPIE)
    SET (APP_PIE_CXXFLAGS "")
    SET (APP_PIE_LDFLAGS "")
  ENDIF (__CHECK_GCC_FPIE)

  IF (__CHECK_LINK_RDYNAMIC)
    SET (LIB_RDYNAMIC_CXXFLAGS "-rdynamic")
  ELSE (__CHECK_LINK_RDYNAMIC)
    SET (LIB_RDYNAMIC_CXXFLAGS "")
  ENDIF (__CHECK_LINK_RDYNAMIC)

  IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -W -Wabi -Wextra -Wno-long-long -Wctor-dtor-privacy -Wreorder -Wold-style-cast -Woverloaded-virtual -DXHTMLDBG_DEBUG")
    IF (XHTMLDBG_BUILD_VERBOSE)
      SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DXHTMLDBG_DEBUG_VERBOSE")
    ENDIF (XHTMLDBG_BUILD_VERBOSE)
  ELSE (CMAKE_BUILD_TYPE STREQUAL "Debug")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -W -Wno-long-long")
  ENDIF (CMAKE_BUILD_TYPE STREQUAL "Debug")

ENDIF (CMAKE_COMPILER_IS_GNUCXX)

##############################################################
## APPEND|REMOVE DEFINITIONS
##############################################################

REMOVE_DEFINITIONS (-DQT3_SUPPORT_WARNINGS -DQT3_SUPPORT)

##############################################################
## SET Working output Directory
##############################################################

SET (ARCHIVE_OUTPUT_PATH
  ${CMAKE_CURRENT_BINARY_DIR}/app
)

SET (EXECUTABLE_OUTPUT_PATH
  ${CMAKE_CURRENT_BINARY_DIR}/app
)

SET (LIBRARY_OUTPUT_PATH
  ${CMAKE_CURRENT_BINARY_DIR}/app
)

SET (PLUGIN_OUTPUT_PATH
  ${CMAKE_CURRENT_BINARY_DIR}/app/plugins
)

SET (PARSER_SCHEME_PATH
  ${CMAKE_CURRENT_BINARY_DIR}/app/share/xhtmldbg/schemas
)

##############################################################
## Configure files
##############################################################

CONFIGURE_FILE (version.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/version.h
)

##############################################################
## D-Bus-1
##############################################################

EXEC_PROGRAM (${PKG_CONFIG_EXECUTABLE}
  ARGS "--variable=session_bus_services_dir dbus-1"
  OUTPUT_VARIABLE DBUS_SESSION_BUS_SERVICES_DIR
)

CONFIGURE_FILE (de.hjcms.xhtmldbg.service.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/de.hjcms.xhtmldbg.service
)

INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/de.hjcms.xhtmldbg.service
  DESTINATION ${DBUS_SESSION_BUS_SERVICES_DIR}
)

##############################################################
## Default Configuration File
##############################################################

FIND_FILE (CA_BUNDLE_FILE
  NAMES ca-certificates.crt ca-bundle.crt
  PATHS /etc/ssl /etc/ssl/certs /usr/share/ssl /usr/local/share/ssl
  DOC "Find SSL Certificate verify location (CAfile)" NO_CMAKE_PATH
)

FIND_FILE (CSS_VALIDATOR_JAR
  NAMES css-validator.jar
  PATHS /usr/share/java /usr/local/share/java /opt/w3c-css-validator /usr/local/w3c-css-validator
  DOC "Find W3C CSS Validator File" NO_CMAKE_PATH
)

CONFIGURE_FILE (xhtmldbg.conf.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/xhtmldbg.conf
)

INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/xhtmldbg.conf
  DESTINATION /etc/xdg/hjcms.de
)

##############################################################
## INCLUDES
##############################################################

SET (XHTMLDBG_INCLUDES
  ${QT_QTMAIN_INCLUDE_DIR}
  ${RAPTOR_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

SET (XHTMLDBG_INCLUDE_INTERFACE
  ${CMAKE_CURRENT_BINARY_DIR}/src/interface
  ${CMAKE_CURRENT_SOURCE_DIR}/src/interface
)

##############################################################
## LINKER PATHS
##############################################################

SET (XHTMLDBG_LIBRARIES
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
  ${QT_QTWEBKIT_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  ${QT_QTXML_LIBRARY}
  ${RAPTOR_LIBRARIES}
  ${QTIDY_LIBRARIES}
  ${GEOIP_LIBRARIES}
)

##############################################################
## Extensions
##############################################################

## needed by hostinfo plugin
FIND_PROGRAM (BIND_UTIL_DIG NAMES dig bind-dig PATHS /bin /usr/bin)


##############################################################
## SUBDIRECTORIES
##############################################################
ADD_SUBDIRECTORY (src)
ADD_SUBDIRECTORY (data)
ADD_SUBDIRECTORY (i18n)

##############################################################
## LGPL/GPL3 Exceptions
##############################################################

INSTALL (FILES
  NEWS
  README
  AUTHORS
  ChangeLog
  COPYING
  DESTINATION share/xhtmldbg
)

##############################################################
## Packager Section
##############################################################

SET (CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
SET (CPACK_PACKAGE_DESCRIPTION_SUMMARY "a XHTML/HTML Debugger for Web Development")
SET (CPACK_PACKAGE_VERSION_MAJOR ${XHTMLDBG_VERSION_MAJOR})
SET (CPACK_PACKAGE_VERSION_MINOR ${XHTMLDBG_VERSION_MINOR})
SET (CPACK_PACKAGE_VERSION_PATCH "${XHTMLDBG_VERSION_RELEASE}${XHTMLDBG_PATCHLEVEL}")
SET (CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
SET (CPACK_PACKAGE_CONTACT "Juergen Heinemann (Undefined) http://www.hjcms.de")
SET (CPACK_PACKAGE_LICENSE "GPLv3 LGPLv2+")
SET (CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/src/icons/qtidy.png")

CONFIGURE_FILE (xhtmldbg.spec.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/xhtmldbg.spec
)

INCLUDE (CPack)

## EOF
