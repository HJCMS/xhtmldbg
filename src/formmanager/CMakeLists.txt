## CMakeLists.txt

SET (subproject "FormManager")

SET (target_project_SOURCES
  formmanager.cpp
  formdblist.cpp
  formconstructor.cpp
)

SET (target_project_HEADERS
  formmanager.h
  formdblist.h
)

SET (target_project_NOMOC_HEADERS
  formconstructor.h
)

KDE4_MOC_HEADERS (target_project_MOC ${target_project_HEADERS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
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
  DBManager
)

## EOF

