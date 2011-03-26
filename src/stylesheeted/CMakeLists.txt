## StyleSheet Editor

SET (subproject "StyleSheeted")

ADD_SUBDIRECTORY (fontsizeadjust)
ADD_SUBDIRECTORY (fontweight)

SET (target_project_SOURCES
  actiontoolbar.cpp
  colorchooserwidget.cpp
  predicates.cpp
  fontstyle.cpp
  stylesheeted.cpp
)

SET (target_project_HEADERS
  actiontoolbar.h
  colorchooserwidget.h
  predicates.h
  fontstyle.h
  stylesheeted.h
)

SET (target_project_NOMOC_HEADERS
)

KDE4_MOC_HEADERS (target_project_MOC ${target_project_HEADERS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/fontsizeadjust
  ${CMAKE_CURRENT_SOURCE_DIR}/fontsizeadjust
  ${CMAKE_CURRENT_BINARY_DIR}/fontweight
  ${CMAKE_CURRENT_SOURCE_DIR}/fontweight
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

KDE4_ADD_LIBRARY (${subproject} STATIC
  ${target_project_MOC}
  ${target_project_HEADERS}
  ${target_project_NOMOC_HEADERS}
  ${target_project_SOURCES}
)

SET_TARGET_PROPERTIES (${subproject} PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (${subproject}
  ${XHTMLDBG_QT_LIBRARIES}
  ${XHTMLDBG_KDE_LIBRARIES}
  FontWeight
  FontSizeAdjust
)

## EOF
