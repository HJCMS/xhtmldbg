## HTML Pages

CONFIGURE_FILE (blank.html.cmake blank.html)
CONFIGURE_FILE (colors.html.cmake colors.html)
CONFIGURE_FILE (welcome.html.cmake welcome.html)
CONFIGURE_FILE (style.css.cmake style.css)

SET (templates
  ${CMAKE_CURRENT_BINARY_DIR}/blank.html
  ${CMAKE_CURRENT_BINARY_DIR}/colors.html
  ${CMAKE_CURRENT_BINARY_DIR}/welcome.html
  ${CMAKE_CURRENT_BINARY_DIR}/style.css
)

INSTALL (FILES
  ${templates}
  DESTINATION share/xhtmldbg/html
)

## EOF
