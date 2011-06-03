# - Try to find the Raptor RDF parsing library (http://librdf.org/raptor/)
# Once done this will define
#
#  RAPTOR_FOUND       - system has Raptor
#  RAPTOR_LIBRARY     - Link these to use Raptor
#  RAPTOR_INCLUDE_DIR - Include directory for using Raptor
#  RAPTOR_DEFINITIONS - Compiler switches required for using Raptor
#  RAPTOR_VERSION     - The Raptor version string

# (c) 2007-2009 Sebastian Trueg <trueg@kde.org>
#
# Based on FindFontconfig Copyright (c) 2006,2007 Laurent Montel, <montel@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

INCLUDE (MacroEnsureVersion)

IF (FORCE_RAPTOR2)
  MESSAGE (STATUS "Looking for Raptor Major Version 2")

    INCLUDE (FindPkgConfig)
    pkg_check_modules (_raptor2 raptor2>=2.0.3)

    STRING (REPLACE "\n" "" RAPTOR_VERSION ${_raptor2_VERSION})
    MACRO_ENSURE_VERSION ("2.0.3" ${RAPTOR_VERSION} RAPTOR_HAVE_TRIG)

    IF(_raptor2_FOUND)

      SET (RAPTOR_DEFINITIONS "${_raptor2_CFLAGS} -DHAVE_RAPTOR2")

      FIND_LIBRARY (RAPTOR_LIBRARY
        NAMES raptor2
        PATHS
        ${_raptor2_LIBRARY_DIRS}
        /usr/local/lib${LIB_SUFFIX}
        /usr/lib${LIB_SUFFIX}
      )

      FIND_PATH (RAPTOR_INCLUDE_DIR
        NAMES raptor2.h
        PATHS
        /usr/include
        /usr/local/include
        ${_raptor2_INSTALL_DIRS}
        PATH_SUFFIXES raptor2
      )

    ENDIF (_raptor2_FOUND)

    IF (RAPTOR_INCLUDE_DIR AND RAPTOR_LIBRARY)
      SET (RAPTOR_FOUND TRUE)
    ENDIF (RAPTOR_INCLUDE_DIR AND RAPTOR_LIBRARY)

    IF (RAPTOR_FOUND)
      IF (NOT Raptor_FIND_QUIETLY)
        MESSAGE (STATUS " Found Raptor ${RAPTOR_VERSION}: libs - ${RAPTOR_LIBRARY}; includes - ${RAPTOR_INCLUDE_DIR}")
      ENDIF (NOT Raptor_FIND_QUIETLY)
    ELSE (RAPTOR_FOUND)
      IF (Raptor_FIND_REQUIRED)
        MESSAGE (FATAL_ERROR " Could NOT find Raptor Version 2")
      ENDIF (Raptor_FIND_REQUIRED)
    ENDIF (RAPTOR_FOUND)

    MARK_AS_ADVANCED (RAPTOR_DEFINITIONS RAPTOR_INCLUDE_DIR RAPTOR_LIBRARY)

ELSE (FORCE_RAPTOR2)
  MESSAGE (STATUS "Looking for Raptor Major Version 1")

  FIND_PROGRAM (RAPTOR_CONFIG NAMES raptor-config)

  IF (RAPTOR_CONFIG)
    EXECUTE_PROCESS (COMMAND ${RAPTOR_CONFIG} --version OUTPUT_VARIABLE RAPTOR_VERSION)

    IF (RAPTOR_VERSION)
      STRING (REPLACE "\n" "" RAPTOR_VERSION ${RAPTOR_VERSION})

      MACRO_ENSURE_VERSION ("1.4.19" ${RAPTOR_VERSION} RAPTOR_HAVE_TRIG)

      # extract include paths from raptor-config
      EXECUTE_PROCESS (COMMAND ${RAPTOR_CONFIG} --cflags OUTPUT_VARIABLE raptor_CFLAGS_ARGS)
      STRING (REPLACE " " ";" raptor_CFLAGS_ARGS ${raptor_CFLAGS_ARGS})
      FOREACH (_ARG ${raptor_CFLAGS_ARGS})
        IF (${_ARG} MATCHES "^-I")
          STRING (REGEX REPLACE "^-I" "" _ARG ${_ARG})
          STRING (REPLACE "\n" "" _ARG ${_ARG})
          LIST (APPEND raptor_INCLUDE_DIRS ${_ARG})
        ENDIF (${_ARG} MATCHES "^-I")
      ENDFOREACH (_ARG)

      # extract lib paths from raptor-config
      EXECUTE_PROCESS (
        COMMAND ${RAPTOR_CONFIG} --libs
        OUTPUT_VARIABLE raptor_CFLAGS_ARGS)
      STRING ( REPLACE " " ";" raptor_CFLAGS_ARGS ${raptor_CFLAGS_ARGS})
      FOREACH ( _ARG ${raptor_CFLAGS_ARGS})
        IF (${_ARG} MATCHES "^-L")
          STRING (REGEX REPLACE "^-L" "" _ARG ${_ARG})
          LIST (APPEND raptor_LIBRARY_DIRS ${_ARG})
        ENDIF (${_ARG} MATCHES "^-L")
      ENDFOREACH (_ARG)

    ENDIF (RAPTOR_VERSION)

  ELSE (RAPTOR_CONFIG)
    SET (RAPTOR_VERSION "1.0.0")
  ENDIF (RAPTOR_CONFIG)

  FIND_PATH (RAPTOR_INCLUDE_DIR raptor.h
    PATHS
    ${redland_INCLUDE_DIRS}
    ${raptor_INCLUDE_DIRS}
    /usr/X11/include
    PATH_SUFFIXES redland
  )

  FIND_LIBRARY (RAPTOR_LIBRARY NAMES raptor libraptor
    PATHS
    ${raptor_LIBRARY_DIRS}
  )

  IF (RAPTOR_INCLUDE_DIR AND RAPTOR_LIBRARY)
    SET (RAPTOR_FOUND TRUE)
  ENDIF (RAPTOR_INCLUDE_DIR AND RAPTOR_LIBRARY)

  IF (RAPTOR_FOUND)
    SET (RAPTOR_DEFINITIONS ${raptor_CFLAGS})
    IF (NOT Raptor_FIND_QUIETLY)
      MESSAGE (STATUS " Found Raptor ${RAPTOR_VERSION}: libs - ${RAPTOR_LIBRARY}; includes - ${RAPTOR_INCLUDE_DIR}")
    ENDIF (NOT Raptor_FIND_QUIETLY)
  ELSE (RAPTOR_FOUND)
    IF (Raptor_FIND_REQUIRED)
      MESSAGE (FATAL_ERROR " Could NOT find Raptor Version 1")
    ENDIF (Raptor_FIND_REQUIRED)
  ENDIF (RAPTOR_FOUND)

  MARK_AS_ADVANCED (RAPTOR_INCLUDE_DIR_TMP RAPTOR_INCLUDE_DIR RAPTOR_LIBRARY)
ENDIF (FORCE_RAPTOR2)
