# - Try to find the Mozilla Plug-In SDK (http://www.mozilla.org)
# Once done this will define
#
#  MOZILLA_PLUGIN_API_FOUND        - system has Mozilla Plug-In API
#  MOZILLA_PLUGIN_API_LIBRARIES    - Link these to use Mozilla Plug-In API
#  MOZILLA_PLUGIN_API_INCLUDE_DIRS - Include directory for using Mozilla Plug-In API
#  MOZILLA_PLUGIN_API_DEFINITION   - Compiler switches required for using Mozilla Plug-In API
#  MOZILLA_PLUGIN_API_VERSION      - The Mozilla Plug-In API version string

# (c) 2009-2010 Juergen Heinemann (Undefined) http://www.hjcms.de
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

FIND_PACKAGE (PkgConfig)

INCLUDE (MacroEnsureVersion)

IF (MOZILLA_PLUGIN_API_INCLUDE_DIR AND MOZILLA_PLUGIN_API_LIBRARIES)
  SET(MozPluginAPI_FIND_QUIETLY TRUE)
ELSE (MOZILLA_PLUGIN_API_INCLUDE_DIR AND MOZILLA_PLUGIN_API_LIBRARIES)
  SET(MozPluginAPI_FIND_QUIETLY FALSE)
ENDIF (MOZILLA_PLUGIN_API_INCLUDE_DIR AND MOZILLA_PLUGIN_API_LIBRARIES)

IF (NOT WIN32)
  IF (MozPluginAPI_FIND_REQUIRED)
    SET (FORCE_REQUIRED "REQUIRED")
  ELSE (MozPluginAPI_FIND_REQUIRED)
    SET (FORCE_REQUIRED "")
  ENDIF (MozPluginAPI_FIND_REQUIRED)
  PKG_CHECK_MODULES (PC_MOZILLA_PLUGIN_API ${FORCE_REQUIRED} mozilla-plugin>=1.9.0)
  PKG_CHECK_MODULES (PC_NSPR ${FORCE_REQUIRED} nspr>=4.7.1)
ENDIF (NOT WIN32)

FIND_PATH (MOZILLA_PLUGIN_API_INCLUDE_DIR npfunctions.h npupp.h
  PATHS
  ${PC_MOZILLA_PLUGIN_API_INCLUDEDIR}
  ${PC_MOZILLA_PLUGIN_API_INCLUDE_DIRS}
)

FIND_PATH (MOZILLA_PLUGIN_NSPR_INCLUDE_DIR nspr.h
  PATHS
  ${PC_MOZILLA_PLUGIN_API_INCLUDE_DIRS}
  ${PC_MOZILLA_PLUGIN_API_INCLUDEDIR}
  ${PC_NSPR_INCLUDE_DIRS}
)

SET (MOZILLA_PLUGIN_API_INCLUDE_DIRS
  ${MOZILLA_PLUGIN_API_INCLUDE_DIR}
  ${MOZILLA_PLUGIN_NSPR_INCLUDE_DIR}
)

IF (PC_NSPR_INCLUDE_DIRS AND PC_NSPR_LIBDIR)
  SET (NSPR_VERSION ${PC_NSPR_VERSION})
  FIND_LIBRARY(_nspr_plds_lib NAMES plds4
    PATHS
    ${PC_MOZILLA_PLUGIN_API_LIBRARY_DIRS}
    ${PC_NSPR_LIBDIR}
  )
  FIND_LIBRARY(_nspr_plc_lib NAMES plc4
    PATHS
    ${PC_NSPR_LIBDIR}
    ${PC_MOZILLA_PLUGIN_API_LIBRARY_DIRS}
  )
  FIND_LIBRARY(_nspr_nspr_lib NAMES nspr4
    PATHS
    ${PC_NSPR_LIBDIR}
    ${PC_MOZILLA_PLUGIN_API_LIBRARY_DIRS}
  )
ENDIF (PC_NSPR_INCLUDE_DIRS AND PC_NSPR_LIBDIR)

SET (MOZILLA_PLUGIN_API_LIBRARIES
  ${_nspr_plds_lib}
  ${_nspr_plc_lib}
  ${_nspr_nspr_lib}
)

MACRO_ENSURE_VERSION ( "1.9.0" ${PC_MOZILLA_PLUGIN_API_VERSION} USE_OLD_NPFUNCTIONS )

IF (MOZILLA_PLUGIN_API_INCLUDE_DIRS AND MOZILLA_PLUGIN_API_LIBRARIES)
  SET (MOZILLA_PLUGIN_API_FOUND TRUE)
  SET (MOZILLA_PLUGIN_API_VERSION ${PC_MOZILLA_PLUGIN_API_VERSION})
  SET (MOZILLA_PLUGIN_API_DEFINITION "-DHAVE_MOZILLA_PLUGIN_API")
  IF (USE_OLD_NPFUNCTIONS)
    SET (MOZILLA_PLUGIN_API_DEFINITION "${MOZILLA_PLUGIN_API_DEFINITION} -DHAVE_OLD_NPUPP")
  ENDIF (USE_OLD_NPFUNCTIONS)
ELSE (MOZILLA_PLUGIN_API_INCLUDE_DIRS AND MOZILLA_PLUGIN_API_LIBRARIES)
  SET (MOZILLA_PLUGIN_API_FOUND FALSE)
ENDIF (MOZILLA_PLUGIN_API_INCLUDE_DIRS AND MOZILLA_PLUGIN_API_LIBRARIES)

IF (MOZILLA_PLUGIN_API_FOUND)
  IF (NOT MozPluginAPI_FIND_QUIETLY)
    MESSAGE (STATUS "Found Mozilla Plug-In API libraries: ${MOZILLA_PLUGIN_API_LIBRARIES}")
    MESSAGE (STATUS "Found Mozilla Plug-In API includes : ${MOZILLA_PLUGIN_API_INCLUDE_DIRS}")
    MESSAGE (STATUS "Found Mozilla Plug-In definition   : ${MOZILLA_PLUGIN_API_DEFINITION}")
    MESSAGE (STATUS "Found Mozilla Plug-In API version  : ${MOZILLA_PLUGIN_API_VERSION}")
  ENDIF (NOT MozPluginAPI_FIND_QUIETLY)
ELSE (MOZILLA_PLUGIN_API_FOUND)
  IF (MozPluginAPI_FIND_REQUIRED)
    MESSAGE (STATUS "Could NOT find Mozilla Plug-In SDK (Xulrunner)")
  ENDIF (MozPluginAPI_FIND_REQUIRED)
ENDIF (MOZILLA_PLUGIN_API_FOUND)

MARK_AS_ADVANCED (MOZILLA_PLUGIN_API_INCLUDE_DIR _nspr_plds_lib _nspr_plc_lib _nspr_nspr_lib)

##EOF
