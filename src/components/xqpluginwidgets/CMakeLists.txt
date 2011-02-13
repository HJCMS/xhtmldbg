# CMakeLists.txt
## x-qt-plugin Support required

SET (subproject "XQPluginWidgets")

SET (LIBRARY_OUTPUT_PATH
  ${LIBRARY_OUTPUT_PATH}/designer
)

SET (subproject_FORMS
  xregistrationformcomponent.ui
)

SET (subproject_SOURCES
  widgetcollection.cpp
  widgetinterface.cpp
  xpasswordwidget.cpp
  xpasswordwidgetplugin.cpp
  xregistrationformular.cpp
  xregistrationformularplugin.cpp
)

SET (subproject_HEADERS
  widgetcollection.h
  xpasswordwidget.h
  xpasswordwidgetplugin.h
  xregistrationformular.h
  xregistrationformularplugin.h
)

IF (PHONON_FOUND)
  ADD_DEFINITIONS (-DHAVE_PHONON)
  SET (subproject_SOURCES
    ${subproject_SOURCES}
    xoggvideoplugin.cpp
    xoggvideo.cpp
  )
  SET (subproject_HEADERS
    ${subproject_HEADERS}
    xoggvideoplugin.h
    xoggvideo.h
  )
ENDIF (PHONON_FOUND)

SET (subproject_NOMOC_HEADERS
  widgetinterface.h
)

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

QT4_WRAP_UI (subproject_UIC ${subproject_FORMS})

QT4_WRAP_CPP (subproject_MOC ${subproject_HEADERS})

ADD_LIBRARY (${subproject} MODULE
  ${subproject_UIC}
  ${subproject_MOC}
  ${subproject_NOMOC_HEADERS}
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

SET_TARGET_PROPERTIES (${subproject} PROPERTIES
  OUTPUT_NAME ${subproject}
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS} -fno-strict-aliasing"
  LINK_FLAGS "${LIB_FPIC_CXXFLAGS} ${LIB_RDYNAMIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (${subproject}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTSCRIPT_LIBRARY}
  ${QT_QTDESIGNER_LIBRARY}
  ${QT_QTDESIGNERCOMPONENTS_LIBRARY}
  ${QT_QTSCRIPTTOOLS_LIBRARY}
  ${QT_QTUITOOLS_LIBRARY}
  ${QJSON_LIBRARIES}
  ${KDE4_KDECORE_LIBRARY}
  ${KDE4_KDEUI_LIBRARY}
  ${PHONON_LIBRARY}
)

FILE (MAKE_DIRECTORY browser designer)
INSTALL (DIRECTORY designer 
  DESTINATION lib${LIB_SUFFIX}/xhtmldbg
  COMPONENT Plugin
)

INSTALL (DIRECTORY browser
  DESTINATION lib${LIB_SUFFIX}/xhtmldbg
  COMPONENT Plugin
)

INSTALL (TARGETS ${subproject} LIBRARY
  DESTINATION lib${LIB_SUFFIX}/xhtmldbg/designer
  COMPONENT Plugin
)

INSTALL (FILES
  ${subproject_HEADERS}
  ${subproject_NOMOC_HEADERS}
  DESTINATION include/xhtmldbg-1.0/designer
)

IF(GETTEXT_MSGFMT_EXECUTABLE)
  ADD_SUBDIRECTORY (po)
ENDIF(GETTEXT_MSGFMT_EXECUTABLE)

## EOF
