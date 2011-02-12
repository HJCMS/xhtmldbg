# CMakeLists.txt

SET (subproject_SOURCES
  certissuertable.cpp
  certissuers.cpp
)

SET (subproject_HEADERS
  certissuertable.h
  certissuers.h
)

KDE4_MOC_HEADERS (subproject_MOC ${subproject_HEADERS})



INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

KDE4_ADD_LIBRARY (CertIssuers STATIC
  ${subproject_MOC}
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

SET_TARGET_PROPERTIES (CertIssuers PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (CertIssuers
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
)

## EOF
