# CMakeLists.txt

SET (sourceviewer_SOURCES
  sourcewidget.cpp
  contextmenu.cpp
)

SET (sourceviewer_HEADERS
  sourcewidget.h
  contextmenu.h
)

KDE4_MOC_HEADERS (sourceviewer_MOC ${sourceviewer_HEADERS})

ADD_DEFINITIONS (${QTIDY_DEFINITIONS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

KDE4_ADD_LIBRARY (SourceViewer STATIC
  ${sourceviewer_MOC}
  ${sourceviewer_HEADERS}
  ${sourceviewer_SOURCES}
)

SET_TARGET_PROPERTIES (SourceViewer PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (SourceViewer
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${XHTMLDBG_KDE_LIBRARIES}
)

## EOF
 
