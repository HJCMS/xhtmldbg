# CMakeLists.txt

SET (src_SOURCES 
  useragentstable.cpp
  useragentstablemodel.cpp
  useragent.cpp
  useragenteditor.cpp
  droptoclipboard.cpp
)

SET (src_HEADERS 
  useragentstable.h
  useragentstablemodel.h
  useragent.h
  useragenteditor.h
  droptoclipboard.h
)



KDE4_MOC_HEADERS (src_MOC ${src_HEADERS})

KDE4_ADD_LIBRARY (UserAgentsEditor STATIC
  ${src_MOC}
  ${src_HEADERS}
  ${src_SOURCES}
)

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

SET_TARGET_PROPERTIES (UserAgentsEditor PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (UserAgentsEditor
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  Settings
)

## EOF
