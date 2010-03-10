# CMakeLists.txt

SET (configdialog_UI
  configdialogui.ui
)

SET (configdialog_SOURCES
  configdialog.cpp
  editcookiestable.cpp
  proxysettings.cpp
  sslconfig.cpp
)

SET (configdialog_HEADERS
  configdialog.h
  editcookiestable.h
  proxysettings.h
  sslconfig.h
)

QT4_WRAP_UI (configdialog_UIC ${configdialog_UI})

QT4_WRAP_CPP (configdialog_MOC ${configdialog_HEADERS})

ADD_DEFINITIONS (${QTIDY_DEFINITIONS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${QTIDY_BINARY_PROJECT_INCULDE}
)

ADD_LIBRARY (ConfigDialog STATIC
  ${configdialog_MOC}
  ${configdialog_UIC}
  ${configdialog_HEADERS}
  ${configdialog_SOURCES}
)

TARGET_LINK_LIBRARIES (ConfigDialog
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
)

## EOF
 
