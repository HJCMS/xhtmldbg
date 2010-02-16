# CMakeLists.txt

SET (sourceview_SOURCES
  webviewer.cpp
  viewer.cpp
  page.cpp
)

SET (sourceview_HEADERS
  webviewer.h
  viewer.h
  page.h
)

QT4_WRAP_CPP (sourceview_MOC ${sourceview_HEADERS})

ADD_DEFINITIONS (${QTIDY_DEFINITIONS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${QTIDY_BINARY_PROJECT_INCULDE}
)

ADD_LIBRARY (WebViewer STATIC
  ${sourceview_MOC}
  ${sourceview_HEADERS}
  ${sourceview_SOURCES}
)

TARGET_LINK_LIBRARIES (WebViewer
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
  ${QT_QTWEBKIT_LIBRARY}
)

## EOF
 
