# CMakeLists.txt

SET (target_plugin XPasswordDialog)

SET (LIBRARY_OUTPUT_PATH
  ${LIBRARY_OUTPUT_PATH}/designer
)

SET (subproject_SOURCES
  passworddialog.cpp
  passworddialogplugin.cpp
)

SET (subproject_HEADERS
  passworddialog.h
  passworddialogplugin.h
)

QT4_WRAP_CPP (subproject_MOC ${subproject_HEADERS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

REMOVE_DEFINITIONS (-DQT_DBUS_LIB -DQT_WEBKIT_LIB -DQT_XML_LIB -DQT_XMLPATTERNS_LIB)

ADD_LIBRARY (${target_plugin} MODULE
  ${subproject_MOC}
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

SET_TARGET_PROPERTIES (${target_plugin} PROPERTIES
  OUTPUT_NAME ${target_plugin}
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
  LINK_FLAGS "${LIB_FPIC_CXXFLAGS} ${LIB_RDYNAMIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (${target_plugin}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTSCRIPT_LIBRARY}
  ${QT_QTDESIGNER_LIBRARY}
  ${QT_QTDESIGNERCOMPONENTS_LIBRARY}
)

INSTALL (TARGETS ${target_plugin}
  LIBRARY DESTINATION lib${LIB_SUFFIX}/xhtmldbg/designer
  COMPONENT Plugin
)

INSTALL (FILES
  ${CMAKE_CURRENT_BINARY_DIR}/passworddialog.h
  DESTINATION include/xhtmldbg-1.0/designer
)

## EOF
