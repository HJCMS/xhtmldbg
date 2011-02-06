prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=$prefix
libdir=$prefix/lib@LIB_SUFFIX@
datadir=$prefix/share/xhtmldbg
includedir=$prefix/include

Name: xhtmldbg
Description: The XHTMLDBG Plugin Interface Library
Url: http://www.hjcms.de
Version: @XHTMLDBG_VERSION@
Requires: QtGui >= 4.7.0 QtCore >= 4.7.0
Libs: -L$libdir -l@INCLUDE_NAME_DIR@
Libs.private: 
Cflags: -I$includedir/@INCLUDE_NAME_DIR@
