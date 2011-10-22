#!/bin/sh
#################################################################
# Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2011
#
# Copyright: See COPYING file that comes with this distribution
#################################################################

if test -f /srv/projects/projectconfigurations.sh ; then
source /srv/projects/projectconfigurations.sh
fi

if test `uname -m` = i686 ; then
  CMAKE_EXTRAS="$CMAKE_EXTRAS -DFORCE_RAPTOR2:BOOL=ON"
else
  CMAKE_EXTRAS="$CMAKE_EXTRAS -DFORCE_RAPTOR2:BOOL=OFF"
fi

cmake -Wdev \
  -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/mpicxx \
  -DCMAKE_CXX_FLAGS:STRING="${CFLAGS:-"-O2"}" \
  -DCMAKE_INSTALL_PREFIX:PATH=/usr \
  -DCMAKE_BUILD_TYPE:STRING=Debug \
  -DXHTMLDBG_BUILD_VERBOSE:BOOL=ON \
  -DCMAKE_SKIP_RPATH:BOOL=ON \
  -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
  -DXHTMLDBG_EXPERIMENTAL:BOOL=ON \
  -DPHONON_STL_INCLUDE_DIR:PATH=/usr/include/kde4/KDE \
  -DAUTOMOC4_EXECUTABLE:FILEPATH=$(which automoc4) \
  $CMAKE_EXTRAS $@ ../
