# CMakeLists.txt

SET (cookiesdatabase_SOURCES
  cookiemanager.cpp
  cookiesdatabaselocation.cpp
  cookieaccesscombobox.cpp
  cookieseditortable.cpp
  cookiesboolcombobox.cpp
)

SET (cookiesdatabase_HEADERS
  cookiemanager.h
  cookiesdatabaselocation.h
  cookieaccesscombobox.h
  cookieseditortable.h
  cookiesboolcombobox.h
)

QT4_WRAP_CPP (cookiesdatabase_MOC ${cookiesdatabase_HEADERS})

QT4_ADD_RESOURCES (cookiesdatabase_RESRC
  cookiesdatabase.qrc
  OPTIONS -name cookiesdatabase
)

ADD_LIBRARY (CookiesDatabase STATIC
  ${cookiesdatabase_MOC}
  ${cookiesdatabase_RESRC}
  ${cookiesdatabase_HEADERS}
  ${cookiesdatabase_STATIC_HEADERS}
  ${cookiesdatabase_SOURCES}
)

REMOVE_DEFINITIONS (-DQT_DBUS_LIB -DQT_WEBKIT_LIB -DQT_XML_LIB -DQT_XMLPATTERNS_LIB)

SET_TARGET_PROPERTIES (CookiesDatabase PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (CookiesDatabase
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
  ${QT_QTSQL_LIBRARY}
)

## EOF
