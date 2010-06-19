# CMakeLists.txt

SET (dominspector_SOURCES
  dominspector.cpp
  domtree.cpp
  liststylesheet.cpp
  domtoolbar.cpp
)

SET (dominspector_HEADERS
  dominspector.h
  domtree.h
  liststylesheet.h
  domtoolbar.h
)

REMOVE_DEFINITIONS (-DQT_DBUS_LIB -DQT_XMLPATTERNS_LIB)

QT4_WRAP_CPP (dominspector_MOC ${dominspector_HEADERS})

QT4_ADD_RESOURCES (dominspector_RESRC dominspector.qrc)

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

ADD_LIBRARY (DomInspector STATIC
  ${dominspector_MOC}
  ${dominspector_RESRC}
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
 
