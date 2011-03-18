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
# Copyright (c) 2007-2011, Juergen Heinemann (Undefined) <nospam (AT) hjcms (DOT) de>
#
# See COPYING file that comes with this distribution
#
#######################################################################


FIND_PACKAGE (Qt4 4.6.0 COMPONENTS QtCore QtGui QtXml REQUIRED)

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
  MESSAGE (STATUS "Found tidy [${TIDY_LIBRARY}][${TIDY_INCLUDE}]")
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
  "$ENV{ProgramFiles}/QTidy"
  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\HJCMS.DE\\QTidy]"
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
  PATHS /usr/local/lib /usr/lib /usr/local/lib64 /usr/lib64 ${QT_LIBRARY_DIR} ENV LD_LIBRARY_PATH
)

IF (NOT QTIDY_LIBRARY)
  SET (QTIDY_FOUND 0)
ENDIF (NOT QTIDY_LIBRARY)

IF (QTIDY_FOUND)
  MESSAGE (STATUS " Found Version ${QTIDY_VERSION}")
  MESSAGE (STATUS " Found includedir ${QTIDY_INCLUDE}")
  MESSAGE (STATUS " Found library ${QTIDY_LIBRARY}")
ELSE (QTIDY_FOUND)
  IF (QTidy_FIND_REQUIRED)
    MESSAGE (FATAL_ERROR "Could NOT find QTidy. Check ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log for more details.")
  ELSE (QTidy_FIND_REQUIRED)
    MESSAGE (STATUS "Notice - QTidy not Found")
  ENDIF (QTidy_FIND_REQUIRED)
ENDIF (QTIDY_FOUND)

##############################################################
## QTIDY_DEFINITIONS
##############################################################

ADD_DEFINITIONS (
  -DHAVE_QTIDY
  ${QT_DEFINITIONS}
  ${QTIDY_DEFINITIONS}
)

##############################################################
## QTIDY_INCLUDE_DIR
##############################################################

SET (QTIDY_INCLUDE_DIR
  ${QT_QTCORE_INCLUDE_DIR}
  ${QT_QTGUI_INCLUDE_DIR}
  ${QT_QTXML_INCLUDE_DIR}
  ${QTIDY_INCLUDE}
  ${TIDY_INCLUDE}
)

##############################################################
## QTIDY_LIBRARIES
##############################################################

SET(QTIDY_LIBRARIES
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTXML_LIBRARY}
  ${QTIDY_LIBRARY}
  ${TIDY_LIBRARY}
)

MARK_AS_ADVANCED (QTIDY_LIBRARIES QTIDY_INCLUDE_DIR)

#eof

