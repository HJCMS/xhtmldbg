# CMakeLists.txt

SET (subproject_SOURCES
  alternatelinkreader.cpp
  raptorparser.cpp
  xsdparser.cpp
  rssparserdialog.cpp
  rsstreeview.cpp
  rssviewer.cpp
  rsshighlighter.cpp
)

SET (subproject_HEADERS
  alternatelinkreader.h
  raptorparser.h
  xsdparser.h
  rssparserdialog.h
  rsstreeview.h
  rssviewer.h
  rsshighlighter.h
)

INCLUDE_DIRECTORIES (${XHTMLDBG_BINARY_PROJECT_INCULDE})

REMOVE_DEFINITIONS (-DQT_DBUS_LIB)

KDE4_MOC_HEADERS (subproject_MOC ${subproject_HEADERS})

KDE4_ADD_LIBRARY (RDFParser STATIC
  ${subproject_MOC}
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

SET_TARGET_PROPERTIES (RDFParser PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (RDFParser
  ${XHTMLDBG_LIBRARIES}
  ${QT_QTXMLPATTERNS_LIBRARY}
  Networker
)

## EOF
