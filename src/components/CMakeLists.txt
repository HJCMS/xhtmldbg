# CMakeLists.txt
## x-qt-plugin Support required

ADD_SUBDIRECTORY (template)

IF (QT_QTSCRIPTTOOLS_FOUND)

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

  ADD_LIBRARY (XQPluginWidgets MODULE
    ${subproject_UIC}
    ${subproject_MOC}
    ${subproject_HEADERS}
    ${subproject_SOURCES}
  )

  SET_TARGET_PROPERTIES (XQPluginWidgets PROPERTIES
    OUTPUT_NAME XQPluginWidgets
    LINKER_LANGUAGE CXX
    COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS} -fno-strict-aliasing"
    LINK_FLAGS "${LIB_FPIC_CXXFLAGS} ${LIB_RDYNAMIC_CXXFLAGS}"
  )

  TARGET_LINK_LIBRARIES (XQPluginWidgets
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTSCRIPT_LIBRARY}
    ${QT_QTDESIGNER_LIBRARY}
    ${QT_QTDESIGNERCOMPONENTS_LIBRARY}
    ${QT_QTSCRIPTTOOLS_LIBRARY}
    ${QT_QTUITOOLS_LIBRARY}
  )

  INSTALL (TARGETS XQPluginWidgets
    LIBRARY DESTINATION lib${LIB_SUFFIX}/xhtmldbg/designer
    COMPONENT Plugin
  )

  INSTALL (FILES
    ${subproject_HEADERS}
    DESTINATION include/xhtmldbg-1.0/designer
  )

ENDIF (QT_QTSCRIPTTOOLS_FOUND)

## EOF
