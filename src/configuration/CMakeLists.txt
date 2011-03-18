# CMakeLists.txt

ADD_SUBDIRECTORY (useragenteditor)
ADD_SUBDIRECTORY (certissuers)
ADD_SUBDIRECTORY (iconthemeselecter)
ADD_SUBDIRECTORY (directorychooser)

SET (configuration_SOURCES
  configuration.cpp
  configurationmenu.cpp
  stackedwidget.cpp
  featurebox.cpp
  pagewidget.cpp
  configide.cpp
  configtidy.cpp
  configdomtree.cpp
  configbrowser.cpp
  confignetwork.cpp
  configheaderdefinitions.cpp
  configcookies.cpp
  configproxy.cpp
  configssl.cpp
  configtrustedhosts.cpp
  configaccesscontrol.cpp
  configuseragents.cpp
  configwebsecurity.cpp
  configextras.cpp
  configutils.cpp
)

SET (configuration_HEADERS
  configuration.h
  configurationmenu.h
  stackedwidget.h
  featurebox.h
  pagewidget.h
  configide.h
  configtidy.h
  configdomtree.h
  configbrowser.h
  confignetwork.h
  configheaderdefinitions.h
  configcookies.h
  configproxy.h
  configssl.h
  configtrustedhosts.h
  configaccesscontrol.h
  configuseragents.h
  configwebsecurity.h
  configextras.h
  configutils.h
)

KDE4_MOC_HEADERS (configuration_MOC ${configuration_HEADERS})



INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/useragenteditor
  ${CMAKE_CURRENT_SOURCE_DIR}/useragenteditor
  ${CMAKE_CURRENT_BINARY_DIR}/certissuers
  ${CMAKE_CURRENT_SOURCE_DIR}/certissuers
  ${CMAKE_CURRENT_BINARY_DIR}/iconthemeselecter
  ${CMAKE_CURRENT_SOURCE_DIR}/iconthemeselecter
  ${CMAKE_CURRENT_BINARY_DIR}/directorychooser
  ${CMAKE_CURRENT_SOURCE_DIR}/directorychooser
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

KDE4_ADD_LIBRARY (Configuration STATIC
  ${configuration_MOC}
  ${configuration_HEADERS}
  ${configuration_SOURCES}
)

SET_TARGET_PROPERTIES (Configuration PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (Configuration
  ${XHTMLDBG_QT_LIBRARIES}
  ${QTIDY_LIBRARIES}
  ${XHTMLDBG_KDE_LIBRARIES}
  DBManager
  CookiesDatabase
  UserAgentsEditor
  CertIssuers
  IconThemeSelecter
  DirectoryChooser
  Settings
)

## EOF
