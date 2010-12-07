# - Try to find QtCrypto (Qt Cryptography Architecture 2)
# Once done this will define
#
#  QTCRYPTO_FOUND - system has QtCrypto
#  QTCRYPTO_INCLUDES - the QCA2 include directory
#  QTCRYPTO_LIBRARIES - the libraries needed to use QtCrypto
#  QTCRYPTO_DEFINITIONS - Compiler switches required for using QtCrypto
#
# Copyright (c) 2010, Juergen Heinemann (Undefine) http://hjcms.de
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

SET (QTCRYPTO_FOUND 0)

IF (NOT QT4_FOUND)
  FIND_PACKAGE (Qt4 4.6.0 COMPONENTS QtCore QtGui QtXml REQUIRED)
ENDIF(NOT QT4_FOUND)

IF (NOT PKG_CONFIG_FOUND)
  FIND_PACKAGE (PkgConfig REQUIRED)
ENDIF(NOT PKG_CONFIG_FOUND)

MESSAGE (STATUS "Looking for QtCrypto")


SET (QTCRYPTO_INC_SEARCH_PATH
  /usr/include/QtCrypto
  /usr/local/include/QtCrypto
  ${QT_INCLUDE_DIR}/QtCrypto
  "$ENV{ProgramFiles}/QtCrypto"
)
FIND_PATH (QTCRYPTO_INCLUDES qca.h ${QTCRYPTO_INC_SEARCH_PATH})

IF (QTCRYPTO_INCLUDES)
  SET (QTCRYPTO_FOUND 1)
ENDIF (QTCRYPTO_INCLUDES)

pkg_check_modules (QCA2 REQUIRED qca2>=2.0.2)

FIND_LIBRARY (QTCRYPTO_LIBRARY
  NAMES qca qca2
  PATHS /usr/local/lib /usr/lib /usr/local/lib64 /usr/lib64 ${QT_LIBRARY_DIR} ENV LD_LIBRARY_PATH
)

IF (NOT QTCRYPTO_LIBRARY)
  SET (QTCRYPTO_FOUND 0)
ENDIF (NOT QTCRYPTO_LIBRARY)

IF (QTCRYPTO_FOUND)
  MESSAGE (STATUS " Found Version ${QCA2_VERSION}")
  MESSAGE (STATUS " Found includedir ${QTCRYPTO_INCLUDES}")
  MESSAGE (STATUS " Found library ${QTCRYPTO_LIBRARY}")
  ADD_DEFINITIONS (-DHAVE_QTCRYPTO)
ELSE (QTCRYPTO_FOUND)
  IF (QTCRYPTO_FIND_REQUIRED)
    MESSAGE (FATAL_ERROR "Could NOT found QtCrypto(libqca) v2. Check ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log for more details.")
  ELSE (QTCRYPTO_FIND_REQUIRED)
    MESSAGE (STATUS "Notice - QtCrypto(libqca) v2 not Found")
  ENDIF (QTCRYPTO_FIND_REQUIRED)
ENDIF (QTCRYPTO_FOUND)

MARK_AS_ADVANCED (QTCRYPTO_LIBRARY QTCRYPTO_INCLUDES)

##EOF
