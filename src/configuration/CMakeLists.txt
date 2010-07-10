# CMakeLists.txt

ADD_SUBDIRECTORY (editcookiestable)
ADD_SUBDIRECTORY (useragenteditor)
ADD_SUBDIRECTORY (certissuers)
ADD_SUBDIRECTORY (iconthemeselecter)

SET (subproject_SOURCES
  configuration.cpp
  configurationmenu.cpp
  stackedwidget.cpp
  pagewidget.cpp
  configide.cpp
  configtidy.cpp
  configdomtree.cpp
  configbrowser.cpp
  configheaderdefinitions.cpp
  configcookies.cpp
  configproxy.cpp
  configssl.cpp
  configtrustedhosts.cpp
  configaccesscontrol.cpp
  configuseragents.cpp
  configgeo.cpp
  configutils.cpp
)

SET (subproject_HEADERS
  configuration.h
  configurationmenu.h
  stackedwidget.h
  pagewidget.h
  configide.h
  configtidy.h
  configdomtree.h
  configbrowser.h
  configheaderdefinitions.h
  configcookies.h
  configproxy.h
  configssl.h
  configtrustedhosts.h
  configaccesscontrol.h
  configuseragents.h
  configgeo.h
  configutils.h
)

QT4_WRAP_CPP (subproject_MOC ${subproject_HEADERS})

REMOVE_DEFINITIONS (-DQT_DBUS_LIB -DQT_WEBKIT_LIB -DQT_XML_LIB -DQT_XMLPATTERNS_LIB)

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/useragenteditor
  ${CMAKE_CURRENT_SOURCE_DIR}/useragenteditor
  ${CMAKE_CURRENT_BINARY_DIR}/certissuers
  ${CMAKE_CURRENT_SOURCE_DIR}/certissuers
  ${CMAKE_CURRENT_BINARY_DIR}/iconthemeselecter
  ${CMAKE_CURRENT_SOURCE_DIR}/iconthemeselecter
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

ADD_LIBRARY (Configuration STATIC
  ${subproject_MOC}
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

SET_TARGET_PROPERTIES (Configuration PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (Configuration
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  EditCookiesTable
  UserAgentsEditor
  CertIssuers
  IconThemeSelecter
)

## EOF
