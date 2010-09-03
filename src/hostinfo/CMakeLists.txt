## Resolv Host Information Plugin

SET (target_subproject HostInfo)

SET(subproject_SOURCES
  hostinfo.cpp
)

SET(subproject_HEADERS
  hostinfo.h
)

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_INCLUDE_INTERFACE}
)

QT4_WRAP_CPP (subproject_MOCS ${subproject_HEADERS})

REMOVE_DEFINITIONS (-DQT_DBUS_LIB -DQT_XML_LIB -DQT_WEBKIT_LIB -DQT_XMLPATTERNS_LIB)

ADD_LIBRARY (${target_subproject} STATIC
  ${subproject_SOURCES}
  ${subproject_RESOURCE}
  ${subproject_MOCS}
)

TARGET_LINK_LIBRARIES (${target_subproject}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
)

SET_TARGET_PROPERTIES (${target_subproject} PROPERTIES
  OUTPUT_NAME ${target_subproject}
  LINKER_LANGUAGE CXX
  LINK_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

##EOF
