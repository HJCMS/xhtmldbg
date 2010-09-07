# CMakeLists.txt

SET (subproject_SOURCES
  pluginfactory.cpp
  pluginsfinder.cpp
  plugin.cpp
  webplugin.cpp
)

SET (subproject_HEADERS
  pluginfactory.h
  pluginsfinder.h
)

SET (subproject_NOMOC_HEADERS
  plugin.h
  webplugin.h
)

REMOVE_DEFINITIONS (-DQT_DBUS_LIB -DQT_XML_LIB -DQT_XMLPATTERNS_LIB)

QT4_WRAP_CPP (subproject_MOC ${subproject_HEADERS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
  ${MOZILLA_PLUGIN_API_INCLUDE_DIRS}
)

ADD_LIBRARY (PluginFactory STATIC
  ${subproject_MOC}
  ${subproject_HEADERS}
  ${subproject_NOMOC_HEADERS}
  ${subproject_SOURCES}
)

## -Wno-old-style-cast
SET_TARGET_PROPERTIES (PluginFactory PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS} -Wno-undef"
)

TARGET_LINK_LIBRARIES (PluginFactory
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTWEBKIT_LIBRARY}
  ${MOZILLA_PLUGIN_API_LIBRARIES}
)

## EOF
