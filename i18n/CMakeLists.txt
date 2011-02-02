# CMakeLists.txt

SET (LANGUAGES
  de
)

EXEC_PROGRAM (${QT_QMAKE_EXECUTABLE} ARGS "-query QT_INSTALL_TRANSLATIONS" OUTPUT_VARIABLE INSTALL_TRANSLATIONS)

FILE (MAKE_DIRECTORY ${ARCHIVE_OUTPUT_PATH})
FOREACH (lng ${LANGUAGES})
  EXEC_PROGRAM (${QT_LRELEASE_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ARGS -compress -silent -nounfinished qt_${lng}.ts xhtmldbg_${lng}.ts -qm ${ARCHIVE_OUTPUT_PATH}/xhtmldbg_${lng}.qm
  )
  EXEC_PROGRAM (${QT_LRELEASE_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ARGS -compress -silent -nounfinished xhtmldbgwidgets_${lng}.ts -qm ${ARCHIVE_OUTPUT_PATH}/xhtmldbgwidgets_${lng}.qm
  )
ENDFOREACH (lng)

## add gettext bindings for KDE4
## http://translate.sourceforge.net/wiki/toolkit/ts2po
IF (TS2PO AND GETTEXT_FOUND)
  ## xhtmldbg
  EXEC_PROGRAM (${TS2PO}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ARGS --pot --output=po/xhtmldbg.pot --input=xhtmldbg_untranslated.ts
  )
  FOREACH (lng ${LANGUAGES})
    FILE (MAKE_DIRECTORY po/${lng})
    FILE (WRITE po/${lng}/.gitignore "*.po")
    EXEC_PROGRAM (${TS2PO}
      ${CMAKE_CURRENT_SOURCE_DIR}
      ARGS --output=po/${lng}/xhtmldbg.po --input=xhtmldbg_${lng}.ts
    )
  ENDFOREACH (lng)
  ## xhtmldbgwidgets
  EXEC_PROGRAM (${TS2PO}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ARGS --pot --output=po/xhtmldbgwidgets.pot --input=xhtmldbgwidgets_untranslated.ts
  )
  FOREACH (lng ${LANGUAGES})
    FILE (MAKE_DIRECTORY po/${lng})
    EXEC_PROGRAM (${TS2PO}
      ${CMAKE_CURRENT_SOURCE_DIR}
      ARGS --output=po/${lng}/xhtmldbgwidgets.po --input=xhtmldbgwidgets_${lng}.ts
    )
  ENDFOREACH (lng)
  ## add gettext directory
  ADD_SUBDIRECTORY (po)
ENDIF(TS2PO AND GETTEXT_FOUND)

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
