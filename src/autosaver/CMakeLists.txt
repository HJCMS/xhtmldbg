# CMakeLists.txt
##
# This Class is a part of the demonstration applications of the Qt Toolkit.
##

SET (subproject_SOURCES autosaver.cpp)
SET (subproject_HEADERS autosaver.h)

QT4_WRAP_CPP (subproject_MOC ${subproject_HEADERS})

ADD_LIBRARY (AutoSaver STATIC
  ${subproject_MOC}
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

REMOVE_DEFINITIONS (-DQT_DBUS_LIB -DQT_WEBKIT_LIB -DQT_XML_LIB -DQT_XMLPATTERNS_LIB)

TARGET_LINK_LIBRARIES (AutoSaver
  ${QT_QTCORE_LIBRARY}
)

## EOF
