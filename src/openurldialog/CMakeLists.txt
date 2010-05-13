# CMakeLists.txt

SET (openurldialog_SOURCES
  openurldialog.cpp
)

SET (openurldialog_HEADERS
  openurldialog.h
)

QT4_WRAP_CPP (openurldialog_MOC ${openurldialog_HEADERS})

ADD_DEFINITIONS (${QTIDY_DEFINITIONS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

ADD_LIBRARY (OpenUrlDialog STATIC
  ${openurldialog_MOC}
  ${openurldialog_HEADERS}
  ${openurldialog_SOURCES}
)

TARGET_LINK_LIBRARIES (OpenUrlDialog
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
)

## EOF
 
