# CMakeLists.txt
# Plugins Directory

IF (BIND_UTIL_DIG)
  ADD_SUBDIRECTORY (hostinfo)
ENDIF (BIND_UTIL_DIG)

## This Plugins only work with UNIX and glibc >= 2.10
IF (XHTMLDBG_BETA_EXTENSION)
  IF (UNIX AND NOT APPLE)
    ADD_SUBDIRECTORY (xdebugger)
  ENDIF (UNIX AND NOT APPLE)
ENDIF (XHTMLDBG_BETA_EXTENSION)

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
