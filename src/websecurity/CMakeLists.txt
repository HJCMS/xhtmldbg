# CMakeLists.txt

SET (subproject_SOURCES
  websecurityitem.cpp
  websecuritytable.cpp
  webdatabasehandler.cpp
)

SET (subproject_HEADERS
  websecuritytable.h
)

SET (subproject_NOMOC_HEADERS
  websecurityitem.h
  webdatabasehandler.h
)

KDE4_MOC_HEADERS (subproject_MOC ${subproject_HEADERS})

ADD_DEFINITIONS (${QTIDY_DEFINITIONS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

KDE4_ADD_LIBRARY (WebSecurity STATIC
  ${subproject_MOC}
  ${subproject_HEADERS}
  ${subproject_NOMOC_HEADERS}
  ${subproject_SOURCES}
)

SET_TARGET_PROPERTIES (WebSecurity PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (WebSecurity
  ${XHTMLDBG_QT_LIBRARIES}
  ${XHTMLDBG_KDE_LIBRARIES}
)

## EOF
