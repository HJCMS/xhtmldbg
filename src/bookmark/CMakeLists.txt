# CMakeLists.txt

SET (bookmark_SOURCES
  bookmark.cpp
  bookmarkitem.cpp
  bookmarkeditor.cpp
  bookmarkreader.cpp
  bookmarktreereader.cpp
  bookmarkwriter.cpp
)

SET (bookmark_HEADERS
  bookmark.h
  bookmarkitem.h
  bookmarkeditor.h
  bookmarkreader.h
  bookmarkwriter.h
)

SET (bookmark_STATIC_HEADERS
  bookmarktreereader.h
)

QT4_WRAP_CPP (bookmark_MOC ${bookmark_HEADERS})

QT4_ADD_RESOURCES (bookmark_RESRC bookmark.qrc)

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

ADD_LIBRARY (Bookmark STATIC
  ${bookmark_MOC}
  ${bookmark_RESRC}
  ${bookmark_HEADERS}
  ${bookmark_STATIC_HEADERS}
  ${bookmark_SOURCES}
)

TARGET_LINK_LIBRARIES (Bookmark
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
)

## EOF
 
