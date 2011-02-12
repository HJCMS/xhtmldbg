# CMakeLists.txt

SET (networker_SOURCES
  networkaccessmanager.cpp
  networksettings.cpp
  networkcookie.cpp
  authenticationdialog.cpp
  certdialog.cpp
  errorsdialog.cpp
  networkcache.cpp
  localsource.cpp
)

SET (networker_HEADERS
  networkaccessmanager.h
  networksettings.h
  networkcookie.h
  authenticationdialog.h
  certdialog.h
  errorsdialog.h
  networkcache.h
)

SET (networker_NO_MOCS_HEADER
  localsource.h
)

KDE4_MOC_HEADERS (networker_MOC ${networker_HEADERS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

KDE4_ADD_LIBRARY (Networker STATIC
  ${networker_MOC}
  ${networker_NO_MOCS_HEADER}
  ${networker_HEADERS}
  ${networker_SOURCES}
)

SET_TARGET_PROPERTIES (Networker PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (Networker
  AutoSaver
  Settings
  CookiesDatabase
  ${XHTMLDBG_QT_LIBRARIES}
  ${XHTMLDBG_KDE_LIBRARIES}
  ${OPENSSL_LIBRARIES}
)

## EOF
