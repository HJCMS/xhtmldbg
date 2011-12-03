# CMakeLists.txt

SET (subproject "DBManager")

SET (subproject_SOURCES
  dbmanager.cpp
  dblocator.cpp
)

SET (subproject_HEADERS
  dbmanager.h
  dblocator.h
)

SET (subproject_NOMOC_HEADERS
)

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_INCLUDES}
  ${XHTMLDBG_INCLUDE_INTERFACE}
)

KDE4_MOC_HEADERS (subproject_MOC ${subproject_HEADERS})

QT4_ADD_RESOURCES (subproject_QRC dbmanager.qrc)

KDE4_ADD_LIBRARY (${subproject} STATIC
  ${subproject_QRC}
  ${subproject_MOC}
  ${subproject_NOMOC_HEADERS}
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

SET_TARGET_PROPERTIES (${subproject} PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (${subproject}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTSQL_LIBRARY}
)

## EOF
