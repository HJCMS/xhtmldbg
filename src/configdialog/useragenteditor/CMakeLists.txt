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

REMOVE_DEFINITIONS (-DQT_DBUS_LIB -DQT_WEBKIT_LIB -DQT_XML_LIB -DQT_XMLPATTERNS_LIB)

QT4_WRAP_CPP (src_MOC ${src_HEADERS})

ADD_LIBRARY (UserAgentsEditor STATIC
  ${src_MOC}
  ${src_HEADERS}
  ${src_SOURCES}
)

TARGET_LINK_LIBRARIES (UserAgentsEditor
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
)

## EOF
