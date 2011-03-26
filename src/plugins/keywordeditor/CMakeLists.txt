## keywordeditor

SET (target_plugin KeywordEditor)

SET (LIBRARY_OUTPUT_PATH
  ${PLUGIN_OUTPUT_PATH}/
)

SET(plugin_SOURCES
  keywordeditor.cpp
  keywordeditordialog.cpp
  keywordsdom.cpp
  firstpagewidget.cpp
  keywordstableitem.cpp
  keywordstable.cpp
  keywordstablemodel.cpp
  keywordscombobox.cpp
  keywordstabledelegation.cpp
  keywordswidget.cpp
  keywordeditorplugin.cpp
  finaliseswidget.cpp
)

SET(plugin_HEADERS
  keywordeditor.h
  keywordeditordialog.h
  firstpagewidget.h
  keywordstable.h
  keywordstablemodel.h
  keywordscombobox.h
  keywordstabledelegation.h
  keywordswidget.h
  keywordeditorplugin.h
  finaliseswidget.h
)

SET(plugin_NOMOC_HEADERS
  keywordstableitem.h
  keywordsdom.h
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
  ${QT_QTXML_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
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
