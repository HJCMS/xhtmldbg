# CMakeLists.txt
#
# XHTMLDBG Plugin Interface
#

CONFIGURE_FILE (xhtmldbginterface.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/xhtmldbginterface.h
)

SET (xhtmldbginterface_HEADERS
  ${CMAKE_CURRENT_BINARY_DIR}/xhtmldbginterface.h
)

#
# XHTMLDBG Plugin Loader
#

SET (interface_SOURCES
  xhtmldbgplugininfo.cpp
  xhtmldbgplugger.cpp
)

SET (interface_HEADERS
  xhtmldbgplugininfo.h
  xhtmldbgplugger.h
  ${xhtmldbginterface_HEADERS}
)

QT4_WRAP_CPP (interface_MOC ${interface_HEADERS})

REMOVE_DEFINITIONS (-DQT_DBUS_LIB -DQT_WEBKIT_LIB -DQT_NETWORK_LIB)

INCLUDE_DIRECTORIES (${XHTMLDBG_INCLUDE_INTERFACE})

ADD_LIBRARY (xhtmldbginterface SHARED
  ${interface_MOC}
  ${interface_HEADERS}
  ${interface_SOURCES}
  ${xhtmldbginterface_HEADERS}
)

SET_TARGET_PROPERTIES (xhtmldbginterface PROPERTIES
  VERSION ${XHTMLDBG_VERSION}
  SOVERSION ${XHTMLDBG_VERSION_MAJOR}
  OUTPUT_NAME xhtmldbg-1.0
  LINKER_LANGUAGE CXX
  LINK_FLAGS "${LIB_FPIC_CXXFLAGS} ${LIB_RDYNAMIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (xhtmldbginterface
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTXML_LIBRARY}
)

INSTALL (TARGETS xhtmldbginterface
  LIBRARY DESTINATION lib${LIB_SUFFIX}
)

CONFIGURE_FILE (xhtmldbg.pc.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/xhtmldbg.pc
)

INSTALL (FILES 
  ${CMAKE_CURRENT_BINARY_DIR}/xhtmldbg.pc
  DESTINATION lib${LIB_SUFFIX}/pkgconfig
)

INSTALL (FILES
  ${CMAKE_CURRENT_BINARY_DIR}/xhtmldbginterface.h
  ${CMAKE_CURRENT_SOURCE_DIR}/xhtmldbgplugininfo.h
  DESTINATION include/xhtmldbg-1.0
)

## EOF
 
