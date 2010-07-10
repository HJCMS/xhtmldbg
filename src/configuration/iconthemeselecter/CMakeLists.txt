# CMakeLists.txt

SET (subproject_SOURCES
  iconthemeitem.cpp
  iconthememodel.cpp
  iconthemeselecter.cpp
  iconthemeslist.cpp
)

SET (subproject_HEADERS
  iconthemeitem.h
  iconthememodel.h
  iconthemeselecter.h
  iconthemeslist.h
)

QT4_WRAP_CPP (subproject_MOC ${subproject_HEADERS})

REMOVE_DEFINITIONS (-DQT_DBUS_LIB -DQT_WEBKIT_LIB -DQT_XML_LIB -DQT_XMLPATTERNS_LIB)

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

ADD_LIBRARY (IconThemeSelecter STATIC
  ${subproject_MOC}
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

SET_TARGET_PROPERTIES (IconThemeSelecter PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (IconThemeSelecter
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
)

## EOF
