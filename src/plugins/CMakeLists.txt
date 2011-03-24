# CMakeLists.txt
# Plugins Directory

ADD_SUBDIRECTORY (selfhtmlsidebar)
ADD_SUBDIRECTORY (rfcbrowser)
ADD_SUBDIRECTORY (keywordeditor)

## Development Template Data
INSTALL (FILES
  template/CMakeLists.txt
  template/helloworld.cpp
  template/helloworld.h
  template/helloworldplugin.cpp
  template/helloworldplugin.h
  DESTINATION share/xhtmldbg/plugintemplate
)

## EOF
