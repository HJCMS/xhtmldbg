# CMakeLists.txt

SET (sourcecache_SOURCES
  sourcecache.cpp
)

SET (sourcecache_HEADERS
  sourcecache.h
)

REMOVE_DEFINITIONS (-DQT_DBUS_LIB -DQT_WEBKIT_LIB -DQT_XML_LIB -DQT_XMLPATTERNS_LIB)

QT4_WRAP_CPP (sourcecache_MOCS ${sourcecache_HEADERS})

ADD_LIBRARY (SourceCache STATIC
  ${sourcecache_MOCS}
  ${sourcecache_HEADERS}
  ${sourcecache_SOURCES}
)

SET_TARGET_PROPERTIES (SourceCache PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

TARGET_LINK_LIBRARIES (SourceCache
  ${QT_QTCORE_LIBRARY}
  ${QT_QTXML_LIBRARY}
)

## EOF
