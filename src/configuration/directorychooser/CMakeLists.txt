# CMakeLists.txt

SET (subproject_SOURCES
  directorychooser.cpp
  directorydialog.cpp
)

SET (subproject_HEADERS
  directorychooser.h
  directorydialog.h
)

REMOVE_DEFINITIONS (-DQT_DBUS_LIB -DQT_WEBKIT_LIB -DQT_XML_LIB -DQT_XMLPATTERNS_LIB)

QT4_WRAP_CPP (subproject_MOC ${subproject_HEADERS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

ADD_LIBRARY (DirectoryChooser STATIC
  ${subproject_MOC}
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

SET_TARGET_PROPERTIES (DirectoryChooser PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (DirectoryChooser
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  Settings
)

## EOF
 
