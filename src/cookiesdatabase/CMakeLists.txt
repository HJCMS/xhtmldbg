# CMakeLists.txt

SET (cookiesdatabase_SOURCES
  cookiemanager.cpp
  cookieaccesscombobox.cpp
  cookieseditortable.cpp
  cookiesboolcombobox.cpp
)

SET (cookiesdatabase_HEADERS
  cookiemanager.h
  cookieaccesscombobox.h
  cookieseditortable.h
  cookiesboolcombobox.h
)

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

KDE4_MOC_HEADERS (cookiesdatabase_MOC ${cookiesdatabase_HEADERS})

KDE4_ADD_LIBRARY (CookiesDatabase STATIC
  ${cookiesdatabase_MOC}
  ${cookiesdatabase_HEADERS}
  ${cookiesdatabase_STATIC_HEADERS}
  ${cookiesdatabase_SOURCES}
)



SET_TARGET_PROPERTIES (CookiesDatabase PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (CookiesDatabase
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
  ${QT_QTSQL_LIBRARY}
  DBManager
)

## EOF
