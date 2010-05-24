# CMakeLists.txt

SET (subproject_TARGET DownloadManager)

SET (subproject_SOURCES
  historymanager.cpp
)

SET (subproject_HEADERS
  historymanager.h
)

SET (subproject_STATIC_HEADERS
)

QT4_WRAP_CPP (subproject_MOC ${subproject_HEADERS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

ADD_LIBRARY (${subproject_TARGET} STATIC
  ${subproject_MOC}
  ${subproject_HEADERS}
  ${subproject_STATIC_HEADERS}
  ${subproject_SOURCES}
)

TARGET_LINK_LIBRARIES (${subproject_TARGET}
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
)

## EOF
