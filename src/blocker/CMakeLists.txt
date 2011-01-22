# CMakeLists.txt

SET (networkblocker_SOURCES
  networkblocker.cpp
  databaselocation.cpp
)

SET (networkblocker_HEADERS
  networkblocker.h
  databaselocation.h
)

QT4_WRAP_CPP (networkblocker_MOC ${networkblocker_HEADERS})

ADD_LIBRARY (NetBlocker STATIC
  ${networkblocker_MOC}
  ${networkblocker_HEADERS}
  ${networkblocker_SOURCES}
)

REMOVE_DEFINITIONS (-DQT_DBUS_LIB -DQT_WEBKIT_LIB -DQT_XML_LIB -DQT_XMLPATTERNS_LIB)

SET_TARGET_PROPERTIES (NetBlocker PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (NetBlocker
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
  ${QT_QTSQL_LIBRARY}
)

## EOF
