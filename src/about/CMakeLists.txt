# CMakeLists.txt

SET (subproject_UIS
  aboutdialog.ui
)

SET (subproject_SOURCES
  about.cpp
)

SET (subproject_HEADERS
  about.h
)

QT4_WRAP_UI (subproject_UIC ${subproject_UIS})

QT4_WRAP_CPP (subproject_MOC ${subproject_HEADERS})

QT4_ADD_RESOURCES (subproject_QRC about.qrc)

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_LIBRARY (About STATIC
  ${subproject_MOC}
  ${subproject_UIC}
  ${subproject_QRC}
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

REMOVE_DEFINITIONS (-DQT_DBUS_LIB -DQT_WEBKIT_LIB -DQT_XML_LIB -DQT_XMLPATTERNS_LIB)

SET_TARGET_PROPERTIES (About PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (About
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
)

## EOF
