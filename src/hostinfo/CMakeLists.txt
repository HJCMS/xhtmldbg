## Resolv Host Information Plugin

SET(hostinfo_SOURCES
  hostinfo.cpp
  hostinfodialog.cpp
)

SET(hostinfo_HEADERS
  hostinfo.h
  hostinfodialog.h
)



KDE4_MOC_HEADERS (hostinfo_MOCS ${hostinfo_HEADERS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_INCLUDE_INTERFACE}
)

KDE4_ADD_LIBRARY (HostInfo STATIC
  ${hostinfo_SOURCES}
  ${hostinfo_MOCS}
  ${hostinfo_HEADERS}
)

SET_TARGET_PROPERTIES (HostInfo PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (HostInfo
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
)

##EOF
