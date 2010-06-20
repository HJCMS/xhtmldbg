# CMakeLists.txt

CONFIGURE_FILE (atom-1.0.xsd.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/atom-1.0.xsd
)

CONFIGURE_FILE (rss-2.0.xsd.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/rss-2.0.xsd
)

CONFIGURE_FILE (XMLSchema.xsd.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/XMLSchema.xsd
)

SET (schema_FILES
  atom-1.0.xsd
  rss-2.0.xsd
  XMLSchema.dtd
  XMLSchema.xsd
  xml.xsd
)

INSTALL (FILES
  ${schema_FILES}
  DESTINATION share/xhtmldbg/schemas/
)

## EOF
