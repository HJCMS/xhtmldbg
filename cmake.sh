#!/bin/sh
#################################################################
# Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010
#
# Copyright: See COPYING file that comes with this distribution
#################################################################

base=/usr/src/packages/Heinemann/Projekte/xhtmldbg

test -d "${base}" || {
  echo "Missing Directory"
  exit 1
}

debug_build_target=${base}/build

if test "`hostname`" = "webmast" ; then
  TUNE="-O3 -mtune=athlon-xp -march=i686 -mno-sse3"
else
  TUNE="-O2 -mtune=generic -march=`uname -i`"
fi

## needed for some development tests
MDEFS=""
if test -d /usr/src/packages/Heinemann/Projekte/xhtmldbg ; then
MDEFS="-DMAINTAINER_REPOSITORY"
fi

MY_CFLAGS="${TUNE} -ggdb3 -gstabs+ -Wformat -Woverloaded-virtual -Wmissing-field-initializers -pedantic ${MDEFS}"
MY_CFLAGS="$MY_CFLAGS -Wlogical-op -Wmissing-declarations -ffast-math -fstrict-aliasing -finline-functions"
MY_CFLAGS="$MY_CFLAGS -fomit-frame-pointer -funwind-tables -fasynchronous-unwind-tables -fexpensive-optimizations -pipe"

INSTALL_DESTDIR="`mktemp --tmpdir=/tmp xhtmldbg.XXXXXXXXXX`"

function runcmake()
{
  cmake -Wdev \
    -DCMAKE_CXX_FLAGS:STRING="$MY_CFLAGS" \
    -DCMAKE_INSTALL_PREFIX:PATH=/usr \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DXHTMLDBG_BUILD_VERBOSE:BOOL=ON \
    -DCMAKE_SKIP_RPATH:BOOL=ON \
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
    -DXHTMLDBG_EXPERIMENTAL:BOOL=ON \
    -DAUTOMOC4_EXECUTABLE:FILEPATH=$(which automoc4) \
    -DCPACK_PACKAGE_INSTALL_DIRECTORY:PATH="$INSTALL_DESTDIR" $@ ../
}

echo "cmake"

if test "$PWD" = "${debug_build_target}" ; then
  if test -f cmake_install.cmake ; then
    rm -rf ${debug_build_target}/*
    mkdir -p ${debug_build_target}/app/{designer,plugins}
  fi
  runcmake $@
else
  mkdir -p ${debug_build_target}/app/{designer,plugins}
  pushd ${debug_build_target}
    runcmake $@
  popd
fi
