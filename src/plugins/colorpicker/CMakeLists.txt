## Resolv Host Information Plugin

SET (target_plugin ColorPicker)

SET (LIBRARY_OUTPUT_PATH
  ${PLUGIN_OUTPUT_PATH}/
)

SET(pluginproject_SOURCES
  colorpicker.cpp
  colorpickerplugin.cpp
  colors.cpp
  colortable.cpp
  watcher.cpp
  grabberwindow.cpp
)

SET(pluginproject_HEADERS
  colorpicker.h
  colorpickerplugin.h
  colortable.h
  watcher.h
  grabberwindow.h
)

SET(pluginproject_NOMOC_HEADERS
  colors.h
)

INCLUDE_DIRECTORIES (
  ${XHTMLDBG_INCLUDES}
  ${XHTMLDBG_INCLUDE_INTERFACE}
)

QT4_WRAP_CPP (pluginproject_MOCS ${pluginproject_HEADERS})

REMOVE_DEFINITIONS (-DQT_WEBKIT_LIB -DQT_XMLPATTERNS_LIB)

ADD_LIBRARY (${target_plugin} MODULE
  ${pluginproject_SOURCES}
  ${pluginproject_RESOURCE}
  ${pluginproject_MOCS}
  ${pluginproject_HEADERS}
  ${pluginproject_NOMOC_HEADERS}
)

TARGET_LINK_LIBRARIES (${target_plugin}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
  xhtmldbginterface
)

SET_TARGET_PROPERTIES (${target_plugin} PROPERTIES
  OUTPUT_NAME ${target_plugin}
  LINKER_LANGUAGE CXX
  LINK_FLAGS "${LIB_FPIC_CXXFLAGS} ${LIB_RDYNAMIC_CXXFLAGS}"
)

INSTALL (TARGETS ${target_plugin}
  LIBRARY DESTINATION lib${LIB_SUFFIX}/xhtmldbg
  COMPONENT Plugin
)

##EOF
