# CMakeLists.txt

CONFIGURE_FILE (ts2gettext.xslt.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/ts2gettext.xslt
)

SET (languages
  de
)

EXEC_PROGRAM (${QT_QMAKE_EXECUTABLE} ARGS "-query QT_INSTALL_TRANSLATIONS" OUTPUT_VARIABLE INSTALL_TRANSLATIONS)

FILE (MAKE_DIRECTORY ${ARCHIVE_OUTPUT_PATH})
FOREACH (lng ${languages})
  EXEC_PROGRAM (${QT_LRELEASE_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ARGS -compress -silent -nounfinished qt_${lng}.ts xhtmldbg_${lng}.ts -qm ${ARCHIVE_OUTPUT_PATH}/xhtmldbg_${lng}.qm
  )
  EXEC_PROGRAM (${QT_LRELEASE_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ARGS -compress -silent -nounfinished xhtmldbgwidgets_${lng}.ts -qm ${ARCHIVE_OUTPUT_PATH}/xhtmldbgwidgets_${lng}.qm
  )
ENDFOREACH (lng)

INSTALL (FILES 
  ${ARCHIVE_OUTPUT_PATH}/xhtmldbg_de.qm
  ${ARCHIVE_OUTPUT_PATH}/xhtmldbgwidgets_de.qm
  DESTINATION ${QT_TRANSLATIONS_DIR}
)

INSTALL (FILES
  xhtmldbg_untranslated.ts
  xhtmldbgwidgets_untranslated.ts
  DESTINATION share/xhtmldbg
)

## EOF
