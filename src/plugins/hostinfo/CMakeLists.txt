## Resolv Host Information Plugin

SET (target_plugin HostInfo)

SET (LIBRARY_OUTPUT_PATH
  ${PLUGIN_OUTPUT_PATH}/
)

SET(hostinfo_SOURCES
  hostinfo.cpp
  hostinfoplugin.cpp
)

SET(hostinfo_HEADERS
  hostinfo.h
  hostinfoplugin.h
)

INCLUDE_DIRECTORIES (
  ${XHTMLDBG_INCLUDES}
  ${XHTMLDBG_INCLUDE_INTERFACE}
)

QT4_WRAP_CPP (hostinfo_MOCS ${hostinfo_HEADERS})

REMOVE_DEFINITIONS (-DQT_DBUS_LIB -DQT_WEBKIT_LIB)

ADD_LIBRARY (${target_plugin} MODULE
  ${hostinfo_SOURCES}
  ${hostinfo_RESOURCE}
  ${hostinfo_MOCS}
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
