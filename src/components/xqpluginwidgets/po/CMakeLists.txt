# CMakeLists.txt

SET(_catalog_name xhtmldbgwidgets)

ADD_CUSTOM_TARGET(xhtmldbgwidgets_gettext ALL)

FILE(GLOB PO_FILES xhtmldbgwidgets_*.po)

FOREACH(_poFile ${PO_FILES})
  GET_FILENAME_COMPONENT(_poFileName ${_poFile} NAME)
  STRING(REGEX REPLACE "^${_catalog_name}_?" "" _langCode ${_poFileName} )
  STRING(REGEX REPLACE "\\.po$" "" _langCode ${_langCode} )
  IF( _langCode )
    GET_FILENAME_COMPONENT(_lang ${_poFile} NAME_WE)
    SET(_gmoFile ${CMAKE_CURRENT_BINARY_DIR}/${_lang}.gmo)
    ADD_CUSTOM_COMMAND(TARGET translations
            COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} --check -o ${_gmoFile} ${_poFile}
            DEPENDS ${_poFile})
    INSTALL(FILES ${_gmoFile} DESTINATION ${LOCALE_INSTALL_DIR}/${_langCode}/LC_MESSAGES/ RENAME ${_catalog_name}.mo)
  ENDIF( _langCode )
ENDFOREACH(_poFile ${PO_FILES})
