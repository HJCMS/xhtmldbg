# CMakeLists.txt

SET (XHTMLDBG_BINARY_PROJECT_INCULDE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/sourceviewer
  ${CMAKE_CURRENT_SOURCE_DIR}/bookmark
  ${CMAKE_CURRENT_SOURCE_DIR}/openurldialog
  ${CMAKE_CURRENT_SOURCE_DIR}/docking
  ${CMAKE_CURRENT_BINARY_DIR}/configdialog
  ${CMAKE_CURRENT_SOURCE_DIR}/networker
  ${CMAKE_CURRENT_SOURCE_DIR}/dominspector
  ${CMAKE_CURRENT_SOURCE_DIR}/configdialog
  ${CMAKE_CURRENT_SOURCE_DIR}/cssvalidator
  ${XHTMLDBG_INCLUDES}
)

ADD_SUBDIRECTORY (sourceviewer)
ADD_SUBDIRECTORY (bookmark)
ADD_SUBDIRECTORY (openurldialog)
ADD_SUBDIRECTORY (docking)
ADD_SUBDIRECTORY (networker)
ADD_SUBDIRECTORY (dominspector)
ADD_SUBDIRECTORY (configdialog)
ADD_SUBDIRECTORY (cssvalidator)

SET (xhtmldbg_SOURCES
  main.cpp
  xhtmldbg.cpp
  application.cpp
  window.cpp
  addresstoolbar.cpp
  keywordstoolbar.cpp
  tidymessanger.cpp
  jsmessanger.cpp
  aboutdialog.cpp
  webviewer.cpp
  viewer.cpp
  page.cpp
  historymanager.cpp
  historyitem.cpp
  historymenu.cpp
  statusbar.cpp
  cookieacceptdialog.cpp
  appevents.cpp
  autoreloader.cpp
  autoreloadmenu.cpp
  useragentmenu.cpp
  xhtmldbgadaptor.cpp
)

SET (xhtmldbg_HEADERS
  xhtmldbg.h
  application.h
  window.h
  addresstoolbar.h
  keywordstoolbar.h
  tidymessanger.h
  jsmessanger.h
  aboutdialog.h
  webviewer.h
  viewer.h
  page.h
  historymanager.h
  historymenu.h
  statusbar.h
  cookieacceptdialog.h
  appevents.h
  autoreloader.h
  autoreloadmenu.h
  useragentmenu.h
  xhtmldbgadaptor.h
)

SET (xhtmldbg_NOMOC
  historyitem.h
)

QT4_WRAP_CPP (xhtmldbg_MOC ${xhtmldbg_HEADERS})

INCLUDE_DIRECTORIES (${XHTMLDBG_BINARY_PROJECT_INCULDE})

QT4_ADD_RESOURCES (xhtmldbg_RESRC xhtmldbg.qrc)

ADD_EXECUTABLE (xhtmldbg
  ${xhtmldbg_RESRC}
  ${xhtmldbg_MOC}
  ${xhtmldbg_NOMOC}
  ${xhtmldbg_UI}
  ${xhtmldbg_ADAPTOR}
  ${xhtmldbg_HEADERS}
  ${xhtmldbg_SOURCES}
)

TARGET_LINK_LIBRARIES (xhtmldbg
  ${XHTMLDBG_LIBRARIES}
  SourceViewer
  Bookmark
  Docking
  DomInspector
  OpenUrlDialog
  Networker
  ConfigDialog
  CSSValidator
)

INSTALL (TARGETS xhtmldbg DESTINATION bin)

INSTALL (FILES xhtmldbg.desktop DESTINATION share/applications)

## EOF
