# CMakeLists.txt

SET (XHTMLDBG_BINARY_PROJECT_INCULDE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/sourceviewer
  ${CMAKE_CURRENT_SOURCE_DIR}/bookmark
  ${CMAKE_CURRENT_SOURCE_DIR}/openurldialog
  ${CMAKE_CURRENT_BINARY_DIR}/configdialog
  ${CMAKE_CURRENT_SOURCE_DIR}/networker
  ${CMAKE_CURRENT_SOURCE_DIR}/configdialog
  ${XHTMLDBG_INCLUDES}
)

ADD_SUBDIRECTORY (sourceviewer)
ADD_SUBDIRECTORY (bookmark)
ADD_SUBDIRECTORY (openurldialog)
ADD_SUBDIRECTORY (networker)
ADD_SUBDIRECTORY (configdialog)

SET (xhtmldbg_SOURCES
  main.cpp
  xhtmldbg.cpp
  application.cpp
  window.cpp
  addresstoolbar.cpp
  keywordstoolbar.cpp
  messanger.cpp
  domviewer.cpp
  aboutdialog.cpp
  webviewer.cpp
  viewer.cpp
  page.cpp
  historymanager.cpp
  historyitem.cpp
  historymenu.cpp
  statusbar.cpp
  cookieview.cpp
  cookieacceptdialog.cpp
  headerview.cpp
)

SET (xhtmldbg_HEADERS
  xhtmldbg.h
  application.h
  window.h
  addresstoolbar.h
  keywordstoolbar.h
  messanger.h
  domviewer.h
  aboutdialog.h
  webviewer.h
  viewer.h
  page.h
  historymanager.h
  historymenu.h
  statusbar.h
  cookieview.h
  cookieacceptdialog.h
  headerview.h
)

SET (xhtmldbg_NOMOC
  historyitem.h
)

QT4_WRAP_CPP (xhtmldbg_MOC ${xhtmldbg_HEADERS})

INCLUDE_DIRECTORIES (${XHTMLDBG_BINARY_PROJECT_INCULDE})

QT4_ADD_RESOURCES (xhtmldbg_RESRC xhtmldbg.qrc)

ADD_EXECUTABLE (xhtmldbg
  ${qrc_rcresource}
  ${xhtmldbg_RESRC}
  ${xhtmldbg_MOC}
  ${xhtmldbg_NOMOC}
  ${xhtmldbg_UI}
  ${xhtmldbg_HEADERS}
  ${xhtmldbg_SOURCES}
)

TARGET_LINK_LIBRARIES (xhtmldbg
  ${XHTMLDBG_LIBRARIES}
  SourceViewer Bookmark OpenUrlDialog Networker ConfigDialog
)

INSTALL (TARGETS xhtmldbg DESTINATION bin)

INSTALL (FILES xhtmldbg.desktop DESTINATION share/applications)

## EOF
