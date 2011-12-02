# CMakeLists.txt
##############################

SET (OPTIONAL_LIBRARIES "")

SET (XHTMLDBG_BINARY_PROJECT_INCULDE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${XHTMLDBG_INCLUDE_INTERFACE}
  ${CMAKE_CURRENT_BINARY_DIR}/about
  ${CMAKE_CURRENT_SOURCE_DIR}/about
  ${CMAKE_CURRENT_SOURCE_DIR}/autosaver
  ${CMAKE_CURRENT_SOURCE_DIR}/busobserver
  ${CMAKE_CURRENT_SOURCE_DIR}/bookmark
  ${CMAKE_CURRENT_SOURCE_DIR}/colorpicker
  ${CMAKE_CURRENT_SOURCE_DIR}/dbmanager
  ${CMAKE_CURRENT_SOURCE_DIR}/cookiesdatabase
  ${CMAKE_CURRENT_SOURCE_DIR}/docking
  ${CMAKE_CURRENT_SOURCE_DIR}/hostinfo
  ${CMAKE_CURRENT_SOURCE_DIR}/sourceviewer
  ${CMAKE_CURRENT_SOURCE_DIR}/dialogs
  ${CMAKE_CURRENT_SOURCE_DIR}/websecurity
  ${CMAKE_CURRENT_SOURCE_DIR}/settings
  ${CMAKE_CURRENT_SOURCE_DIR}/cssvalidator
  ${CMAKE_CURRENT_SOURCE_DIR}/configuration
  ${CMAKE_CURRENT_SOURCE_DIR}/stylesheeted
  ${CMAKE_CURRENT_SOURCE_DIR}/urltoolbar
  ${CMAKE_CURRENT_SOURCE_DIR}/dominspector
  ${CMAKE_CURRENT_SOURCE_DIR}/networker
  ${CMAKE_CURRENT_SOURCE_DIR}/downloadmanager
  ${CMAKE_CURRENT_SOURCE_DIR}/rdfparser
  ${CMAKE_CURRENT_SOURCE_DIR}/formmanager
  ${XHTMLDBG_INCLUDES}
)

ADD_SUBDIRECTORY (html)
ADD_SUBDIRECTORY (about)
ADD_SUBDIRECTORY (autosaver)
ADD_SUBDIRECTORY (busobserver)
ADD_SUBDIRECTORY (bookmark)
ADD_SUBDIRECTORY (colorpicker)
ADD_SUBDIRECTORY (dbmanager)
ADD_SUBDIRECTORY (cookiesdatabase)
ADD_SUBDIRECTORY (docking)
ADD_SUBDIRECTORY (hostinfo)
ADD_SUBDIRECTORY (interface)
ADD_SUBDIRECTORY (dialogs)
ADD_SUBDIRECTORY (websecurity)
ADD_SUBDIRECTORY (settings)
ADD_SUBDIRECTORY (stylesheeted)
## to predicate on certain conditions
ADD_SUBDIRECTORY (sourceviewer)
ADD_SUBDIRECTORY (cssvalidator)
ADD_SUBDIRECTORY (configuration)
ADD_SUBDIRECTORY (dominspector)
ADD_SUBDIRECTORY (networker)
ADD_SUBDIRECTORY (downloadmanager)
ADD_SUBDIRECTORY (rdfparser)
ADD_SUBDIRECTORY (formmanager)
ADD_SUBDIRECTORY (urltoolbar)
##############################
## Interfaces
##############################
IF (XULRUNNER_FOUND)
  SET (XHTMLDBG_BINARY_PROJECT_INCULDE
    ${XHTMLDBG_BINARY_PROJECT_INCULDE}
    ${CMAKE_CURRENT_SOURCE_DIR}/npploader
  )
  SET (OPTIONAL_LIBRARIES
    ${OPTIONAL_LIBRARIES}
    NPPLoader
  )
  ADD_SUBDIRECTORY (npploader)
  ADD_DEFINITIONS (-DHAVE_NPPLOADER)
ENDIF (XULRUNNER_FOUND)

##############################
## Extensions and Plugins
##############################
ADD_SUBDIRECTORY (plugins)
ADD_SUBDIRECTORY (components)

##############################
## SOURCES
##############################
SET (xhtmldbg_SOURCES
  main.cpp
  xhtmldbgmain.cpp
  application.cpp
  window.cpp
  keywordstoolbar.cpp
  tidymessanger.cpp
  jsmessanger.cpp
  webviewer.cpp
  viewer.cpp
  page.cpp
  pagehistory.cpp
  historymanager.cpp
  historyitem.cpp
  historymenu.cpp
  statusbar.cpp
  cookieacceptdialog.cpp
  appevents.cpp
  autoreloader.cpp
  autoreloadmenu.cpp
  useragentmenu.cpp
  zoombar.cpp
  geolocation.cpp
  javascriptpopup.cpp
  webinspector.cpp
)

##############################
## HEADERS
##############################
SET (xhtmldbg_HEADERS
  xhtmldbgmain.h
  application.h
  window.h
  keywordstoolbar.h
  tidymessanger.h
  jsmessanger.h
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
  zoombar.h
  geolocation.h
  javascriptpopup.h
)

##############################
## NOMOC HEADERS
##############################
SET (xhtmldbg_NOMOC
  historyitem.h
  pagehistory.h
)

##############################
## x-qt-plugin SUPPORT
##############################
IF (QT_QTUITOOLS_FOUND)
  SET (OPTIONAL_LIBRARIES ${OPTIONAL_LIBRARIES}
    ${QT_QTSCRIPT_LIBRARY}
    ${QT_QTUITOOLS_LIBRARY}
  )
  SET (xhtmldbg_SOURCES ${xhtmldbg_SOURCES}
    uitoolsloader.cpp
  )
  SET (xhtmldbg_HEADERS ${xhtmldbg_HEADERS}
    uitoolsloader.h
  )
ENDIF (QT_QTUITOOLS_FOUND)

##############################
## Experimental Code
##############################
# IF (XHTMLDBG_EXPERIMENTAL)
# ENDIF (XHTMLDBG_EXPERIMENTAL)

##############################
## generated MOC Files
##############################
KDE4_MOC_HEADERS (xhtmldbg_MOC ${xhtmldbg_HEADERS})

##############################
## INCLUDES
##############################
INCLUDE_DIRECTORIES (${XHTMLDBG_BINARY_PROJECT_INCULDE})

##############################
## RESOURCE
##############################
QT4_ADD_RESOURCES (xhtmldbg_RESRC xhtmldbg.qrc)

##############################
## EXECUTABLE
##############################
KDE4_ADD_EXECUTABLE (xhtmldbg
  ${xhtmldbg_RESRC}
  ${xhtmldbg_MOC}
  ${xhtmldbg_NOMOC}
  ${xhtmldbg_UI}
  ${xhtmldbg_ADAPTOR}
  ${xhtmldbg_HEADERS}
  ${xhtmldbg_SOURCES}
)

##############################
## LINKER
##############################
TARGET_LINK_LIBRARIES (xhtmldbg
  xhtmldbginterface
  About
  BusObserver
  Bookmark
  ColorPicker
  Configuration
  CookiesDatabase
  CSSValidator
  DBManager
  Dialogs
  Docking
  DomInspector
  DownloadManager
  FormManager
  HostInfo
  Networker
  RDFParser
  Settings
  SourceViewer
  StyleSheeted
  UrlToolBar
  WebSecurity
  ${OPTIONAL_LIBRARIES}
  ${XHTMLDBG_LIBRARIES}
)

##############################
## LINKER PROPERTIES
##############################
SET_TARGET_PROPERTIES (xhtmldbg PROPERTIES
  OUTPUT_NAME xhtmldbg
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS} ${APP_PIE_CXXFLAGS}"
  LINK_FLAGS "${XHTMLDBG_APP_LDFLAGS} ${LIB_FPIC_CXXFLAGS} ${APP_PIE_LDFLAGS}"
)

##############################
## INSTALL
##############################
INSTALL (TARGETS xhtmldbg DESTINATION ${BIN_INSTALL_DIR})

INSTALL (FILES xhtmldbg.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})

## EOF
