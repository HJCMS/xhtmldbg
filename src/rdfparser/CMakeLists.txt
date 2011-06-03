# CMakeLists.txt

SET (subproject_SOURCES
  alternatelinkreader.cpp
  xsdparser.cpp
  rssparserdialog.cpp
  rsstreeview.cpp
  rssviewer.cpp
  rsshighlighter.cpp
)

IF(FORCE_RAPTOR2 AND RAPTOR_FOUND)
  SET (subproject_SOURCES
    ${subproject_SOURCES}
    raptorparser2.cpp
  )
ELSE(FORCE_RAPTOR2 AND RAPTOR_FOUND)
  SET (subproject_SOURCES
    ${subproject_SOURCES}
    raptorparser.cpp
  )
ENDIF(FORCE_RAPTOR2 AND RAPTOR_FOUND)

SET (subproject_HEADERS
  alternatelinkreader.h
  raptorparser.h
  xsdparser.h
  rssparserdialog.h
  rsstreeview.h
  rssviewer.h
  rsshighlighter.h
)

INCLUDE_DIRECTORIES (${XHTMLDBG_BINARY_PROJECT_INCULDE} ${RAPTOR_INCLUDE_DIR})

ADD_DEFINITIONS(${RAPTOR_DEFINITIONS})

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
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTXML_LIBRARY}
  ${QT_QTXMLPATTERNS_LIBRARY}
  ${RAPTOR_LIBRARY}
  Networker
)

## EOF
