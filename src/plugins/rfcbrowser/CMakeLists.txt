## Resolv Host Information Plugin

SET (target_plugin RFCBrowser)

SET (LIBRARY_OUTPUT_PATH
  ${PLUGIN_OUTPUT_PATH}/
)

SET(plugin_SOURCES
  rfcbrowser.cpp
  selectermodel.cpp
  rfcbrowserplugin.cpp
)

SET(plugin_HEADERS
  rfcbrowser.h
  selectermodel.h
  rfcbrowserplugin.h
)

INCLUDE_DIRECTORIES (
  ${XHTMLDBG_INCLUDES}
  ${XHTMLDBG_INCLUDE_INTERFACE}
)

QT4_WRAP_CPP (plugin_MOCS ${plugin_HEADERS})

QT4_ADD_RESOURCES (plugin_RESRC rfcbrowser.qrc)

REMOVE_DEFINITIONS (-DQT_DBUS_LIB -DQT_WEBKIT_LIB -DQT_NETWORK_LIB -DQT_XMLPATTERNS_LIB)

ADD_LIBRARY (${target_plugin} MODULE
  ${plugin_SOURCES}
  ${plugin_RESOURCE}
  ${plugin_MOCS}
  ${plugin_RESRC}
)

TARGET_LINK_LIBRARIES (${target_plugin}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTXML_LIBRARY}
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
