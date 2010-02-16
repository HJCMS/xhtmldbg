# CMakeLists.txt

SET (XHTMLDBG_BINARY_PROJECT_INCULDE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
#   ${CMAKE_CURRENT_SOURCE_DIR}/bookmark
#   ${CMAKE_CURRENT_SOURCE_DIR}/icontheme
#   ${CMAKE_CURRENT_SOURCE_DIR}/browser
#   ${CMAKE_CURRENT_SOURCE_DIR}/configdialog
#   ${CMAKE_CURRENT_SOURCE_DIR}/sourceview
#   ${CMAKE_CURRENT_SOURCE_DIR}/metatab
#   ${CMAKE_CURRENT_SOURCE_DIR}/cssviewer
  ${XHTMLDBG_INCLUDES}
)

# ADD_SUBDIRECTORY (bookmark)
# ADD_SUBDIRECTORY (icontheme)
# ADD_SUBDIRECTORY (browser)
# ADD_SUBDIRECTORY (configdialog)
# ADD_SUBDIRECTORY (sourceview)
# ADD_SUBDIRECTORY (metatab)
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

## FIXME add_custom_command doesn't override OPTIONS to rcc command !!!
# SET (qrc_rcresource qrc_xhtmldbgresource.cxx)
# ADD_CUSTOM_COMMAND (OUTPUT ${qrc_rcresource}
#   COMMAND ${QT_RCC_EXECUTABLE} ARGS -name xhtmldbgresource -o ${qrc_rcresource} ${XHTMLDBG_RESOURCE_FILE}
#   DEPENDS debuggerapplication.cpp
# )

ADD_EXECUTABLE (xhtmldbg
  ${qrc_rcresource}
  ${xhtmldbg_MOC}
  ${xhtmldbg_UI}
  ${xhtmldbg_HEADERS}
  ${xhtmldbg_SOURCES}
)

TARGET_LINK_LIBRARIES (xhtmldbg
  ${XHTMLDBG_LIBRARIES}
)

INSTALL (TARGETS xhtmldbg DESTINATION bin)

# INSTALL (FILES xhtmldbgdebugger.desktop DESTINATION share/applications)

## EOF
