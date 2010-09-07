# CMakeLists.txt
##############################################################
PROJECT (xhtmldbg)

CMAKE_MINIMUM_REQUIRED (VERSION 2.8.0 FATAL_ERROR)

##############################################################
# CMP0002: we have multiple targets with the same name for the unit tests
##############################################################
CMAKE_POLICY (VERSION 2.8)
ENABLE_LANGUAGE (CXX)

##############################################################
# where to look first for cmake modules, before ${CMAKE_ROOT}/cmake/Modules/ is checked
##############################################################
SET (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules")

##############################################################
## Package
##############################################################
SET (QT_MIN_VERSION 4.6.0)
FIND_PACKAGE (PkgConfig REQUIRED)
FIND_PACKAGE (Qt4 REQUIRED)
FIND_PACKAGE (Qt4 COMPONENTS QTWEBKIT QTNETWORK QTDBUS QTXMLPATTERNS QTSQL QTUITOOLS QTSCRIPT)
FIND_PACKAGE (QTidy REQUIRED)
FIND_PACKAGE (OpenSSL REQUIRED)
FIND_PACKAGE (Raptor REQUIRED)
FIND_PACKAGE (GeoIP REQUIRED)
FIND_PACKAGE (Java)
FIND_PACKAGE (MozillaPluginAPI)

##############################################################
## Set xhtmldbg Specific Information
##############################################################

SET (XHTMLDBG_VERSION_MAJOR 0)
SET (XHTMLDBG_VERSION_MINOR 8)
SET (XHTMLDBG_VERSION_RELEASE 10)
SET (XHTMLDBG_PATCHLEVEL ".rc11")
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
  INCLUDE (CheckingGNUCXX)
ENDIF (CMAKE_COMPILER_IS_GNUCXX)

##############################################################
## APPEND|REMOVE DEFINITIONS
##############################################################

IF (GEOIP_FOUND)
  ADD_DEFINITIONS (${GEOIP_DEFINITIONS})
ENDIF (GEOIP_FOUND)

IF (MOZILLA_PLUGIN_API_FOUND)
  ADD_DEFINITIONS (${MOZILLA_PLUGIN_API_DEFINITION})
ENDIF (MOZILLA_PLUGIN_API_FOUND)

IF (QT_QTUITOOLS_FOUND)
  ADD_DEFINITIONS ("-DHAVE_QTUITOOLS")
ENDIF (QT_QTUITOOLS_FOUND)

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
## D-Bus-1 Service Directory
##############################################################

EXECUTE_PROCESS (
  COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=session_bus_services_dir dbus-1
  OUTPUT_VARIABLE _dbus_services_dir
  RESULT_VARIABLE _dbus_services_dir_failed
)

FIND_PATH (DBUS_DATA_INSTALL_DIR services interfaces
  PATHS
  /usr/share/dbus-1
  /usr/local/share/dbus-1
  ${_dbus_services_dir}
  $ENV{KDEDIRS}"/share/dbus-1"
  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\GNU]/share/dbus-1"
  DOC "Find D-Bus Files directory"
)

IF (NOT DBUS_DATA_INSTALL_DIR)
  MESSAGE (FATAL_ERROR "Can not find D-Bus data Directory for interface and service files")
ENDIF(NOT DBUS_DATA_INSTALL_DIR)

CONFIGURE_FILE (de.hjcms.xhtmldbg.service.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/de.hjcms.xhtmldbg.service
)

INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/de.hjcms.xhtmldbg.service
  DESTINATION ${DBUS_DATA_INSTALL_DIR}/services
)

INSTALL (FILES de.hjcms.xhtmldbg.xml
  DESTINATION ${DBUS_DATA_INSTALL_DIR}/interfaces
)

MAKE_DIRECTORY (browser-plugins)
INSTALL (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/browser-plugins
  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/xhtmldbg
)

##############################################################
## OpenSSL Certification Bundle
##############################################################

FIND_FILE (CA_BUNDLE_FILE
  NAMES ca-bundle.pem ca-certificates.crt
  PATHS
    /var/lib/ca-certificates
    /etc/ssl/certs
    /etc/ca-certificates
    /usr/lib/ca-certificates
    /usr/share/ca-certificates
    /usr/local/share/ca-certificates
  DOC "Find SSL Certificate verify location (CAfile)" NO_CMAKE_PATH
)

##############################################################
## W3C CSS Validator Runtime Files
##############################################################

FIND_FILE (CSS_VALIDATOR_JAR
  NAMES css-validator.jar w3c-css-validator.jar
  PATHS
    /usr/share/java
    /usr/share/java/w3c-css-validator
    /usr/local/share
    /usr/local/share/w3c-css-validator
    /usr/lib/java
    /var/lib/java
    /opt/w3c-css-validator
    /usr/local/w3c-css-validator
  DOC "Find W3C CSS Validator File" NO_CMAKE_PATH
)

##############################################################
## INCLUDEDIRS
##############################################################

INCLUDE_DIRECTORIES (
  ${QT_INCLUDE_DIR}
  ${QTIDY_INCLUDE_DIR}
  ${RAPTOR_INCLUDE_DIR}
)

SET (XHTMLDBG_INCLUDES
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
## Library Dependeces
##############################################################

SET (XHTMLDBG_LIBRARIES
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
  ${QT_QTWEBKIT_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  ${QT_QTXML_LIBRARY}
  ${QT_QTSQL_LIBRARY}
  ${RAPTOR_LIBRARIES}
  ${QTIDY_LIBRARIES}
  ${GEOIP_LIBRARIES}
  ${OPENSSL_LIBRARIES}
)

##############################################################
## Extension Extras
##############################################################

## needed by hostinfo plugin
FIND_PROGRAM (BIND_UTIL_DIG NAMES dig bind-dig PATHS /bin /usr/bin)

##############################################################
## Oxygen Icon Theme
##############################################################

FIND_PATH (ICON_THEME_PATH
  NAMES oxygen/index.theme
  PATHS
  /usr/share/icons
  /usr/local/share/icons
  $ENV{KDEDIRS}"/share/icons"
  ${CMAKE_GENERIC_PROGRAM_FILES}"/kde/share/icons"
  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\GNU]/share/icons"
  DOC "path location of oxygen icon theme"
)

IF (NOT ICON_THEME_PATH)
  MESSAGE (FATAL_ERROR  "xhtmldbg: Oxygen Theme Path not found!\nFix it with: -DICON_THEME_PATH:PATH=<path>")
ELSE (NOT ICON_THEME_PATH)
  MESSAGE (STATUS  "Found Oxygen Icon Theme in: \"${ICON_THEME_PATH}\"")
ENDIF (NOT ICON_THEME_PATH)

##############################################################
## Configure Versions Header
##############################################################

CONFIGURE_FILE (version.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/version.h
)

CONFIGURE_FILE (xhtmldbg.conf.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/xhtmldbg.conf
)

INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/xhtmldbg.conf
  DESTINATION /etc/xdg/hjcms.de
)

##############################################################
## SUBDIRECTORIES
##############################################################
ADD_SUBDIRECTORY (src)
ADD_SUBDIRECTORY (data)
ADD_SUBDIRECTORY (i18n)

##############################################################
## LGPLv2+/GPLv3 Exceptions
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
INCLUDE (DistributePackager)

## EOF
