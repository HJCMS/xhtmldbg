## gettext

SET (POT_FILES
  xhtmldbg.pot
  xhtmldbgwidgets.pot
)

FOREACH (lng ${LANGUAGES})
  GETTEXT_PROCESS_PO_FILES (${lng} ALL
    INSTALL_DESTINATION share/locale/
    ${lng}/xhtmldbg.po ${lng}/xhtmldbgwidgets.po
  )
ENDFOREACH (lng)
