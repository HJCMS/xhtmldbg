# CMakeLists.txt
# Plugins Directory

IF (BIND_UTIL_DIG)
  ADD_SUBDIRECTORY (hostinfo)
ENDIF (BIND_UTIL_DIG)

ADD_SUBDIRECTORY (selfhtmlsidebar)
ADD_SUBDIRECTORY (webkitinspector)

## Development Template Data
INSTALL (FILES
  template/CMakeLists.txt
  template/helloworld.cpp
  template/helloworld.h
  template/helloworldplugin.cpp
  template/helloworldplugin.h
  DESTINATION share/xhtmldbg/plugintemplate
)

## EOF
