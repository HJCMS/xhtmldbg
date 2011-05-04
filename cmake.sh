#!/bin/sh
#################################################################
# Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2011
#
# Copyright: See COPYING file that comes with this distribution
#################################################################

if test -f $HOME/hjcms/projectconfigurations.txt ; then
. $HOME/hjcms/projectconfigurations.txt
fi

cmake -Wdev \
	-DCMAKE_CXX_FLAGS:STRING="${CFLAGS:-"-O2"}" \
	-DCMAKE_INSTALL_PREFIX:PATH=/usr \
	-DCMAKE_BUILD_TYPE:STRING=Debug \
	-DXHTMLDBG_BUILD_VERBOSE:BOOL=ON \
	-DCMAKE_SKIP_RPATH:BOOL=ON \
	-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
	-DXHTMLDBG_EXPERIMENTAL:BOOL=ON \
	-DPHONON_STL_INCLUDE_DIR:PATH=/usr/include/KDE \
	-DAUTOMOC4_EXECUTABLE:FILEPATH=$(which automoc4) \
	-DCPACK_PACKAGE_INSTALL_DIRECTORY:PATH="$INSTALL_DESTDIR" $@ ../
