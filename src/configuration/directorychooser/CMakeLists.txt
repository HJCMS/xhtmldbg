# CMakeLists.txt

SET (subproject_SOURCES
  directorychooser.cpp
  directorydialog.cpp
)

SET (subproject_HEADERS
  directorychooser.h
  directorydialog.h
)



KDE4_MOC_HEADERS (subproject_MOC ${subproject_HEADERS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

KDE4_ADD_LIBRARY (DirectoryChooser STATIC
  ${subproject_MOC}
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

SET_TARGET_PROPERTIES (DirectoryChooser PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (DirectoryChooser
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  Settings
)

## EOF
 
