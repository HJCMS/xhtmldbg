# CMakeLists.txt

SET (sourceview_SOURCES
  sourcewidget.cpp
  sourceview.cpp
  listlines.cpp
  highlighter.cpp
  contextmenu.cpp
)

SET (sourceview_HEADERS
  sourcewidget.h
  sourceview.h
  listlines.h
  highlighter.h
  contextmenu.h
)

REMOVE_DEFINITIONS (-DQT_DBUS_LIB -DQT_WEBKIT_LIB -DQT_XML_LIB -DQT_XMLPATTERNS_LIB)

QT4_WRAP_CPP (sourceview_MOC ${sourceview_HEADERS})

ADD_DEFINITIONS (${QTIDY_DEFINITIONS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

ADD_LIBRARY (SourceViewer STATIC
  ${sourceview_MOC}
  ${sourceview_HEADERS}
  ${sourceview_SOURCES}
)

SET_TARGET_PROPERTIES (SourceViewer PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (SourceViewer
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
)

## EOF
 
