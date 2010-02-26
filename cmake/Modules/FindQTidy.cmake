# FindQTidy.cmake
#
# This module can be used to find QTidy.
# The most important issue is that the Qt4 qmake is available via the system path.
# This qmake is then used to detect basically everything else.
# This module defines a number of key variables and macros. 
# It sets up the compilation environment for include directories,
# preprocessor defines and populates a QTIDY_LIBRARIES variable.
#
# Typical usage could be something like:
#   FIND_PACKAGE(QTidy REQUIRED)
#   ADD_EXECUTABLE(myexe main.cpp)
#   TARGET_LINK_LIBRARIES(myexe ${QTIDY_LIBRARIES})
#
#  QTIDY_DEFINITIONS  Definitions to use when compiling code that uses QTidy.
#  QTIDY_INCLUDES     List of paths to all include directories of Qt4 and QTidy
#                     QT_INCLUDE_DIR and QTIDY_INCLUDE_DIR are always in this variable
#                     even if NOTFOUND, all other INCLUDE_DIRS are only added if they are found.
#
#  Include directories for the Qt modules are listed here.
#  You do not need to use these variables if you include QT_USE_FILE.
#
#  QT_INCLUDE_DIR              Path to "include" of Qt4
# Copyright (c) 2007-2010, Juergen Heinemann (Undefined) <nospam (AT) hjcms (DOT) de>
#
# See COPYING file that comes with this distribution
#
#######################################################################


FIND_PACKAGE ( Qt4 4.5.6 COMPONENTS QtCore QtGui QtXml REQUIRED )
INCLUDE (${QT_USE_FILE})

##############################################################
## FATAL ERRORS
##############################################################

IF (NOT QT_USE_QTXML)
  MESSAGE (FATAL_ERROR  "xhtmldbg: Requires Qt4 with QtXml support.")
ENDIF (NOT QT_USE_QTXML)

##############################################################
## TIDY
##############################################################

SET (TIDY_FOUND 0)
SET (TIDY_INC_SEARCH_PATH
  /usr/include
  /usr/include/tidy
  /usr/local/include/tidy
)
FIND_PATH (TIDY_INCLUDE tidy.h ${TIDY_INC_SEARCH_PATH})

FIND_LIBRARY (TIDY_LIBRARY NAMES tidy tidy-0.99)

IF (TIDY_INCLUDE OR TIDY_LIBRARY)
  SET (TIDY_FOUND 1)
ENDIF (TIDY_INCLUDE OR TIDY_LIBRARY)

IF (NOT TIDY_FOUND)
  MESSAGE (FATAL_ERROR "tidy development file from http://tidy.sourceforge.net not found.")
ELSE (NOT TIDY_FOUND)
  MESSAGE(STATUS "Found tidy [${TIDY_LIBRARY}][${TIDY_INCLUDE}]")
ENDIF (NOT TIDY_FOUND)

##############################################################
## QTIDY
##############################################################

SET (QTIDY_FOUND 0)

MESSAGE (STATUS "Looking for QTidy")

SET (QTIDY_INC_SEARCH_PATH
  /usr/include/QTidy
  /usr/local/include/QTidy
  ${QT_INCLUDE_DIR}/QTidy
)
FIND_PATH (QTIDY_INCLUDE QTidy.h ${QTIDY_INC_SEARCH_PATH})

IF (QTIDY_INCLUDE)
  SET (QTIDY_FOUND 1)
ENDIF (QTIDY_INCLUDE)

FILE (READ "${QTIDY_INCLUDE}/QTidy.h" _QTidy_VERSION_H)
STRING (REGEX REPLACE ".*#define QTIDY_VERSION.*\"([0-9\\.]+[0-9])\".*" "\\1" QTidy_VERSION "${_QTidy_VERSION_H}")
SET (QTIDY_VERSION ${QTidy_VERSION} CACHE INTERNAL "The version number for QTidy library")
SET (QTIDY_DEFINITIONS "-DQTIDY_VERSION_STRING=\"${QTIDY_VERSION}\"")

FIND_LIBRARY (QTIDY_LIBRARY
  NAMES QTidy QTidy-1.0
  PATHS ${QT_LIBRARY_DIR} NO_DEFAULT_PATH
)

IF (QTIDY_FOUND)
  MESSAGE (STATUS " Found version ${QTIDY_VERSION}")
  MESSAGE (STATUS " Found includedir ${QTIDY_INCLUDE}")
  MESSAGE (STATUS " Found library ${QTIDY_LIBRARY}")
ELSE (QTIDY_FOUND)
  MESSAGE (FATAL_ERROR "Could NOT find QTidy. Check ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log for more details.")
ENDIF (QTIDY_FOUND)

INCLUDE_DIRECTORIES (
  ${QTIDY_INCLUDE}
  ${QT_QTCORE_INCLUDE_DIR}
  ${QT_QTGUI_INCLUDE_DIR}
  ${QT_QTXML_INCLUDE_DIR}
  ${TIDY_INCLUDE}
)

##############################################################
## QTIDY_DEFINITIONS
##############################################################

ADD_DEFINITIONS (
  ${QTIDY_DEFINITIONS}
  ${QT_DEFINITIONS}
)

##############################################################
## QTIDY_LIBRARIES
##############################################################

SET(QTIDY_LIBRARIES
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTXML_LIBRARY}
  ${QTIDY_LIBRARY}
  ${TIDY_LIBRARY}
)

#eof

