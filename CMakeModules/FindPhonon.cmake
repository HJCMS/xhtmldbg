# Find phonon
#
#  PHONON_FOUND    - system has Phonon Library
#  PHONON_INCLUDES - the Phonon include directory
#  PHONON_LIBRARY  - link these to use Phonon
#  PHONON_VERSION  - the version of the Phonon Library
#
# Copyright (c) 2011, Juergen Heinemann (Undefined) http://www.hjcms.de
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

SET (PHONON_FOUND 0)

IF (NOT QT4_FOUND)
  FIND_PACKAGE (Qt4 4.6.3 COMPONENTS QtCore QtGui REQUIRED)
ENDIF(NOT QT4_FOUND)

IF (NOT PKG_CONFIG_FOUND)
  FIND_PACKAGE (PkgConfig REQUIRED)
ENDIF(NOT PKG_CONFIG_FOUND)

MESSAGE (STATUS "Looking for Phonon")

pkg_check_modules (_phonon REQUIRED phonon>=4.4.0)

FIND_LIBRARY (PHONON_LIBRARY
  NAMES phonon
  HINTS ${_phonon_LIBRARY_DIRS} ${QT_LIBRARY_DIR}
)

FIND_PATH (PHONON_INCLUDE_DIR
  NAMES phonon/phonon_export.h
  HINTS ${_phonon_INCLUDE_DIRS} ${QT_INCLUDE_DIR}
)

FIND_PATH (PHONON_STL_INCLUDE_DIR
  NAMES KDE/KConfig
  HINTS ${_phonon_INCLUDE_DIRS} ${QT_INCLUDE_DIR}
)

IF (PHONON_INCLUDE_DIR AND PHONON_LIBRARY)
  SET (PHONON_INCLUDES ${PHONON_STL_INCLUDE_DIR}/KDE ${PHONON_STL_INCLUDE_DIR} ${PHONON_INCLUDE_DIR})
  SET (PHONON_VERSION ${_phonon_VERSION})
  SET (PHONON_FOUND 1)
  MESSAGE (STATUS " Found Version ${PHONON_VERSION}")
  MESSAGE (STATUS " Found includedir ${PHONON_INCLUDES}")
  MESSAGE (STATUS " Found library ${PHONON_LIBRARY}")
  ADD_DEFINITIONS (-DHAVE_PHONON)
ENDIF (PHONON_INCLUDE_DIR AND PHONON_LIBRARY)

MARK_AS_ADVANCED (PHONON_INCLUDE_DIR PHONON_LIBRARY)

##EOF
