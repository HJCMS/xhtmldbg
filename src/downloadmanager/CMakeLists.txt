# CMakeLists.txt

SET (subproject_SOURCES
  downloadmanager.cpp
  downloader.cpp
  downloadstable.cpp
  downloadstablemodel.cpp
  downloadsinfo.cpp
)

SET (subproject_HEADERS
  downloadmanager.h
  downloader.h
  downloadstable.h
  downloadstablemodel.h
  downloadsinfo.h
)

SET (subproject_STATIC_HEADERS
)



KDE4_MOC_HEADERS (subproject_MOC ${subproject_HEADERS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

KDE4_ADD_LIBRARY (DownloadManager STATIC
  ${subproject_MOC}
  ${subproject_HEADERS}
  ${subproject_STATIC_HEADERS}
  ${subproject_SOURCES}
)

SET_TARGET_PROPERTIES (DownloadManager PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (DownloadManager
  AutoSaver
  Settings
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
)

## EOF
