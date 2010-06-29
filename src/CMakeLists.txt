# CMakeLists.txt

SET (XHTMLDBG_BINARY_PROJECT_INCULDE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${XHTMLDBG_INCLUDE_INTERFACE}
  ${CMAKE_CURRENT_SOURCE_DIR}/autosaver
  ${CMAKE_CURRENT_SOURCE_DIR}/sourceviewer
  ${CMAKE_CURRENT_SOURCE_DIR}/bookmark
  ${CMAKE_CURRENT_SOURCE_DIR}/settargetdialog
  ${CMAKE_CURRENT_SOURCE_DIR}/openurldialog
  ${CMAKE_CURRENT_SOURCE_DIR}/docking
  ${CMAKE_CURRENT_SOURCE_DIR}/editcookiestable
  ${CMAKE_CURRENT_BINARY_DIR}/configdialog
  ${CMAKE_CURRENT_SOURCE_DIR}/networker
  ${CMAKE_CURRENT_SOURCE_DIR}/dominspector
  ${CMAKE_CURRENT_SOURCE_DIR}/cssvalidator
  ${CMAKE_CURRENT_SOURCE_DIR}/downloadmanager
  ${CMAKE_CURRENT_SOURCE_DIR}/rdfparser
  ${CMAKE_CURRENT_SOURCE_DIR}/configuration
  ${XHTMLDBG_INCLUDES}
)

ADD_SUBDIRECTORY (autosaver)
ADD_SUBDIRECTORY (interface)
ADD_SUBDIRECTORY (sourceviewer)
ADD_SUBDIRECTORY (bookmark)
ADD_SUBDIRECTORY (settargetdialog)
ADD_SUBDIRECTORY (openurldialog)
ADD_SUBDIRECTORY (docking)
ADD_SUBDIRECTORY (editcookiestable)
ADD_SUBDIRECTORY (networker)
ADD_SUBDIRECTORY (dominspector)
ADD_SUBDIRECTORY (cssvalidator)
ADD_SUBDIRECTORY (downloadmanager)
ADD_SUBDIRECTORY (rdfparser)
ADD_SUBDIRECTORY (configuration)
ADD_SUBDIRECTORY (plugins)

SET (xhtmldbg_SOURCES
  main.cpp
  xhtmldbgmain.cpp
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
  zoombar.cpp
  geolocation.cpp
)

SET (xhtmldbg_HEADERS
  xhtmldbgmain.h
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
  zoombar.h
  geolocation.h
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

IF (__CHECK_GCC_FPIE)
  SET ( "${CMAKE_CXX_FLAGS} -fPIE")
ENDIF (__CHECK_GCC_FPIE)

TARGET_LINK_LIBRARIES (xhtmldbg
  ${XHTMLDBG_LIBRARIES}
  xhtmldbginterface
  SourceViewer
  Bookmark
  Docking
  DomInspector
  SetTargetDialog
  OpenUrlDialog
  Networker
  EditCookiesTable
  Configuration
  CSSValidator
  DownloadManager
  RDFParser
)

SET_TARGET_PROPERTIES (xhtmldbg PROPERTIES
  OUTPUT_NAME xhtmldbg
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
  LINK_FLAGS "${LIB_FPIC_CXXFLAGS} ${APP_PIE_LDFLAGS}"
)

INSTALL (TARGETS xhtmldbg DESTINATION bin)

INSTALL (FILES xhtmldbg.desktop DESTINATION share/applications)

## EOF
