## CMakeLists.txt

ADD_SUBDIRECTORY (schemas)

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
    DESTINATION share/icons/oxygen/${s}x${s}/apps/
  )
ENDFOREACH(s)

## SVG scalable
INSTALL (FILES
  icontheme/scalable/apps/xhtmldbg.svgz
  DESTINATION share/icons/oxygen/scalable/apps/
)

## Pixmaps
INSTALL (FILES
  pixmaps/xhtmldbg.xpm
  DESTINATION share/pixmaps/
)

##EOF
