# CMakeLists.txt
# Plugins Directory

## This Plugins only work with UNIX and glibc >= 2.8
IF (UNIX AND NOT APPLE)
  ADD_SUBDIRECTORY (hostinfo)
  ADD_SUBDIRECTORY (xdebugger)
ENDIF (UNIX AND NOT APPLE)

ADD_SUBDIRECTORY (template)

## EOF
