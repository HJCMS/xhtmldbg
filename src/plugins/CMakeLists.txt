# CMakeLists.txt
# Plugins Directory

IF (BIND_UTIL_DIG)
  ADD_SUBDIRECTORY (hostinfo)
ENDIF (BIND_UTIL_DIG)

## This Plugins only work with UNIX and glibc >= 2.8
IF (UNIX AND NOT APPLE)
  ADD_SUBDIRECTORY (xdebugger)
ENDIF (UNIX AND NOT APPLE)

ADD_SUBDIRECTORY (template)

## EOF
