## CMakeLists.txt
#########################################################

SET (subproject "Observer")

SET (subproject_SOURCES
  observer.cpp
)

SET (subproject_HEADERS
  observer.h
)

KDE4_MOC_HEADERS (subproject_MOCS ${subproject_HEADERS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${MAIN_PROJECT_INCLUDES}
)

KDE4_ADD_LIBRARY (subproject STATIC
  ${subproject_MOCS}
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

TARGET_LINK_LIBRARIES (subproject
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  ${KDE4_KDECORE_LIBRARY}
  ${KDE4_KDEUI_LIBRARY}
)

SET_TARGET_PROPERTIES (subproject PROPERTIES
  OUTPUT_NAME ${subproject}
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS} ${APP_PIE_CXXFLAGS}"
  LINK_FLAGS "${LIB_FPIC_CXXFLAGS} ${APP_PIE_LDFLAGS}"
)

## EOF
