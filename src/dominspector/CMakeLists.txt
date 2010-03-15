# CMakeLists.txt

SET (dominspector_SOURCES
  dominspector.cpp
  domtree.cpp
)

SET (dominspector_HEADERS
  dominspector.h
  domtree.h
)

SET (dominspector_STATIC_HEADERS
)

QT4_WRAP_CPP (dominspector_MOC ${dominspector_HEADERS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

ADD_LIBRARY (DomInspector STATIC
  ${dominspector_MOC}
  ${dominspector_HEADERS}
  ${dominspector_STATIC_HEADERS}
  ${dominspector_SOURCES}
)

TARGET_LINK_LIBRARIES (DomInspector
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTXML_LIBRARY}
  ${QT_QTWEBKIT_LIBRARY}
)

## EOF
 
