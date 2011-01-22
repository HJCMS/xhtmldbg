# CMakeLists.txt

SET (settargetdialog_SOURCES
  settargetdialog.cpp
  openfiledialog.cpp
  openurldialog.cpp
)

SET (settargetdialog_HEADERS
  settargetdialog.h
  openfiledialog.h
  openurldialog.h
)

REMOVE_DEFINITIONS (-DQT_DBUS_LIB -DQT_WEBKIT_LIB -DQT_XML_LIB -DQT_XMLPATTERNS_LIB)

QT4_WRAP_CPP (settargetdialog_MOC ${settargetdialog_HEADERS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

ADD_LIBRARY (Dialogs STATIC
  ${settargetdialog_MOC}
  ${settargetdialog_HEADERS}
  ${settargetdialog_SOURCES}
)

SET_TARGET_PROPERTIES (Dialogs PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (Dialogs
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
)

## EOF
 
