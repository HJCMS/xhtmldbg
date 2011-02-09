## CMakeLists.txt

ADD_SUBDIRECTORY (schemas)

# KDE4_ADD_APP_ICON (xhtmldbg_icons_SRCS "icontheme/*/apps/xhtmldbg.*")

## Default IconTheme
SET (icon_sizes
  16
  22
  24
  32
  36
  48
  64
  72
  96
  128
  192
)

FOREACH(s ${icon_sizes})
  INSTALL (FILES
    icontheme/${s}/apps/xhtmldbg.png
    DESTINATION ${ICON_INSTALL_DIR}/oxygen/${s}x${s}/apps/
  )
ENDFOREACH(s)

## Pixmaps
INSTALL (FILES
  pixmaps/xhtmldbg.xpm
  DESTINATION share/pixmaps/
)

##EOF
