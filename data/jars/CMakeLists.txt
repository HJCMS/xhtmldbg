# CMakeLists.txt

SET (cssvalidator_JARFILES
  css-validator.jar
  css-validator.war
  commons-collections-3.2.1.jar
  commons-lang-2.4.jar
  commons-logging-1.1.jar
  jigadmin.jar
  jigedit.jar
  jigsaw.jar
  resolver.jar
  serializer.jar
  tagsoup-1.2.jar
  velocity-1.6.3.jar
  xercesImpl.jar
  xercesSamples.jar
  xml-apis.jar
)

INSTALL (FILES ${cssvalidator_JARFILES} DESTINATION share/xhtmldbg/jars)

## EOF
