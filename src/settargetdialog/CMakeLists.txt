# CMakeLists.txt

SET (settargetdialog_SOURCES
  settargetdialog.cpp
)

SET (settargetdialog_HEADERS
  settargetdialog.h
)

QT4_WRAP_CPP (settargetdialog_MOC ${settargetdialog_HEADERS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

ADD_LIBRARY (SetTargetDialog STATIC
  ${settargetdialog_MOC}
  ${settargetdialog_HEADERS}
  ${settargetdialog_SOURCES}
)

TARGET_LINK_LIBRARIES (SetTargetDialog
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
)

## EOF
 
