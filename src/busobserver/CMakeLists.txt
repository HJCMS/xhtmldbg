## CMakeLists.txt
#########################################################

SET (BusObserver_SOURCES
  busobserver.cpp
)

SET (BusObserver_HEADERS
  busobserver.h
)

KDE4_MOC_HEADERS (BusObserver_MOCS ${BusObserver_HEADERS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${MAIN_PROJECT_INCLUDES}
)

KDE4_ADD_LIBRARY (BusObserver STATIC
  ${BusObserver_MOCS}
  ${BusObserver_HEADERS}
  ${BusObserver_SOURCES}
)

TARGET_LINK_LIBRARIES (BusObserver
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  ${KDE4_KDECORE_LIBRARY}
  ${KDE4_KDEUI_LIBRARY}
)

SET_TARGET_PROPERTIES (BusObserver PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS} ${APP_PIE_CXXFLAGS}"
  LINK_FLAGS "${LIB_FPIC_CXXFLAGS} ${APP_PIE_LDFLAGS}"
)

## EOF
