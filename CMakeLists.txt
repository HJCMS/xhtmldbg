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
## Requiered "QtCore QtGui QtXml QtNetwork"
##############################################################
SET (QT_MIN_VERSION 4.6.0)
FIND_PACKAGE (Qt4 REQUIRED)
SET (QT_USE_QTGUI 1)
SET (QT_USE_QTXML 1)
SET (QT_USE_QTNETWORK 1)
SET (QT_USE_QTWEBKIT 1)
SET (QT_USE_QTSQL 1)

INCLUDE (${QT_USE_FILE})

##############################################################
## Set xhtmldbg Specific Information
##############################################################

SET (XHTMLDBG_VERSION_MAJOR 0)
SET (XHTMLDBG_VERSION_MINOR 8)
SET (XHTMLDBG_VERSION_RELEASE 0)
SET (XHTMLDBG_VERSION "${XHTMLDBG_VERSION_MAJOR}.${XHTMLDBG_VERSION_MINOR}.${XHTMLDBG_VERSION_RELEASE}")
SET (XHTMLDBG_VERSION_STRING "${XHTMLDBG_VERSION}")
SET (XHTMLDBG_DEFINITIONS "-DXHTMLDBG_VERSION_STRING=\"${XHTMLDBG_VERSION}\"")
SET (XHTMLDBG_DISTRIBUTION_TEXT "compiled sources" CACHE STRING "Indicate the distribution in bug reports")
SET (LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)")
SET (XHTMLDBG_CXXFLAGS "" CACHE STRING "Defined in pkg-config file")
SET (XHTMLDBG_REQUIRES "" CACHE STRING "Defined in pkg-config file")
SET (XHTMLDBG_RESOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/data/qrcimages.qrc")
SET (CMAKE_INSTALL_RPATH_USE_LINK_PATH  TRUE)

EXEC_PROGRAM (${QT_QMAKE_EXECUTABLE}
  ARGS "-query QT_INSTALL_PREFIX"
  OUTPUT_VARIABLE CMAKE_INSTALL_PREFIX
)

## Icon Theme
SET (ICON_THEME_FOLDER "" CACHE STRING "icontheme directory foldername (oxygen/hicolor) default is oxygen")
IF (NOT ICON_THEME_FOLDER)
MESSAGE (STATUS "Notice setting ICON_THEME_FOLDER to oxygen!")
SET (ICON_THEME_FOLDER "oxygen" CACHE STRING "oxygen" FORCE)
ENDIF (NOT ICON_THEME_FOLDER)

##############################################################
## Set Header Definitions
##############################################################

INCLUDE (CheckCXXCompilerFlag)
INCLUDE (MacroEnsureVersion)

IF (CMAKE_COMPILER_IS_GNUCXX)

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

  IF (__CHECK_GCC_VISIBILITY AND GCC_IS_NEWER_THAN_4_1)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")

    IF (GCC_IS_NEWER_THAN_4_2)
        SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility-inlines-hidden")
    ENDIF (GCC_IS_NEWER_THAN_4_2)

  ENDIF (__CHECK_GCC_VISIBILITY AND GCC_IS_NEWER_THAN_4_1)

  IF (GCC_IS_NEWER_THAN_4_2 AND __CHECK_GCC_VISIBILITY)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_VISIBILITY")
    ## Bugfix gcc-4.3 need explicit definition
    ADD_DEFINITIONS (${QT_DEFINITIONS} -DHAVE_VISIBILITY)
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

  IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -W -Wextra -pedantic -Wno-long-long -DXHTMLDBG_DEBUG")
  ELSE (CMAKE_BUILD_TYPE STREQUAL "Debug")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  ENDIF (CMAKE_BUILD_TYPE STREQUAL "Debug")

ENDIF (CMAKE_COMPILER_IS_GNUCXX)

##############################################################
## FATAL ERRORS
##############################################################

IF (NOT QT_USE_QTXML)
  MESSAGE (FATAL_ERROR  "xhtmldbg: Requires Qt4 with QtXml support.")
ENDIF (NOT QT_USE_QTXML)

IF (NOT QT_USE_QTNETWORK)
  MESSAGE (FATAL_ERROR  "xhtmldbg: Requires Qt4 with QtNetwork support.")
ENDIF (NOT QT_USE_QTNETWORK)

IF (NOT QT_USE_QTWEBKIT)
  MESSAGE (FATAL_ERROR  "xhtmldbg: Requires Qt4 with QtWebKit support.")
ENDIF (NOT QT_USE_QTWEBKIT)


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

##############################################################
## INCLUDES
##############################################################

SET (XHTMLDBG_INCLUDES
  ${QT_INCLUDES}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/lib
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/QTidy
)

##############################################################
## LINKER PATHS
##############################################################

SET (XHTMLDBG_LIBRARIES
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTXML_LIBRARY}
  ${QT_QTWEBKIT_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
)

##############################################################
## Search Tidy
##############################################################

SET (TIDY_FOUND 0)
SET (TIDY_INC_SEARCH_PATH
  /usr/include
  /usr/include/tidy
  /usr/local/include/tidy
)
FIND_PATH (TIDY_INCLUDE_PATH  tidy.h ${TIDY_INC_SEARCH_PATH})
FIND_LIBRARY (TIDY_LIBRARY tidy)

IF (TIDY_INCLUDE_PATH OR TIDY_LIBRARY)
  SET (TIDY_FOUND 1)
ENDIF (TIDY_INCLUDE_PATH OR TIDY_LIBRARY)

IF (NOT TIDY_FOUND)
  MESSAGE (FATAL_ERROR "tidy development headers or library not found!")
ELSE (NOT TIDY_FOUND)
  MESSAGE (STATUS "nice tidy development files found!")
  SET (XHTMLDBG_INCLUDES ${XHTMLDBG_INCLUDES} ${TIDY_INCLUDE_PATH})
  SET (XHTMLDBG_LIBRARIES ${XHTMLDBG_LIBRARIES} ${TIDY_LIBRARY})
ENDIF (NOT TIDY_FOUND)

##############################################################
## SUBDIRECTORIES
##############################################################
ADD_SUBDIRECTORY (src)
ADD_SUBDIRECTORY (data)

##############################################################
## LGPL/GPL3 Exceptions
##############################################################

INSTALL (FILES
  NEWS
  README
  AUTHORS
  ChangeLog
  COPYING
  COPYING.lib
  DESTINATION share/xhtmldbg
)

## EOF
