# - Try to find the Xulrunner Plug-In SDK (http://www.mozilla.org)
# Once done this will define
#
#  XULRUNNER_FOUND        - system has Xulrunner
#  XULRUNNER_LIBRARIES    - Link these to use Xulrunner
#  XULRUNNER_INCLUDE_DIRS - Include directory for using Xulrunner
#  XULRUNNER_DEFINITION   - Compiler switches required for using Xulrunner
#  XULRUNNER_VERSION      - The Xulrunner version string

# (c) 2009-2010 Juergen Heinemann (Undefined) http://www.hjcms.de
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

FIND_PACKAGE (PkgConfig)

SET (XULRUNNER_FOUND FALSE)

IF (XULRUNNER_INCLUDE_DIR AND XULRUNNER_LIBRARIES)
  SET(XULRUNNER_FIND_QUIETLY TRUE)
ELSE (XULRUNNER_INCLUDE_DIR AND XULRUNNER_LIBRARIES)
  SET(XULRUNNER_FIND_QUIETLY FALSE)
ENDIF (XULRUNNER_INCLUDE_DIR AND XULRUNNER_LIBRARIES)

# we need at least version 1.9.2 for mozilla-plugin Interface for Out of process plugins (OOPP)
# @see https://developer.mozilla.org/en/Plugins/Out_of_process_plugins
IF (NOT XULRUNNER_MIN_VERSION)
  SET (XULRUNNER_MIN_VERSION "1.9.2")
ENDIF (NOT XULRUNNER_MIN_VERSION)

# we need at least version 3.12.8 for libnss
IF (NOT NSS_MIN_VERSION)
  SET (NSS_MIN_VERSION "3.12.8")
ENDIF (NOT NSS_MIN_VERSION)

# we need at least version 4.8.6 for libnspr
IF (NOT NSPR_MIN_VERSION)
  SET (NSPR_MIN_VERSION "4.8.6")
ENDIF (NOT NSPR_MIN_VERSION)

IF (XULRUNNER_FIND_REQUIRED)
  SET (_XURLRUNNER_FORCE_REQUIRED "REQUIRED")
ELSE (XULRUNNER_FIND_REQUIRED)
  SET (_XURLRUNNER_FORCE_REQUIRED "")
ENDIF (XULRUNNER_FIND_REQUIRED)

PKG_CHECK_MODULES (_PC_XULRUNNER ${_XURLRUNNER_FORCE_REQUIRED}
  mozilla-plugin>=${XULRUNNER_MIN_VERSION}
  nss>=${NSS_MIN_VERSION}
  nspr>=${NSPR_MIN_VERSION}
)

FIND_PATH (XULRUNNER_INCLUDE_DIR npfunctions.h
  PATHS
  ${_PC_XULRUNNER_mozilla-plugin_INCLUDEDIR}
  ${_PC_XULRUNNER_INCLUDE_DIRS}
)

IF (_PC_XULRUNNER_FOUND AND XULRUNNER_INCLUDE_DIR)

  SET (XULRUNNER_INCLUDES "${XULRUNNER_INCLUDE_DIR};${_PC_XULRUNNER_INCLUDE_DIRS}")
  SET (XULRUNNER_LIBRARIES ${_PC_XULRUNNER_LIBRARIES})

  IF (XULRUNNER_INCLUDES AND XULRUNNER_LIBRARIES)
    SET (XULRUNNER_FOUND TRUE)
    SET (XULRUNNER_VERSION ${_PC_XULRUNNER_mozilla-plugin_VERSION})
    SET (XULRUNNER_DEFINITIONS "${_PC_XULRUNNER_CFLAGS_OTHER}")
  ENDIF (XULRUNNER_INCLUDES AND XULRUNNER_LIBRARIES)

  IF (NOT XULRUNNER_FIND_QUIETLY)
    MESSAGE (STATUS "Found Xulrunner libraries: ${XULRUNNER_LIBRARIES}")
    MESSAGE (STATUS "Found Xulrunner includes : ${XULRUNNER_INCLUDES}")
    MESSAGE (STATUS "Found Xulrunner defines  : ${XULRUNNER_DEFINITIONS}")
    MESSAGE (STATUS "Found Xulrunner version  : ${XULRUNNER_VERSION}")
  ENDIF (NOT XULRUNNER_FIND_QUIETLY)

ENDIF (_PC_XULRUNNER_FOUND AND XULRUNNER_INCLUDE_DIR)

MARK_AS_ADVANCED (XULRUNNER_INCLUDES XULRUNNER_LIBRARIES XULRUNNER_DEFINITIONS)

##EOF