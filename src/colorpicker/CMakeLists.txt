## Resolv Host Information Plugin

SET(colorpicker_SOURCES
  colorpicker.cpp
  colors.cpp
  colortable.cpp
  colorpickerbutton.cpp
  grabberwindow.cpp
)

SET(colorpicker_HEADERS
  colorpicker.h
  colortable.h
  colorpickerbutton.h
  grabberwindow.h
)

SET(colorpicker_NOMOC_HEADERS
  colors.h
)

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_INCLUDE_INTERFACE}
)

KDE4_MOC_HEADERS (colorpicker_MOCS ${colorpicker_HEADERS})

REMOVE_DEFINITIONS (-DQT_WEBKIT_LIB -DQT_XML_LIB -DQT_XMLPATTERNS_LIB)

KDE4_ADD_LIBRARY (ColorPicker STATIC
  ${colorpicker_MOCS}
  ${colorpicker_HEADERS}
  ${colorpicker_NOMOC_HEADERS}
  ${colorpicker_SOURCES}
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
