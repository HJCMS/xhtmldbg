## HTML Pages

CONFIGURE_FILE (blank.html.cmake blank.html)
CONFIGURE_FILE (colors.html.cmake colors.html)
CONFIGURE_FILE (welcome.html.cmake welcome.html)

SET (templates
  blank.html
  colors.html
  welcome.html
)

INSTALL (FILES
  ${templates}
  DESTINATION share/xhtmldbg/html
)

## EOF
