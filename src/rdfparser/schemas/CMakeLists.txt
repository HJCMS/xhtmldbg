# CMakeLists.txt

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
