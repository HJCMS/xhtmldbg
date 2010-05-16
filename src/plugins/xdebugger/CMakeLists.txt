# CMakeLists.txt

SET (target_plugin XDebugger)

ADD_SUBDIRECTORY (ledlabel)

SET (LIBRARY_OUTPUT_PATH
  ${PLUGIN_OUTPUT_PATH}/
)

SET (xdebugger_SOURCES
  xdebuggerplugin.cpp
  xdebugger.cpp
  xdebugmenubar.cpp
  xdebugprojectdialog.cpp
  xdebugprojecttable.cpp
  xdebugprojecttablemodel.cpp
  xdebugprojecttabledelegation.cpp
  xdebugprojectitem.cpp
  qbytearrayfifo.cpp
  xdebugspinbox.cpp
  xdebuglineedit.cpp
  xdebugsocket.cpp
  xdebugclient.cpp
  xdebugserver.cpp
  xdebugreader.cpp
)

SET (xdebugger_HEADERS
  xdebuggerplugin.h
  xdebugger.h
  xdebugmenubar.h
  xdebugprojectdialog.h
  xdebugprojecttable.h
  xdebugprojecttablemodel.h
  xdebugprojecttabledelegation.h
  xdebugspinbox.h
  xdebuglineedit.h
  xdebugsocket.h
  xdebugclient.h
  xdebugserver.h
)

SET (xdebugger_NOMOC
  xdebugprojectitem.h
  qbytearrayfifo.h
  xdebugreader.h
)

REMOVE_DEFINITIONS (-DQT_DBUS_LIB -DQT_WEBKIT_LIB)

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/ledlabel
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
  ${XHTMLDBG_INCLUDES}
  ${XHTMLDBG_INCLUDE_INTERFACE}
)

QT4_WRAP_CPP (xdebugger_MOC ${xdebugger_HEADERS})

ADD_LIBRARY (${target_plugin} MODULE
  ${xdebugger_MOC}
  ${xdebugger_NOMOC}
  ${xdebugger_HEADERS}
  ${xdebugger_SOURCES}
)

TARGET_LINK_LIBRARIES (${target_plugin}
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTXML_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
  ledlabel
  xhtmldbginterface
)

SET_TARGET_PROPERTIES (${target_plugin} PROPERTIES
  OUTPUT_NAME ${target_plugin}
  LINKER_LANGUAGE CXX
)

INSTALL (TARGETS ${target_plugin}
  LIBRARY DESTINATION lib${LIB_SUFFIX}/xhtmldbg
  COMPONENT Plugin
)

## EOF
