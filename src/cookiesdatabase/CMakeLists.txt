# CMakeLists.txt

SET (subproject_SOURCES
  cookiesdatabaselocation.cpp
  cookiesstorage.cpp
  cookieshandle.cpp
  cookieaccesscombobox.cpp
  cookieseditortable.cpp
  cookiesboolcombobox.cpp
)

SET (subproject_HEADERS
  cookiesdatabaselocation.h
  cookiesstorage.h
  cookieshandle.h
  cookieaccesscombobox.h
  cookieseditortable.h
  cookiesboolcombobox.h
)

QT4_WRAP_CPP (subproject_MOC ${subproject_HEADERS})

QT4_ADD_RESOURCES (subproject_RESRC
  cookiesdatabase.qrc
  OPTIONS -name cookiesdatabase
)

ADD_LIBRARY (CookiesDatabase STATIC
  ${subproject_MOC}
  ${subproject_RESRC}
  ${subproject_HEADERS}
  ${subproject_STATIC_HEADERS}
  ${subproject_SOURCES}
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
