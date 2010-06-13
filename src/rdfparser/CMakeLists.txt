# CMakeLists.txt

SET (subproject_SOURCES
  alternatelinkreader.cpp
  raptorparser.cpp
  rssitem.cpp
  rssmodel.cpp
  rssparserdialog.cpp
  rsstreeview.cpp
  rssviewer.cpp
  rsshighlighter.cpp
)

SET (subproject_HEADERS
  alternatelinkreader.h
  raptorparser.h
  rssmodel.h
  rssparserdialog.h
  rsstreeview.h
  rssviewer.h
  rsshighlighter.h
)

SET (subproject_NOMOCS
  rssitem.h
)

INCLUDE_DIRECTORIES (${XHTMLDBG_BINARY_PROJECT_INCULDE})

REMOVE_DEFINITIONS ( -DQT_DBUS_LIB )

QT4_WRAP_CPP (subproject_MOC ${subproject_HEADERS})

ADD_LIBRARY (RDFParser STATIC
  ${subproject_MOC}
  ${subproject_HEADERS}
  ${subproject_NOMOCS}
  ${subproject_SOURCES}
)

TARGET_LINK_LIBRARIES (RDFParser
  ${XHTMLDBG_LIBRARIES}
  Networker
)

## EOF
