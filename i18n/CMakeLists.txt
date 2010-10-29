# CMakeLists.txt

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
ENDFOREACH (lng)

INSTALL (FILES 
  ${ARCHIVE_OUTPUT_PATH}/xhtmldbg_de.qm
  DESTINATION ${QT_TRANSLATIONS_DIR}
)

INSTALL (FILES
  xhtmldbg_untranslated.ts
  DESTINATION share/xhtmldbg
)

##############################################################
#
# Additionally it provides the following macros:
# GETTEXT_CREATE_TRANSLATIONS ( outputFile [ALL] file1 ... fileN )
#    This will create a target "translations" which will convert the 
#    given input po files into the binary output mo file. If the 
#    ALL option is used, the translations will also be created when
#    building the default target.
# GETTEXT_PROCESS_POT( <potfile> [ALL] [INSTALL_DESTINATION <destdir>] <lang1> <lang2> ... )
#     Process the given pot file to mo files.
#     If INSTALL_DESTINATION is given then automatically install rules will be created,
#     the language subdirectory will be taken into account (by default use share/locale/).
#     If ALL is specified, the pot file is processed when building the all traget.
#     It creates a custom target "potfile".
#
# GETTEXT_PROCESS_PO_FILES(<lang> [ALL] [INSTALL_DESTINATION <dir>] <po1> <po2> ...)
#     Process the given po files to mo files for the given language.
#     If INSTALL_DESTINATION is given then automatically install rules will be created,
#     the language subdirectory will be taken into account (by default use share/locale/).
#     If ALL is specified, the po files are processed when building the all traget.
#     It creates a custom target "pofiles".
##############################################################

# SET (gettext_langs
#   de.po
# )
# 
# IF (GETTEXT_FOUND)
#   GETTEXT_PROCESS_PO_FILES ( ALL
#     
#     ${gettext_langs}
#   )
# ENDIF (GETTEXT_FOUND)

## EOF
