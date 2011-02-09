# CMakeLists.txt
##
# This Class is a part of the demonstration applications of the Qt Toolkit.
##

SET (autosaver_SOURCES autosaver.cpp)
SET (autosaver_HEADERS autosaver.h)

KDE4_MOC_HEADERS (autosaver_MOC ${autosaver_HEADERS})

KDE4_ADD_LIBRARY (AutoSaver STATIC
  ${autosaver_MOC}
  ${autosaver_SOURCES}
  ${autosaver_HEADERS}
)

REMOVE_DEFINITIONS (-DQT_DBUS_LIB -DQT_WEBKIT_LIB -DQT_XML_LIB -DQT_XMLPATTERNS_LIB)

SET_TARGET_PROPERTIES (AutoSaver PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (AutoSaver
  ${QT_QTCORE_LIBRARY}
)

## EOF
