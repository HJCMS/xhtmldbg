# CMakeLists.txt

SET (networker_SOURCES
  networkaccessmanager.cpp
  networksettings.cpp
  networkcookie.cpp
  authenticationdialog.cpp
)

SET (networker_HEADERS
  networkaccessmanager.h
  networksettings.h
  networkcookie.h
  authenticationdialog.h
)

SET (networker_NOMOC
)

QT4_WRAP_CPP (networker_MOC ${networker_HEADERS})

ADD_DEFINITIONS (${QTIDY_DEFINITIONS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${QTIDY_BINARY_PROJECT_INCULDE}
)

ADD_LIBRARY (Networker STATIC
  ${networker_MOC}
  ${networker_NOMOC}
  ${networker_HEADERS}
  ${networker_SOURCES}
)

TARGET_LINK_LIBRARIES (Networker ${XHTMLDBG_LIBRARIES} )

## EOF
