## CMakeLists.txt

SET (urltoolbar_SOURCES
  urlcompletermodel.cpp
  urllineedit.cpp
  urltoolbar.cpp
  urlhistory.cpp
)

SET (urltoolbar_HEADERS
  urlcompletermodel.h
  urllineedit.h
  urltoolbar.h
)

SET (urltoolbar_NOMOC_HEADERS
  urlhistory.h
)

KDE4_MOC_HEADERS (urltoolbar_MOC ${urltoolbar_HEADERS})

ADD_DEFINITIONS (${QTIDY_DEFINITIONS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

KDE4_ADD_LIBRARY (UrlToolBar STATIC
  ${urltoolbar_MOC}
  ${urltoolbar_HEADERS}
  ${urltoolbar_NOMOC_HEADERS}
  ${urltoolbar_SOURCES}
)

SET_TARGET_PROPERTIES (UrlToolBar PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (UrlToolBar
  Settings
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${XHTMLDBG_KDE_LIBRARIES}
)

## EOF
