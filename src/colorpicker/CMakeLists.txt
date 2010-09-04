## Resolv Host Information Plugin

SET(subproject_SOURCES
  colorpicker.cpp
  colors.cpp
  colortable.cpp
  colorpickerbutton.cpp
  grabberwindow.cpp
)

SET(subproject_HEADERS
  colorpicker.h
  colortable.h
  colorpickerbutton.h
  grabberwindow.h
)

SET(subproject_NOMOC_HEADERS
  colors.h
)

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_INCLUDE_INTERFACE}
)

QT4_WRAP_CPP (subproject_MOCS ${subproject_HEADERS})

REMOVE_DEFINITIONS (-DQT_WEBKIT_LIB -DQT_XML_LIB -DQT_XMLPATTERNS_LIB)

ADD_LIBRARY (ColorPicker STATIC
  ${subproject_SOURCES}
  ${subproject_RESOURCE}
  ${subproject_MOCS}
  ${subproject_HEADERS}
  ${subproject_NOMOC_HEADERS}
)

SET_TARGET_PROPERTIES (ColorPicker PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (ColorPicker
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
)

##EOF
