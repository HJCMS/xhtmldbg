## Template Plugin

SET (target_plugin SelfHtmlSidebar)

SET (LIBRARY_OUTPUT_PATH
  ${PLUGIN_OUTPUT_PATH}/
)

SET(selfhtmlsidebar_SOURCES
  selfhtmlsidebar.cpp
  selfhtmlsidebarplugin.cpp
)

SET(selfhtmlsidebar_HEADERS
  selfhtmlsidebar.h
  selfhtmlsidebarplugin.h
)

INCLUDE_DIRECTORIES (
  ${XHTMLDBG_INCLUDEDIR}
  ${XHTMLDBG_INCLUDE_INTERFACE}
)

REMOVE_DEFINITIONS (-DQT_XMLPATTERNS_LIB)

KDE4_MOC_HEADERS (selfhtmlsidebar_MOCS ${selfhtmlsidebar_HEADERS})

KDE4_ADD_LIBRARY (${target_plugin} MODULE
  ${selfhtmlsidebar_SOURCES}
  ${selfhtmlsidebar_RESOURCE}
  ${selfhtmlsidebar_MOCS}
)

TARGET_LINK_LIBRARIES (${target_plugin}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTWEBKIT_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
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
