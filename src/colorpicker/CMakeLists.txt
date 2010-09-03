## Resolv Host Information Plugin

SET (target_subproject ColorPicker)

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

ADD_LIBRARY (${target_subproject} STATIC
  ${subproject_SOURCES}
  ${subproject_RESOURCE}
  ${subproject_MOCS}
  ${subproject_HEADERS}
  ${subproject_NOMOC_HEADERS}
)

TARGET_LINK_LIBRARIES (${target_subproject}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
)

SET_TARGET_PROPERTIES (${target_subproject} PROPERTIES
  OUTPUT_NAME ${target_subproject}
  LINKER_LANGUAGE CXX
  LINK_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

##EOF
