# CMakeLists.txt

SET (docking_SOURCES
  docking.cpp
  cookiesdock.cpp
  headerdock.cpp
  postdock.cpp
)

SET (docking_HEADERS
  docking.h
  cookiesdock.h
  headerdock.h
  postdock.h
)

SET (docking_STATIC_HEADERS
)

QT4_WRAP_CPP (docking_MOC ${docking_HEADERS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

ADD_LIBRARY (Docking STATIC
  ${docking_MOC}
  ${docking_HEADERS}
  ${docking_STATIC_HEADERS}
  ${docking_SOURCES}
)

TARGET_LINK_LIBRARIES (Docking
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
)

## EOF
 
