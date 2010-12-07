# CMakeLists.txt

ADD_SUBDIRECTORY (template)

## x-qt-plugin Support
SET (OPTIONAL_LIBRARIES "")
IF (QT_QTSCRIPTTOOLS_FOUND)
  SET (OPTIONAL_LIBRARIES ${OPTIONAL_LIBRARIES}
    ${QT_QTSCRIPTTOOLS_LIBRARY}
    ${QT_QTUITOOLS_LIBRARY}
  )
ENDIF (QT_QTSCRIPTTOOLS_FOUND)

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
  widgetinterface.h
  xpasswordwidget.h
  xpasswordwidgetplugin.h
  xregistrationformular.h
  xregistrationformularplugin.h
)

QT4_WRAP_UI (subproject_UIC ${subproject_FORMS})

QT4_WRAP_CPP (subproject_MOC ${subproject_HEADERS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

REMOVE_DEFINITIONS (-DQT_DBUS_LIB -DQT_WEBKIT_LIB -DQT_XML_LIB -DQT_XMLPATTERNS_LIB)

ADD_LIBRARY (XHTMLDBGWidgets MODULE
  ${subproject_UIC}
  ${subproject_MOC}
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

SET_TARGET_PROPERTIES (XHTMLDBGWidgets PROPERTIES
  OUTPUT_NAME XHTMLDBGWidgets
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
  LINK_FLAGS "${LIB_FPIC_CXXFLAGS} ${LIB_RDYNAMIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (XHTMLDBGWidgets
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTSCRIPT_LIBRARY}
  ${QT_QTDESIGNER_LIBRARY}
  ${QT_QTDESIGNERCOMPONENTS_LIBRARY}
  ${OPTIONAL_LIBRARIES}
)

INSTALL (TARGETS XHTMLDBGWidgets
  LIBRARY DESTINATION lib${LIB_SUFFIX}/xhtmldbg/designer
  COMPONENT Plugin
)

INSTALL (FILES
  ${subproject_HEADERS}
  DESTINATION include/xhtmldbg-1.0/designer
)

## EOF
