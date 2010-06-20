# CMakeLists.txt

FILE (MAKE_DIRECTORY ${PARSER_SCHEME_PATH})

SET (schemafiles
  atom-1.0.xsd
  datatypes.dtd
  rss-2.0.xsd
  XMLSchema.dtd
  XMLSchema.xsd
  xml.xsd
  xsd.xsl
  xslt10.dtd
)

FOREACH (scheme ${schemafiles})
  MESSAGE (STATUS "Configure ${scheme}")
  CONFIGURE_FILE (${scheme}.cmake ${PARSER_SCHEME_PATH}/${scheme})
ENDFOREACH (scheme)

FOREACH(inst ${schemafiles})
  INSTALL (FILES
    ${PARSER_SCHEME_PATH}/${inst}
    DESTINATION share/xhtmldbg/schemas/
  )
ENDFOREACH(inst)

## EOF

