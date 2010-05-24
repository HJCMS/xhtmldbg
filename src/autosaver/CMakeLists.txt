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

TARGET_LINK_LIBRARIES (AutoSaver
  ${QT_QTCORE_LIBRARY}
)

## EOF
