# CMakeLists.txt

SET (docking_SOURCES
  docking.cpp
  docktreewidget.cpp
  headerdock.cpp
)

SET (docking_HEADERS
  docking.h
  docktreewidget.h
  headerdock.h
)

SET (docking_STATIC_HEADERS
)

KDE4_MOC_HEADERS (docking_MOC ${docking_HEADERS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

KDE4_ADD_LIBRARY (Docking STATIC
  ${docking_MOC}
  ${docking_HEADERS}
  ${docking_STATIC_HEADERS}
  ${docking_SOURCES}
)

SET_TARGET_PROPERTIES (Docking PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (Docking
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
  Networker
)

## EOF
 
