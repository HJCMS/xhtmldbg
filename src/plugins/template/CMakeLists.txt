## Template Plugin

PKG_CHECK_MODULES (XHTMLDBG xhtmldbg>=0.8.5)

SET (target_plugin HelloWorld)

SET (LIBRARY_OUTPUT_PATH
  ${PLUGIN_OUTPUT_PATH}/
)

SET(helloworld_SOURCES
  helloworld.cpp
  helloworldplugin.cpp
)

SET(helloworld_HEADERS
  helloworld.h
  helloworldplugin.h
)

INCLUDE_DIRECTORIES (
  ${XHTMLDBG_INCLUDEDIR}
)

KDE4_MOC_HEADERS (helloworld_MOCS ${helloworld_HEADERS})



KDE4_ADD_LIBRARY (${target_plugin} MODULE
  ${helloworld_SOURCES}
  ${helloworld_RESOURCE}
  ${helloworld_MOCS}
)

TARGET_LINK_LIBRARIES (${target_plugin}
  ${XHTMLDBG_LIBRARIES}
)

SET_TARGET_PROPERTIES (${target_plugin} PROPERTIES
  OUTPUT_NAME ${target_plugin}
  LINKER_LANGUAGE CXX
)

INSTALL (TARGETS ${target_plugin}
  LIBRARY DESTINATION lib${LIB_SUFFIX}/xhtmldbg
  COMPONENT Plugin
)

##EOF
