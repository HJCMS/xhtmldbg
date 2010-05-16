# CMakeLists.txt

SET (ledlabel_SOURCES
  ledlabel.cpp
)

SET (ledlabel_HEADERS
  ledlabel.h
)

QT4_WRAP_CPP (ledlabel_MOC ${ledlabel_HEADERS})

QT4_ADD_RESOURCES (ledlabel_RESRC ledlabel.qrc)

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
)

ADD_LIBRARY (ledlabel STATIC
  ${ledlabel_MOC}
  ${ledlabel_RESRC}
  ${ledlabel_HEADERS}
  ${ledlabel_SOURCES}
)

TARGET_LINK_LIBRARIES (ledlabel
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
)

## EOF
