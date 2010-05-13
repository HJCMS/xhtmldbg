# CMakeLists.txt

SET (cssvalidator_UIFILES
  cssconfiguremain.ui
)

SET (cssvalidator_SOURCES
  validator.cpp
  cssvalidator.cpp
  soupreader.cpp
  validatormenu.cpp
  cssconfigure.cpp
)

SET (cssvalidator_HEADERS
  validator.h
  cssvalidator.h
  soupreader.h
  validatormenu.h
  cssconfigure.h
)

SET (cssvalidator_NOMOC
)

QT4_WRAP_UI (cssvalidator_UIC ${cssvalidator_UIFILES})

QT4_WRAP_CPP (cssvalidator_MOC ${cssvalidator_HEADERS})

ADD_DEFINITIONS (${QTIDY_DEFINITIONS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

ADD_LIBRARY (CSSValidator STATIC
  ${cssvalidator_UIC}
  ${cssvalidator_MOC}
  ${cssvalidator_NOMOC}
  ${cssvalidator_HEADERS}
  ${cssvalidator_SOURCES}
)

TARGET_LINK_LIBRARIES (CSSValidator
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTXML_LIBRARY}
)

## EOF
