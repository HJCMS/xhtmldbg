# CMakeLists.txt

SET (networker_SOURCES
  networkaccessmanager.cpp
  networksettings.cpp
  networkcookie.cpp
  authenticationdialog.cpp
  certdialog.cpp
  errorsdialog.cpp
  autosaver.cpp
)

SET (networker_HEADERS
  networkaccessmanager.h
  networksettings.h
  networkcookie.h
  authenticationdialog.h
  certdialog.h
  errorsdialog.h
  autosaver.h
)

SET (networker_NOMOC
)

QT4_WRAP_CPP (networker_MOC ${networker_HEADERS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

ADD_LIBRARY (Networker STATIC
  ${networker_MOC}
  ${networker_NOMOC}
  ${networker_HEADERS}
  ${networker_SOURCES}
)

TARGET_LINK_LIBRARIES (Networker ${XHTMLDBG_LIBRARIES} )

## EOF
