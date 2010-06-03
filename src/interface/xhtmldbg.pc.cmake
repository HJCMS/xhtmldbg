prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=$prefix
libdir=$prefix/lib@LIB_SUFFIX@
datadir=$prefix/share/xhtmldbg
includedir=$prefix/include

Name: xhtmldbg
Description: The XHTMLDBG Plugin Interface Library
Url: http://www.hjcms.de
Version: @XHTMLDBG_VERSION@
Requires: QtGui >= 4.6.0 QtCore >= 4.6.0
Libs: -L$libdir -lxhtmldbg-1.0
Libs.private: 
Cflags: -I$includedir/xhtmldbg-1.0
