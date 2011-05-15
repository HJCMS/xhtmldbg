# CMakeLists.txt

SET (npploader_SOURCES
  npploader.cpp
  nppattributes.cpp
  nppplugin.cpp
  nppfailurewidget.cpp
)

SET (npploader_HEADERS
  npploader.h
  nppattributes.h
  nppfailurewidget.h
)

SET (npploader_NOMOC_HEADERS
  nppplugin.h
)

KDE4_MOC_HEADERS (npploader_MOCS ${npploader_HEADERS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${XULRUNNER_INCLUDES}
)

KDE4_ADD_LIBRARY (NPPLoader STATIC
  ${npploader_MOCS}
  ${npploader_NOMOC_HEADERS}
  ${npploader_HEADERS}
  ${npploader_SOURCES}
)


ADD_DEFINITIONS (${XULRUNNER_DEFINITIONS})

SET_TARGET_PROPERTIES (NPPLoader PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS} -Wno-undef"
)

TARGET_LINK_LIBRARIES (NPPLoader
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
  ${QT_QTWEBKIT_LIBRARY}
  ${KDE4_KDECORE_LIBRARY}
  ${KDE4_KDEUI_LIBRARY}
  ${KDE4_KFILE_LIBRARY}
  ${KDE4_KDEWEBKIT_LIBRARY}
  ${XULRUNNER_LIBRARIES}
)

## EOF
