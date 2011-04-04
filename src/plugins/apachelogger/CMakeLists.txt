## apachelogger

SET (target_plugin ApacheLogger)

SET (LIBRARY_OUTPUT_PATH
  ${PLUGIN_OUTPUT_PATH}/
)

SET(plugin_SOURCES
  apachelogger.cpp
  apacheloggerplugin.cpp
  listmessages.cpp
  loglistener.cpp
)

SET(plugin_HEADERS
  apachelogger.h
  apacheloggerplugin.h
  listmessages.h
  loglistener.h
)

SET(plugin_NOMOC_HEADERS
)

INCLUDE_DIRECTORIES (
  ${XHTMLDBG_INCLUDES}
  ${XHTMLDBG_INCLUDE_INTERFACE}
)

KDE4_MOC_HEADERS (plugin_MOCS ${plugin_HEADERS})

KDE4_ADD_LIBRARY (${target_plugin} MODULE
  ${plugin_SOURCES}
  ${plugin_HEADERS}
  ${plugin_NOMOC_HEADERS}
  ${plugin_MOCS}
)

TARGET_LINK_LIBRARIES (${target_plugin}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${XHTMLDBG_KDE_LIBRARIES}
  xhtmldbginterface
)

SET_TARGET_PROPERTIES (${target_plugin} PROPERTIES
  OUTPUT_NAME ${target_plugin}
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
  LINK_FLAGS "${LIB_FPIC_CXXFLAGS} ${LIB_RDYNAMIC_CXXFLAGS}"
)

INSTALL (TARGETS ${target_plugin}
  LIBRARY DESTINATION lib${LIB_SUFFIX}/xhtmldbg
  COMPONENT Plugin
)

##EOF
