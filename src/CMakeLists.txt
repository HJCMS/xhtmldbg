# CMakeLists.txt

SET (XHTMLDBG_BINARY_PROJECT_INCULDE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
#   ${CMAKE_CURRENT_SOURCE_DIR}/bookmark
#   ${CMAKE_CURRENT_SOURCE_DIR}/icontheme
#   ${CMAKE_CURRENT_SOURCE_DIR}/browser
#   ${CMAKE_CURRENT_SOURCE_DIR}/configdialog
  ${CMAKE_CURRENT_SOURCE_DIR}/sourceviewer
  ${CMAKE_CURRENT_SOURCE_DIR}/webviewer
#   ${CMAKE_CURRENT_SOURCE_DIR}/cssviewer
  ${XHTMLDBG_INCLUDES}
)

# ADD_SUBDIRECTORY (bookmark)
# ADD_SUBDIRECTORY (icontheme)
# ADD_SUBDIRECTORY (browser)
# ADD_SUBDIRECTORY (configdialog)
ADD_SUBDIRECTORY (sourceviewer)
ADD_SUBDIRECTORY (webviewer)
# ADD_SUBDIRECTORY (cssviewer)

SET (xhtmldbg_SOURCES
  main.cpp
  window.cpp
  addresstoolbar.cpp
  seotoolbar.cpp
  tabbedwidget.cpp
  messanger.cpp
)

SET (xhtmldbg_HEADERS
  main.h
  window.h
  addresstoolbar.h
  seotoolbar.h
  tabbedwidget.h
  messanger.h
)

QT4_WRAP_CPP (xhtmldbg_MOC ${xhtmldbg_HEADERS})

INCLUDE_DIRECTORIES (${XHTMLDBG_BINARY_PROJECT_INCULDE})

QT4_ADD_RESOURCES (xhtmldbg_RESRC xhtmldbg.qrc)

ADD_EXECUTABLE (xhtmldbg
  ${qrc_rcresource}
  ${xhtmldbg_RESRC}
  ${xhtmldbg_MOC}
  ${xhtmldbg_UI}
  ${xhtmldbg_HEADERS}
  ${xhtmldbg_SOURCES}
)

TARGET_LINK_LIBRARIES (xhtmldbg
  ${XHTMLDBG_LIBRARIES}
  SourceViewer WebViewer
)

INSTALL (TARGETS xhtmldbg DESTINATION bin)

# INSTALL (FILES xhtmldbgdebugger.desktop DESTINATION share/applications)

## EOF
