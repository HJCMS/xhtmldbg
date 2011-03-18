project                      = @PROJECT_NAME@
application                  = @PROJECT_NAME@
version                      = @XHTMLDBG_VERSION@
description                  = XHTML/HTML Debugger Project
url                          = http://xhtmldbg.hjcms.de/doc/
indexes                      = @QT_DOC_PATH@/html/qt.index
# The outputdir variable specifies the directory 
# where QDoc will put the generated documentation.
outputdir                    = html

qhp.projects                       = xhtmldbg
qhp.xhtmldbg.namespace             = de.hjcms.xhtmldbg.08
qhp.xhtmldbg.virtualFolder         = xhtmldbgdoc
qhp.xhtmldbg.indexTitle            = XHTML/HTML Reference API
qhp.xhtmldbg.indexRoot             =
qhp.xhtmldbg.filterAttributes      = xhtmldbg @XHTMLDBG_VERSION@ qtrefdoc
qhp.xhtmldbg.customFilters.Qt.name = xhtmldbg @XHTMLDBG_VERSION@
qhp.xhtmldbg.file                  = xhtmldbg.qhp
qhp.xhtmldbg.extraFiles            = xhtmldbg.png

language                     = Cpp
# The headerdirs variable specifies the directories 
# containing the header files associated 
# with the .cpp source files used in the documentation.
headerdirs                   = @CMAKE_CURRENT_SOURCE_DIR@/src/interface
# The sourcedirs variable specifies the 
# directories containing the .cpp or .qdoc 
# files used in the documentation.
sourcedirs                   = @CMAKE_CURRENT_SOURCE_DIR@/src/interface
# The exampledirs variable specifies the directories containing 
# the source code of the example files.
exampledirs                  = .
# The imagedirs variable specifies the 
# directories containing the images used in the documentation.
imagedirs                    = @CMAKE_CURRENT_SOURCE_DIR@/data/icontheme/32/apps/
sources.fileextensions       = "*.cpp *.qdoc"
tagfile                      = @QT_DOC_PATH@/html/qt.tags
base                         = file:@QT_DOC_PATH@/doc/html
