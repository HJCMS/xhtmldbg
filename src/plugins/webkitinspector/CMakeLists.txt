## webkitinspector

SET (xhtmldbg_plugin WebKitInspector)

SET (LIBRARY_OUTPUT_PATH
  ${PLUGIN_OUTPUT_PATH}/
)

SET(xhtmldbgplugin_SOURCES
  webinspector.cpp
  webinspectorclient.cpp
  webkitinspectorplugin.cpp
)

SET(xhtmldbgplugin_HEADERS
  webinspector.h
  webinspectorclient.h
  webkitinspectorplugin.h
)

INCLUDE_DIRECTORIES (
  ${XHTMLDBG_INCLUDES}
  ${XHTMLDBG_INCLUDE_INTERFACE}
)

QT4_WRAP_CPP (xhtmldbgplugin_MOCS ${xhtmldbgplugin_HEADERS})

REMOVE_DEFINITIONS (-DQT_DBUS_LIB -DQT_XML_LIB)

ADD_LIBRARY (${xhtmldbg_plugin} MODULE
  ${xhtmldbgplugin_SOURCES}
  ${xhtmldbgplugin_RESOURCE}
  ${xhtmldbgplugin_MOCS}
)

TARGET_LINK_LIBRARIES (${xhtmldbg_plugin}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTWEBKIT_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
  xhtmldbginterface
)

SET_TARGET_PROPERTIES (${xhtmldbg_plugin} PROPERTIES
  OUTPUT_NAME ${xhtmldbg_plugin}
  LINKER_LANGUAGE CXX
  LINK_FLAGS "${LIB_FPIC_CXXFLAGS} ${LIB_RDYNAMIC_CXXFLAGS}"
)

INSTALL (TARGETS ${xhtmldbg_plugin}
  LIBRARY DESTINATION lib${LIB_SUFFIX}/xhtmldbg
  COMPONENT Plugin
)

##EOF
