# CMakeLists.txt

ADD_SUBDIRECTORY (useragenteditor)

SET (configdialog_UI
  configdialogui.ui
)

SET (configdialog_SOURCES
  configdialog.cpp
  editcookiestable.cpp
  proxysettings.cpp
)

SET (configdialog_HEADERS
  configdialog.h
  editcookiestable.h
  proxysettings.h
)

QT4_WRAP_UI (configdialog_UIC ${configdialog_UI})

QT4_WRAP_CPP (configdialog_MOC ${configdialog_HEADERS})

REMOVE_DEFINITIONS (-DQT_DBUS_LIB -DQT_WEBKIT_LIB -DQT_XML_LIB -DQT_XMLPATTERNS_LIB)
ADD_DEFINITIONS (${QTIDY_DEFINITIONS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/useragenteditor
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

ADD_LIBRARY (ConfigDialog STATIC
  ${configdialog_MOC}
  ${configdialog_UIC}
  ${configdialog_HEADERS}
  ${configdialog_SOURCES}
)

SET_TARGET_PROPERTIES (ConfigDialog PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (ConfigDialog
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  UserAgentsEditor
)

## EOF
