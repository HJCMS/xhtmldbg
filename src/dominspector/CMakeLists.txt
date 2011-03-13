# CMakeLists.txt

SET (dominspector_SOURCES
  dominspector.cpp
  domtree.cpp
  liststylesheet.cpp
  domtoolbar.cpp
  contentdialog.cpp
)

SET (dominspector_HEADERS
  dominspector.h
  domtree.h
  liststylesheet.h
  domtoolbar.h
  contentdialog.h
)

KDE4_MOC_HEADERS (dominspector_MOC ${dominspector_HEADERS})

QT4_ADD_RESOURCES (dominspector_RESRC dominspector.qrc)

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

KDE4_ADD_LIBRARY (DomInspector STATIC
  ${dominspector_MOC}
  ${dominspector_RESRC}
  ${dominspector_HEADERS}
  ${dominspector_STATIC_HEADERS}
  ${dominspector_SOURCES}
)

SET_TARGET_PROPERTIES (DomInspector PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (DomInspector
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTXML_LIBRARY}
  ${QT_QTWEBKIT_LIBRARY}
  ${XHTMLDBG_KDE_LIBRARIES}
  Settings
)

## EOF
 
