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
  ${CMAKE_CURRENT_SOURCE_DIR}/bookmark
  ${CMAKE_CURRENT_SOURCE_DIR}/colorpicker
  ${CMAKE_CURRENT_SOURCE_DIR}/dbmanager
  ${CMAKE_CURRENT_SOURCE_DIR}/cookiesdatabase
  ${CMAKE_CURRENT_SOURCE_DIR}/docking
  ${CMAKE_CURRENT_SOURCE_DIR}/hostinfo
  ${CMAKE_CURRENT_SOURCE_DIR}/sourcecache
  ${CMAKE_CURRENT_SOURCE_DIR}/sourceviewer
  ${CMAKE_CURRENT_SOURCE_DIR}/dialogs
  ${CMAKE_CURRENT_SOURCE_DIR}/settings
  ${CMAKE_CURRENT_SOURCE_DIR}/cssvalidator
  ${CMAKE_CURRENT_SOURCE_DIR}/configuration
  ${CMAKE_CURRENT_SOURCE_DIR}/dominspector
  ${CMAKE_CURRENT_SOURCE_DIR}/networker
  ${CMAKE_CURRENT_SOURCE_DIR}/downloadmanager
  ${CMAKE_CURRENT_SOURCE_DIR}/rdfparser
  ${XHTMLDBG_INCLUDES}
)

ADD_SUBDIRECTORY (html)
ADD_SUBDIRECTORY (about)
ADD_SUBDIRECTORY (autosaver)
ADD_SUBDIRECTORY (bookmark)
ADD_SUBDIRECTORY (colorpicker)
ADD_SUBDIRECTORY (dbmanager)
ADD_SUBDIRECTORY (cookiesdatabase)
ADD_SUBDIRECTORY (docking)
ADD_SUBDIRECTORY (hostinfo)
ADD_SUBDIRECTORY (interface)
ADD_SUBDIRECTORY (sourcecache)
ADD_SUBDIRECTORY (sourceviewer)
ADD_SUBDIRECTORY (dialogs)
ADD_SUBDIRECTORY (settings)
## to predicate on certain conditions
ADD_SUBDIRECTORY (cssvalidator)
ADD_SUBDIRECTORY (configuration)
ADD_SUBDIRECTORY (dominspector)
ADD_SUBDIRECTORY (networker)
ADD_SUBDIRECTORY (downloadmanager)
ADD_SUBDIRECTORY (rdfparser)
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

IF (XHTMLDBG_EXPERIMENTAL)
  ADD_DEFINITIONS ("-D_XHTMLDBG_EXPERIMENTAL")
ENDIF (XHTMLDBG_EXPERIMENTAL)

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
  addresstoolbar.cpp
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
  xhtmldbgadaptor.cpp
  zoombar.cpp
  geolocation.cpp
  javascriptpopup.cpp
  webinspector.cpp
  resizeportbuttons.cpp
)

##############################
## HEADERS
##############################
SET (xhtmldbg_HEADERS
  xhtmldbgmain.h
  application.h
  window.h
  addresstoolbar.h
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
  xhtmldbgadaptor.h
  zoombar.h
  geolocation.h
  javascriptpopup.h
  webinspector.h
  resizeportbuttons.h
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
## generated MOC Files
##############################
IF (KDE_FOUND)
  KDE4_MOC_HEADERS (xhtmldbg_MOC ${xhtmldbg_HEADERS})
ELSE (KDE_FOUND)
  QT4_WRAP_CPP (xhtmldbg_MOC ${xhtmldbg_HEADERS})
ENDIF (KDE_FOUND)

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
IF (KDE_FOUND)
  KDE4_ADD_EXECUTABLE (xhtmldbg
    ${xhtmldbg_RESRC}
    ${xhtmldbg_MOC}
    ${xhtmldbg_NOMOC}
    ${xhtmldbg_UI}
    ${xhtmldbg_ADAPTOR}
    ${xhtmldbg_HEADERS}
    ${xhtmldbg_SOURCES}
  )
ELSE (KDE_FOUND)
  ADD_EXECUTABLE (xhtmldbg
    ${xhtmldbg_RESRC}
    ${xhtmldbg_MOC}
    ${xhtmldbg_NOMOC}
    ${xhtmldbg_UI}
    ${xhtmldbg_ADAPTOR}
    ${xhtmldbg_HEADERS}
    ${xhtmldbg_SOURCES}
  )
ENDIF (KDE_FOUND)

##############################
## LINKER
##############################
TARGET_LINK_LIBRARIES (xhtmldbg
  About
  Bookmark
  ColorPicker
  CookiesDatabase
  Docking
  HostInfo
  xhtmldbginterface
  Dialogs
  SourceCache
  SourceViewer
  Settings
  CSSValidator
  Configuration
  DomInspector
  DBManager
  Networker
  DownloadManager
  RDFParser
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
  LINK_FLAGS "${LIB_FPIC_CXXFLAGS} ${APP_PIE_LDFLAGS} ${TWEAKING_LDFLAGS}"
)

##############################
## INSTALL
##############################
INSTALL (TARGETS xhtmldbg DESTINATION bin)

INSTALL (FILES xhtmldbg.desktop DESTINATION share/applications)

## EOF
