# CMakeLists.txt

SET (subproject_SOURCES
  alternatelinkreader.cpp
  rssitem.cpp
  rssmodel.cpp
  rssparserdialog.cpp
  rssparser.cpp
)

SET (subproject_HEADERS
  alternatelinkreader.h
  rssmodel.h
  rssparserdialog.h
  rssparser.h
)

SET (subproject_NOMOCS
  rssitem.h
)

QT4_WRAP_CPP (subproject_MOC ${subproject_HEADERS})

ADD_LIBRARY (AlternateLinkReader STATIC
  ${subproject_MOC}
  ${subproject_HEADERS}
  ${subproject_NOMOCS}
  ${subproject_SOURCES}
)

TARGET_LINK_LIBRARIES (AlternateLinkReader
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
  ${QT_QTWEBKIT_LIBRARY}
)

## EOF
